#include "Object3D.h"


class Object3D
{
public:
	Object3D();
	~Object3D();
	
	bool loadOBJ(const char* path);


		
	std::vector < glm::vec3 > vertices;
	std::vector < glm::vec2 > uvs;
	std::vector < glm::vec3 > normals;

private:



};

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

		if (strcmp(lineHeader, "V") == 0) { //vertices
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
			fscanf(file, "%f %f %f\n", &normal.x, normal.y, normal.z);
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


	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		this->vertices.push_back(vertex);
		this->uvs.push_back(uv);
		this->normals.push_back(normal);

	}
	fclose(file);
	return true;

}