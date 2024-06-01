#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
#include "Texture.h"
#include "Physics.h"
#include "Camera.h"

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

struct InstallStruct {

	VertexArray *vao;
	VertexBuffer *vbo;
	VertexBufferLayout *layout;
	Texture *text;
	Object3D *objFile;
};


GLfloat ZOOM = 10.0f;
float angle = 0.0f;

glm::mat3 NormalMatrix;

void SetUniforms(Shader shader);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);





void Install(InstallStruct pack);


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "ERROR " << message;
}
void print_error(int error, const char* description);




bool spot = true, point = true, dir = true, amb = true, material = true, isMoving = false;


int main() {


	GLFWwindow* window;


	glfwSetErrorCallback(print_error);



	if (!glfwInit()) return -1;


	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);


	window = glfwCreateWindow(videoMode->width, videoMode->height, "P3D - Trabalho Pratico 1 (Part #1)", nullptr, NULL);
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
	glfwSetKeyCallback(window, key_callback);


	glfwSetScrollCallback(window, scrollCallback);



	const int MODEL_COUNT=15;

	

	Shader shader("shaders/shader.frag", "shaders/shader.vert");
	//Shader shader("../../ProjetoP3D/ProjetoP3D/shaders/shader.frag", "../../ProjetoP3D/ProjetoP3D/shaders/shader.vert");

	VertexBufferLayout layout_v;

	layout_v.Push<glm::vec3>(1); //vertices
	layout_v.Push<glm::vec3>(1); //normais
	layout_v.Push<glm::vec3>(1); //UVs

	std::vector<Object3D> models;
	VertexBuffer VBOs[MODEL_COUNT];
	VertexArray VAOs[MODEL_COUNT];
	Texture Textures[MODEL_COUNT];


	for (int i = 0; i < MODEL_COUNT; i++) {

		InstallStruct installStruct;
		
		std::string fileName = "OBJ files/Ball" + std::to_string(i + 1) + ".obj";

		Object3D model(fileName.c_str());

		models.push_back(model);

		installStruct.layout = &layout_v;
		installStruct.objFile = &model;
		installStruct.vao = &VAOs[i];
		installStruct.vbo = &VBOs[i];
		installStruct.text = &Textures[i];

		Install(installStruct);
	}
	
	shader.Bind();

	
	Object3D Table("OBJ files/cube.obj", false);
	
	VertexArray VAO_table;
	VertexBuffer VBO_table;

	InstallStruct installStruct;
	installStruct.layout = &layout_v;
	installStruct.objFile = &Table;
	installStruct.vao = &VAO_table;
	installStruct.vbo = &VBO_table;
	
	Install(installStruct);

	shader.Bind();
	

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)videoMode->width/(float)videoMode->height, 0.1f, 1000.f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, ZOOM),	// Posição da câmara no mundo
		glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	glm::mat4 ModelView = view * model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));


	glm::vec3 white(1.0f, 1.0f, 1.0f);
	float intensity = 0.0f;
	glm::vec3 direction(2.0f, -1.0f, -2.0f);
	


	



	shader.SetUniform1i("Texture", 0);


	shader.SetUniformMat4f("Model", model);
	shader.SetUniformMat4f("View", view);
	shader.SetUniformMat4f("ModelView", ModelView);
	shader.SetUniformMat3f("NormalMatrix", NormalMatrix);
	shader.SetUniformMat4f("Projection", projection);

	Buffer buffer;


