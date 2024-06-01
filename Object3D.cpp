#include "Object3D.h"

Object3D::Object3D(const char* path)
{
	loadOBJ(path);

	VertexCount = getVertexCount();

	std::string mp = "MTL/" + material.mtl_path;
	loadMTL(mp.c_str());


}

Object3D::Object3D(const char* path, bool _mtl)
{
	loadOBJ(path);
	VertexCount = getVertexCount();
	loadDefaultMTL();
}


Object3D::~Object3D()
{

}

int Object3D::getVertexCount()
{
	return meshVector.size()/3;
}

bool Object3D ::loadOBJ(const char* path){
	
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

			
			
	printf("Loading OBJ file %s...\n", path);


	
	FILE* file = fopen(path, "r");
	
	if (file == NULL) {
		printf("Impossible to open the file !\n"); 
		getchar();
		return false;
	}

	while (1) //lê linha a linha o ficheiro obj e armazena os: V, vt, vn e f
	{
		char lineHeader[128];
		//ler a primeira palavra da linha
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // fim do ficheiro

		//ler o mtl tambem


		if (strcmp(lineHeader, "v") == 0) { //vertices
			glm::vec3 vertex; 
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) { //coordenadas de textura
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) { //normais
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) { //faces e indices 
			
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("ficheiro incompativel\n");
				getchar();
				return false;
			}
		
			//indices dos vertices
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			//indices dos UVs
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			//indices dos normais
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else if (strcmp(lineHeader, "mtllib") == 0) { //nome do ficheiro da imagem de textura:
			char temp_buffer[128];
			fscanf(file, "%127s", temp_buffer);
			std::string temp_mtl = temp_buffer;
		
			material.mtl_path = temp_mtl;
			printf("temp_mtl : %s\n",temp_mtl);
		
		}
		else {
			// comentarios, etc...
			char miscBuffer[1000];
			fgets(miscBuffer, 1000, file);
		}
	}
	
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		Mesh tempMesh;

		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		tempMesh.vertices = vertex;
		tempMesh.normals = normal;
		tempMesh.uvs = uv;

		meshVector.push_back(vertex);
		meshVector.push_back(normal);
		meshVector.push_back(glm::vec3(uv.x, uv.y, 1.0f));

		meshStruct.push_back(tempMesh);
	}
	fclose(file);

	
	return true;


}

bool Object3D::loadMTL(const char* path) 
{
	glm::vec3 temp_ambient;
	glm::vec3 temp_difuse;
	glm::vec3 temp_specular;
	float temp_shininess;
	std::string temp_text;

	printf("Loading MTL file %s...\n", path);


	FILE* file = fopen(path, "r");

	if (file == NULL) {
		printf("Impossible to open the file !\n");
		getchar();
		return false;
	}

	while (1) //lê linha a linha o ficheiro mtl e armazena os: Ka, Kd, Ks e Ns
	{
		char lineHeader[128];
		//ler a primeira palavra da linha
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // fim do ficheiro

		if (strcmp(lineHeader, "Ns") == 0) { //Ns Expoente especular
			
			fscanf(file, "%f\n", &temp_shininess);
		}
		else if (strcmp(lineHeader, "Ka") == 0) { //Ka Coeficiente de reflexão da luz ambiente 

			fscanf(file, "%f %f %f\n", &temp_ambient.x, &temp_ambient.y, &temp_ambient.z);
		}
		else if (strcmp(lineHeader, "Kd") == 0) { //Kd Coeficiente de reflexão da luz difusa
			
			fscanf(file, "%f %f %f\n", &temp_difuse.x, &temp_difuse.y, &temp_difuse.z);
		}
		else if (strcmp(lineHeader, "Ks") == 0) { //Ks Coeficiente de reflexão da luz especular 
		
			fscanf(file, "%f %f %f\n", &temp_specular.x, &temp_specular.y, &temp_specular.z);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0) { //nome do ficheiro da imagem de textura:
			char temp_buffer[128]; 
			fscanf(file, "%127s", temp_buffer); 
			temp_text = temp_buffer;
			material.texture = temp_text;

		}
	}

	fclose(file);
	
	material.shininess = temp_shininess;
	material.difuse = temp_difuse;
	material.specular = temp_specular;
	material.ambient = temp_ambient;
	

	return true;


}

bool Object3D::loadDefaultMTL()
{


	material.shininess = 12.0f;
	material.difuse = glm::vec3(1.0f, 1.0f, 1.0f);;
	material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	material.texture = "X";


	return true;
}

void Object3D::printInfo(int numLines) {
	// Print vertices
	
	std::cout << "Vertices:" << std::endl;
	for (int i = 0; i < meshVector.size() && (numLines == -1 || i < numLines); i+=3) {
		const auto& vertex = meshVector[i];
		std::cout << "  (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
		
		std::cout << "UVs:" << std::endl;
		const auto& uv = meshVector[i+1];
		std::cout << "  (" << uv.x << ", " << uv.y << ")" << std::endl;
	
		const auto& normal = meshVector[i+2];
		std::cout << "  (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
	
	}
}




