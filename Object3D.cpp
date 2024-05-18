#include "Object3D.h"

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
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
		return false;
	}

	while (1) //lê linha a linha o ficheiro obj e armazena os: V, vt, vn e f
	{
		char lineHeader[128];
		//ler a primeira palavra da linha
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // fim do ficheiro

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
		else {
			// comentarios, etc...
			char miscBuffer[1000];
			fgets(miscBuffer, 1000, file);
		}
	}
	
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex-1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

	
		this->vertices.push_back(vertex);
		this->uvs.push_back(uv);
		this->normals.push_back(normal);

	}
	fclose(file);
	return true;

}

void Object3D::printInfo(int numLines) {
	// Print vertices
	std::cout << "Vertices:" << std::endl;
	for (int i = 0; i < vertices.size() && (numLines == -1 || i < numLines); ++i) {
		const auto& vertex = vertices[i];
		std::cout << "  (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
	}

	// Print texture coordinates (UVs)
	std::cout << "UVs:" << std::endl;
	for (int i = 0; i < uvs.size() && (numLines == -1 || i < numLines); ++i) {
		const auto& uv = uvs[i];
		std::cout << "  (" << uv.x << ", " << uv.y << ")" << std::endl;
	}

	// Print normals
	std::cout << "Normals:" << std::endl;
	for (int i = 0; i < normals.size() && (numLines == -1 || i < numLines); ++i) {
		const auto& normal = normals[i];
		std::cout << "  (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
	}
}