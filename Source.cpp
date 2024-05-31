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
#include "Texture.h"
#include "Physics.h"

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

GLfloat ZOOM = 10.0f;
float angle = 0.0f;

glm::mat3 NormalMatrix;

void SetUniforms(Shader shader);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


void print_error(int error, const char* description);


bool spot = true, point = true, dir = true, amb = true, material = true;


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



	const int MODEL_COUNT=2;

	

	//Shader shader("shaders/shader.frag", "shaders/shader.vert");
	Shader shader("../../ProjetoP3D/ProjetoP3D/shaders/shader.frag", "../../ProjetoP3D/ProjetoP3D/shaders/shader.vert");

		VertexBufferLayout layout_v;

		layout_v.Push<glm::vec3>(1); //vertices
		layout_v.Push<glm::vec3>(1); //normais
		layout_v.Push<glm::vec3>(1); //UVs


	std::vector<Object3D> models;
	VertexBuffer VBOs[MODEL_COUNT];
	VertexArray VAOs[MODEL_COUNT];

	std::string image_path = "Texture/PoolBalluv1.jpg";	
	Texture texture(image_path);

	for (int i = 0; i < MODEL_COUNT; i++) {

		std::string fileName = "OBJ files/Ball" + std::to_string(i + 1) + ".obj";
		

		Object3D model(fileName.c_str());

		models.push_back(model);

		VBOs[i].LateInit(&model.meshVector[0], models[0].meshVector.size() * sizeof(glm::vec3));

		VAOs[i].AddBuffer(VBOs[i], layout_v);

		VAOs[i].Bind();


	}

		shader.Bind();
		texture.Bind();

		
	
	//	->	ERROR Program/shader state performance warning: Vertex shader in program 1 is being recompiled based on GL state.


		Object3D Table("OBJ files/cube.obj", false);


	VertexBuffer VBO_table(&Table.meshVector[0], Table.meshVector.size()*sizeof(glm::vec3));
	
	VertexArray VAO_table;
	VAO_table.AddBuffer(VBO_table, layout_v);
	texture.Bind();




	shader.Bind();
	
//	table_shader.Unbind();

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


/*	table_shader.Bind();

	table_shader.SetUniformMat4f("Model", model);
	table_shader.SetUniformMat4f("View", view);
	table_shader.SetUniformMat4f("ModelView", ModelView);
	table_shader.SetUniformMat3f("NormalMatrix", NormalMatrix);
	table_shader.SetUniformMat4f("Projection", projection);

	table_shader.SetUniform4f("table_color", 0.05f, 0.87f, 0.24f, 1.0f);

	table_shader.Unbind();
	*/
	Buffer buffer;

	for (int i = 0; i < MODEL_COUNT; i++)
	{
		VAOs[i].Unbind();
		VBOs[i].Unbind();
	}



#pragma region Lights

	Light *light_Ptr[4];

	AmbientLight ambientLight(glm::vec3(0.1f, 0.1f, 0.1f));
	
	DirectionalLight directionLight(
		glm::vec3(0.2f, 0.2f, 0.2f),//ambiente
		glm::vec3(1.0f, 1.0f, 1.0f),//diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), //specular
		glm::vec3(0.0f, 0.0f, 0.0f));//direction

	SpotLight spotLight(
		glm::vec3(0.1f, 0.1f, 0.1f),//ambiente
		glm::vec3(1.0f, 1.0f, 1.0f),//diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),//specular
		glm::vec3(0.0, 10.0f, 0.0f),//position
		glm::vec3(0.0f, -1.0f, 0.0f),//direction
		1.0f,	//constant
		0.045f,	//linear
		0.0075f,//quadratic
		10.5f,	//spout cut off
		12.0f);	//spot exponent

	PointLight pointLight(
		glm::vec3(0.1f, 0.1f, 0.1f),//ambiente
		glm::vec3(1.0f, 1.0f, 1.0f),//diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),//specular
		glm::vec3(0.0f, 0.0f, 5.0f),//position
		1.0f,	//constant
		0.045f,	//linear
		0.0075f//quadratic
	);


	light_Ptr[0] = &ambientLight;
	light_Ptr[1] = &directionLight;
	light_Ptr[2] = &spotLight;
	light_Ptr[3] = &pointLight;


	/*for (int i=0; i < 4; i++) {

		light_Ptr[i]->UseLight(shader);

	}*/

#pragma endregion

	
	SetUniforms(shader);
	//SetUniforms(table_shader);




	glEnable(GL_DEPTH_TEST);

	glm::vec3 world_position(3.0f, 0.0f, 6.0f);
	//	float offset[5] = {-5.0f, -2.0f, 0.0f, 2.0f, 5.0f};
	float offset[2] = { -5.0f, 5.0f };


	int j = 0;
	Physics physics;



	std::vector<glm::mat4> ModelViews;

	for (int i = 0; i < MODEL_COUNT; i++) {
		ModelViews.push_back(ModelView);
	}


	int factor = 1;


	// Push main
	while (!glfwWindowShouldClose(window))
	{


		buffer.Clear();

		// Model

		view = glm::lookAt(world_position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		world_position.z = ZOOM;


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader.SetUniform1i("spotLightOn", spot);
		shader.SetUniform1i("pointLightOn", point);
		shader.SetUniform1i("directionalLightOn", dir);
		shader.SetUniform1i("ambientLightOn", amb);
		/*
		table_shader.Bind();
		table_shader.SetUniform1i("spotLightOn", spot);
		table_shader.SetUniform1i("pointLightOn", point);
		table_shader.SetUniform1i("directionalLightOn", dir);
		table_shader.SetUniform1i("ambientLightOn", amb);
		table_shader.Unbind();*/

		glm::vec3 translation;


		shader.SetUniform1i("IsTable", false);
		for (int i = 0; i < MODEL_COUNT; i++)
		{



			translation = glm::vec3(offset[i], 0, 0);

			if (!physics.CheckCollisions(ModelViews[i], ModelViews))//check collision 
			{
				if (i == 0)
				{
					translation = glm::vec3(offset[i] += 0.01f*factor, 0, 0);
				}
			}
			else 
			{
				translation = glm::vec3(offset[i] * factor, 0, 0);
			}
			model = glm::translate(glm::mat4(1.0f), translation);


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
			

				buffer.Draw(VAOs[i], models[i].getVertexCount(), shader);
		}

		//----------------------MESA----------------------------
		
		translation = glm::vec3(0.0f, -4.0f, 0.0f);
		

		model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 0.5f, 3.5f));
		model = glm::translate(model, translation);

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

		buffer.Draw(VAO_table, Table.getVertexCount(), shader);
		
		
		
		//-------------------------------------------------------
		


		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void print_error(int error, const char* description) {
	std::cout << description << std::endl;
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
	shader.SetUniform3f("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	shader.SetUniform3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform1f("spotLight.constant", 1.0f);
	shader.SetUniform1f("spotLight.linear", 0.045f);
	shader.SetUniform1f("spotLight.quadratic", 0.0075f);

	shader.SetUniform1f("spotLight.spotCutoff", cos(glm::radians(10.5f)));
	shader.SetUniform1f("spotLight.spotExponent", cos(glm::radians(12.0f)));
	shader.SetUniform3f("spotLight.spotDirection", 0.0f, -1.0f, 0.0f);


}



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
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
	}
}

