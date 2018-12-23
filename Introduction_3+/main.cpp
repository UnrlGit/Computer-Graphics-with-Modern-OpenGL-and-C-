#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "MyGLWindow.h"
#include "Camera.h"
#include "Texture.h"

//#include "stb_image.h"

//const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159262f / 180.f;

MyGLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";
//Fragment shader
static const char * fShader = "Shaders/shader.frag";

//const char * brickTexturepath = "Textures/brick.png";

void CreateObjects() {
	//INDEX DRAW STUFF (also IBO at start)
	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2		
	};

	GLfloat vertices[] = {
		// x    y      z	  u		v
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	 0.5, 0.0f, 
		1.0f, -1.0f, 0.0f,	 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	 0.5f, 1.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indicies, 20, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indicies, 20, 12);
	meshList.push_back(obj2);
	
}



void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main() 
{
	mainWindow = MyGLWindow(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// camera init
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	// uniformView is for camera
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// loop until window closed
	while (mainWindow.getShouldClose() == false)
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now; 

		// s cim ocistiti sto
		// clear window
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		// what needs to be "cleared"
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		// uniformView is still for camera
		uniformView = shaderList[0]->GetViewLocation();

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));		
		model = glm::scale(model, glm::vec3(0.4f, 0.4, 1.0f));	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));	
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4{ 1.0f };
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh();

		//unassign shader
		glUseProgram(0);

		mainWindow.swapBuffers();

		// get and handle user input events - check for any action like key presses, etc.
		glfwPollEvents();

		//check for key presses
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
	}

	return 0;
}