#pragma region Lights

	Light* light_Ptr[4];  // Array of pointers to lights

	// Initialize lights
	light_Ptr[0] = new AmbientLight(glm::vec3(0.1f, 0.1f, 0.1f));

	light_Ptr[1] = new DirectionalLight(
		glm::vec3(0.2f, 0.2f, 0.2f),  // ambient
		glm::vec3(1.0f, 1.0f, 1.0f),  // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),  // specular
		glm::vec3(0.0f, 0.0f, 0.0f)   // direction
	);
	light_Ptr[2] = new SpotLight(
		glm::vec3(0.1f, 0.1f, 0.1f),  // ambient
		glm::vec3(1.0f, 1.0f, 1.0f),  // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),  // specular
		glm::vec3(0.0, 10.0f, 0.0f),  // position
		glm::vec3(0.0f, -1.0f, 0.0f), // direction
		1.0f,                          // constant
		0.045f,                        // linear
		0.0075f,                       // quadratic
		10.5f,                         // spout cut off
		12.0f                          // spot exponent
	);
	light_Ptr[3] = new PointLight(
		glm::vec3(0.1f, 0.1f, 0.1f),  // ambient
		glm::vec3(1.0f, 1.0f, 1.0f),  // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),  // specular
		glm::vec3(0.0f, 0.0f, 5.0f),  // position
		1.0f,                          // constant
		0.06f,                        // linear
		0.02f                        // quadratic
	);

	for (int i=0; i < 4; i++) {

		light_Ptr[i]->UseLight(&shader);

	}

#pragma endregion

	
	SetUniforms(shader);
	
	
	
	glEnable(GL_DEPTH_TEST);

	glm::vec3 world_position(3.0f, 0.0f, 6.0f);


	float offsetX[15] = { -5.0f,-2.5f,0.0f,2.5f, 5.0f,	-3.75f,-1.25f, 1.25f, -3.75f, 		-2.5f, 0.0f, 2.5f,		-1.25f, 1.25f,		0.0f};
	float offsetZ[15] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,		2.5f, 2.4f, 2.5f, 2.5f, 		5.0f, 5.0f,5.0f,		7.5f, 7.5f,		10.0f};


	int j = 0;
	
	Physics physics;
	
	std::vector<glm::mat4> ModelViews;

	for (int i = 0; i < MODEL_COUNT; i++) {
		ModelViews.push_back(ModelView);
	}


	int factor = 1;

	Mouse mouse;
	glm::mat4 rotation = glm::mat4(1.0f);

	while (!glfwWindowShouldClose(window))
	{


		buffer.Clear();

		// Model

		view = glm::lookAt(world_position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		world_position.z = ZOOM;

		//para o spotlight nao seguir a camara
		shader.SetUniformMat4f("View", view);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader.SetUniform1i("spotLightOn", spot);
		shader.SetUniform1i("pointLightOn", point);
		shader.SetUniform1i("directionalLightOn", dir);
		shader.SetUniform1i("ambientLightOn", amb);
		
		glm::vec3 translation;


		mouse.CamMove(window);
		glm::quat quaternionY = glm::angleAxis(mouse.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat quaternionX = glm::angleAxis(mouse.angleX, glm::vec3(1.0, 0.0f, 0.0f));

		shader.SetUniform1i("IsTable", false);
	
		
		for (int i = 0; i < MODEL_COUNT; i++)
		{



			translation = glm::vec3(offsetX[i], 0, offsetZ[i]);


			if (!physics.CheckCollisions(ModelViews[i], ModelViews))//check collision 
			{
			
				if (isMoving) {

					if (i == 0)
					{
						translation = glm::vec3(offsetX[i] += 0.01f, 0, offsetZ[i]);
					}
				}
			}
			else 
			{
				translation = glm::vec3(offsetX[i], 0, offsetZ[i]);
			}
			model = glm::translate(glm::mat4(1.0f), translation);



			model = glm::toMat4(quaternionY) * model;
			model = glm::toMat4(quaternionX) * model;


			ModelViews[i] = view * model;
			NormalMatrix = glm::inverseTranspose(glm::mat3(ModelViews[i]));
			
		

			shader.SetUniformMat4f("ModelView", ModelViews[i]);
			shader.SetUniformMat3f("NormalMatrix", NormalMatrix);
		
			

			if (material)
			{
				shader.SetUniform3f("material.emissive", 0.0f, 0.0f, 0.0f);
				shader.SetUniformVec3("material.ambient", models[i].material.ambient);
				shader.SetUniformVec3("material.diffuse", models[i].material.difuse);
				shader.SetUniformVec3("material.specular", models[i].material.specular);
				shader.SetUniform1f("material.shininess", models[i].material.shininess);
			}
			else 
			{
				// Material properties
				shader.SetUniform3f("material.emissive", 0.0f, 0.0f, 0.0f);
				shader.SetUniform3f("material.ambient", 1.0f, 1.0f, 1.0f);
				shader.SetUniform3f("material.diffuse", 1.0f, 1.0f, 1.0f);
				shader.SetUniform3f("material.specular", 1.0f, 1.0f, 1.0f);
				shader.SetUniform1f("material.shininess", 12.0f);
			}
			

			Textures[i].Bind();

			buffer.Draw(VAOs[i], models[i].VertexCount, shader);

		}

		//----------------------MESA----------------------------
		
		translation = glm::vec3(0.0f, -4.0f, 0.0f);
		

		model = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 0.5f, 30.0f));
		model = glm::translate(model, translation);

		model = glm::toMat4(quaternionY) * model;
		model = glm::toMat4(quaternionX) * model;

		ModelView = view * model;
		NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));
		

		shader.SetUniform3f("material.emissive", 0.0f, 0.0f, 0.0f);
		shader.SetUniformVec3("material.ambient", Table.material.ambient);
		shader.SetUniformVec3("material.diffuse", Table.material.difuse);
		shader.SetUniformVec3("material.specular",Table.material.specular);
		shader.SetUniform1f("material.shininess", Table.material.shininess);
		

		shader.SetUniformMat4f("ModelView", ModelView);
		shader.SetUniformMat3f("NormalMatrix", NormalMatrix);

		shader.SetUniform1i("IsTable", true);

		buffer.Draw(VAO_table, Table.VertexCount, shader);
		
		
		
		//-------------------------------------------------------
		


		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}


