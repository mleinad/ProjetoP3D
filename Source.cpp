#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h";

//#include "Luis/Renderer.h"
//#include "Luis/VertexBuffer.h"
//#include "Luis/IndexBuffer.h"
//#include "Luis/VertexArray.h"
//#include "Luis/VertexBufferLayout.h"

#include <sstream>



#include "Object3D.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <thread>
#include "Light.h"

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

GLfloat ZOOM = 10.0f;
float angle = 0.0f;

glm::mat3 NormalMatrix;


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//zoom in
	if (yoffset == 1) {

		ZOOM -= fabs(ZOOM) * 0.1f;
	}
	//zoom out
	else if (yoffset == -1) {

		ZOOM += fabs(ZOOM) * 0.1f;

	}

}
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "ERROR " << message;
}

void print_error(int error, const char* description);
void load_textures(std::vector<std::string> textureFiles);

void display(std::vector<glm::vec3> obj, glm::mat4 mvp) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe

	float* vertex_stream = static_cast<float*>(glm::value_ptr(obj.front()));

	std::vector<glm::vec3> colors{
		glm::vec3(1.0f, 0.0f, 0.0f), // Red
		glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
		glm::vec3(0.0f, 1.0f, 0.0f), // Green
		glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
		glm::vec3(0.0f, 0.0f, 1.0f), // Blue
		glm::vec3(1.0f, 0.0f, 1.0f)  // Magenta
	};

	// Desenha quad em modo imediato
	glBegin(GL_TRIANGLES);
	/* obj.size() * (obj.front().length()) é o mesmo que (6*4)*3 */
	/* 6 faces * 4 vértices por face * 3 coordenadas por vértice */
	for (size_t i = 0; i < obj.size(); ++i) {
		// Assign a color based on the face index, reusing colors if needed
		const glm::vec3& color = colors[i / 3 % colors.size()];
		glColor3f(color.r, color.g, color.b);

		// Access the vertex
		const glm::vec3& vertex = obj[i];

		// Transform the vertex by the MVP matrix
		glm::vec4 transformed_vertex = mvp * glm::vec4(vertex, 1.0f);

		// Perform perspective division
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;

		// Issue the vertex to OpenGL
		glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
	}
	glEnd();
}


void draw(glm::mat4 projection, std::vector<glm::vec3> object, float& angle)
{


	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, ZOOM),	// Posição da câmara no mundo
		glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	// Model
	glm::mat4 model = glm::mat4(1.0f);

	// Vai efetuando uma rotação ao objeto (apenas para podermos ver todas as faces desenhadas).
	model = glm::rotate(model, angle += 0.001, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

	// MVP
	glm::mat4 mvp = projection * view * model;

	display(object, mvp);
}


int main() {


	GLFWwindow* window;


	glfwSetErrorCallback(print_error);



	if (!glfwInit()) return -1;



	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);


	window = glfwCreateWindow(1600, 900, "P3D - Trabalho Pratico 1 (Part #1)", nullptr, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit()) { //Access violation executing location 0x00000000 sem isto depois da window 
		printf("failed to initialize OpenGL\n");
		return -1;
	}


	// Enable debug output
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);


	glfwSetScrollCallback(window, scrollCallback);


	Object3D ball("OBJ files/Ball1.obj");

	Object3D cube("OBJ files/cube.obj");

	ball.loadMTL("Ball1.mtl");



	Shader shader("shaders/shader.frag", "shaders/shader.vert");



	std::vector<Object3D> models;
//	std::vector<VertexBuffer> VBOs;

	VertexArray VAOs[3];

	for (int i = 0; i < 3; i++) {

		std::string fileName = "OBJ files/Ball" + std::to_string(i + 1) + ".obj";
		
		Object3D model(fileName.c_str());
		models.push_back(model);

	}

		VertexBufferLayout layout_v;

		layout_v.Push<glm::vec3>(1);
		layout_v.Push<glm::vec3>(1);



		VertexBuffer VBO_1(&models[0].mesh[0], models[0].mesh.size() * sizeof(glm::vec3));
		//VBOs.push_back(VBO_1);

		VertexBuffer VBO_2(&models[1].mesh[0], models[1].mesh.size() * sizeof(glm::vec3));
		//VBOs.push_back(VBO_2);

		VertexBuffer VBO_3(&models[2].mesh[0], models[2].mesh.size() * sizeof(glm::vec3));
		//VBOs.push_back(VBO_3);


		VAOs[0].AddBuffer(VBO_1, layout_v);
		
		VAOs[1].AddBuffer(VBO_2, layout_v);
		
		VAOs[2].AddBuffer(VBO_3, layout_v);

		
		VAOs[0].Bind();

		shader.Bind();

		VAOs[1].Bind();

		shader.Bind();

		VAOs[2].Bind();

		shader.Bind();






	//object.printInfo();


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, ZOOM),	// Posição da câmara no mundo
		glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;


	glm::vec3 white(1.0f, 1.0f, 1.0f);
	float intensity = 0.0f;
	glm::vec3 direction(2.0f, -1.0f, -2.0f);
	Light light(white, intensity, direction, 1.0f);


	shader.Bind(); 


	shader.SetUniformMat4f("u_MPV", mvp);

	shader.SetUniform4f("u_Color", 0.8f, 1.0f, 0.8f, 1.0f);

	shader.SetUniform1f("material.specularIntensity", 1.0f);
	shader.SetUniform1f("material.shininess", 32.0f);


	int i = 0;


	shader.Unbind();

	Buffer buffer;

	for (int i = 0; i < 3; i++)
	{
		VAOs[i].Unbind();
	}

	VBO_1.Unbind();
	VBO_2.Unbind();
	VBO_3.Unbind();

	shader.Unbind();


	glEnable(GL_DEPTH_TEST);

	glm::vec3 world_position(0.0f, 0.0f, 0.0f);
	float offset[3] = {-3.0f, 0.0f, 3.0f};

	while (!glfwWindowShouldClose(window))
	{


		buffer.Clear();

		// Model

		view = glm::lookAt(world_position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		world_position.z = ZOOM;


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		

			shader.Bind();
			light.UseLight(shader);

			glm::vec3 translation;

			for (int i=0; i<3; i++)
			{
				translation = glm::vec3(offset[i], 0, 0);

				model = glm::translate(glm::mat4(1.0f), translation);
				mvp = projection * view * model;
				shader.SetUniformMat4f("u_MPV", mvp);
				buffer.Draw(VAOs[i], models[i].vertices.size(), shader);
			}






		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void print_error(int error, const char* description) {
	std::cout << description << std::endl;
}



void load_textures(std::vector<std::string> textureFiles) {

	GLuint textureName = 0;

	// Gera um nome de textura
	glGenTextures(1, &textureName);

	// Ativa a Unidade de Textura #0
	// A Unidade de Textura 0 está ativa por defeito.
	// Só uma Unidade de Textura pode estar ativa.
	glActiveTexture(GL_TEXTURE0);

	// Vincula esse nome de textura ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureName);

	// NOTA:
	// Num cube map de texturas, todas as texturas devem:
	// - ter a mesma resolução;
	// - possuir o mesmo número de níveis de mipmap; e,
	// - partilhar os mesmos parâmetros.

	// Define os parâmetros de filtragem (wrapping e ajuste de tamanho)
	// para a textura que está vinculada ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#ifdef _D_STORAGE
	// Aloca memória para o cube map de texturas
	// Textura imutável, i.e., apenas é possível alterar a imagem.
	{
		// Leitura da resolução e número de canais da imagem.
		int width, height, nChannels;
		// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
		stbi_set_flip_vertically_on_load(true);
		unsigned char* imageData = stbi_load(textureFiles[0].c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			stbi_image_free(imageData);

			// Alocação de memória
			glTexStorage2D(GL_TEXTURE_CUBE_MAP,
				1,					// Número de níveis de Mipmap para as texturas. 1 se não forem utilizados Mipmaps.
				nChannels == 4 ? GL_RGBA8 : GL_RGB8,	// Formato interno da imagem de textura
				width, height		// width, height
			);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}

	// Para cada face do cubo
	GLint face = 0;
	for (auto file : textureFiles) {
		// Leitura/descompressão do ficheiro com imagem de textura
		int width, height, nChannels;
		unsigned char* imageData = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target da face
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,					// Nível do Mipmap
				0, 0,				// xoffset, yoffset
				width, height,		// width, height
				nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
				GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
				imageData);			// Apontador para os dados da imagem de textura

			face++;

			// Liberta a imagem da memória do CPU
			stbi_image_free(imageData);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}
#else
	// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
	stbi_set_flip_vertically_on_load(true);

	// Para cada face do cubo
	GLint face = 0;

	for (auto file : textureFiles) {
		// Leitura/descompressão do ficheiro com imagem de textura
		int width, height, nChannels;
		unsigned char* imageData = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target da face
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,					// Nível do Mipmap
				GL_RGB,				// Formato interno do OpenGL
				width, height,		// width, height
				0,					// border
				nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
				GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
				imageData);			// Apontador para os dados da imagem de textura

			face++;

			// Liberta a imagem da memória do CPU
			stbi_image_free(imageData);
		}
		else {
			std::cout << "Error loading texture!" << std::endl;
		}
	}
#endif
}