void SetUniforms(Shader shader) {

	// Fonte de luz ambiente global
	glm::vec3 amb (0.1f, 0.1f, 0.1f);
	shader.SetUniformVec3("ambientLight.ambient", amb);

	// Fonte de luz direcional
	shader.SetUniform3f("directionalLight.direction", 0.0f, 0.0f, 0.0f);
	shader.SetUniform3f("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniform3f("directionalLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("directionalLight.specular", 1.0f, 1.0f, 1.0f);

	// Fonte de luz pontual #1
	shader.SetUniform3f("pointLight.position", 0.0f, 0.0f, 5.0f);
	shader.SetUniform3f("pointLight.ambient", 0.1f, 0.1f, 0.1f);
	shader.SetUniform3f("pointLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("pointLight.specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform1f("pointLight.constant", 1.0f);
	shader.SetUniform1f("pointLight.linear", 0.06f);
	shader.SetUniform1f("pointLight.quadratic", 0.02f);


	//Fonte de luz holofote 
	shader.SetUniform3f("spotLight.position", 0.0, 10.0f, 0.0f);
	shader.SetUniform3f("spotLight.ambient", 0.3f, 0.3f, 0.3f);
	shader.SetUniform3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform1f("spotLight.constant", 1.0f);
	shader.SetUniform1f("spotLight.linear", 0.045f);
	shader.SetUniform1f("spotLight.quadratic", 0.0075f);

	shader.SetUniform1f("spotLight.spotCutoff", cos(glm::radians(10.5f)));
	shader.SetUniform1f("spotLight.spotExponent", cos(glm::radians(12.0f)));
	shader.SetUniform3f("spotLight.spotDirection", 0.0f, -1.0f, 0.0f);


}


void Install(InstallStruct pack)
{

	if (pack.objFile->material.texture != "X") {
		std::string image_path = "Texture/" + pack.objFile->material.texture;
		pack.text->LateInit(image_path);
	}

	pack.vbo->LateInit(&pack.objFile->meshVector[0], pack.objFile->meshVector.size() * sizeof(glm::vec3));

	pack.vao->AddBuffer(*pack.vbo, *pack.layout);

	pack.vao->Bind();
}

#pragma region control callbacks


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		amb = !amb;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		dir = !dir;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		point = !point;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		spot = !spot;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		material = !material;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		isMoving = true;
	}

	
}



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


#pragma endregion



void print_error(int error, const char* description) {
	std::cout << description << std::endl;
}