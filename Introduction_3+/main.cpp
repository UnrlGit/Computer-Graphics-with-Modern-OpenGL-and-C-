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

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"
#include "MyGLWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include "SpotLight.h"
#include "Model.h"

#include <assimp\Importer.hpp>

//#include "stb_image.h"

//const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159262f / 180.f;

MyGLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;
Model testModel;
Model imperialStarDestroyer;
Model moon;
Model sun;



DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";
//Fragment shader
static const char * fShader = "Shaders/shader.frag";

//const char * brickTexturepath = "Textures/brick.png";

void calcAverageNormals(unsigned int * indicies, unsigned int indicieCount, GLfloat * vertices,
	unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indicieCount; i+=3)
	{
		unsigned int in0 = indicies[i] * vLength;
		unsigned int in1 = indicies[i+1] * vLength;
		unsigned int in2 = indicies[i+2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x;
		vertices[in0+1] += normal.y;
		vertices[in0+2] += normal.z;
		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount/vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset+1] = vec.y;
		vertices[nOffset+2] = vec.z;
	}
}

void CreateObjects() {
	//INDEX DRAW STUFF (also IBO at start)
	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2		
	};

	GLfloat vertices[] = {
		// x    y      z	  u		v		nx		ny	nz
		-1.0f, -1.0f, -0.6f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	 0.5, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,	 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	 0.5f, 1.0f,	0.0f, 0.0f, 0.0f,
	};

	unsigned int floorIndicies[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.f,		10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indicies, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indicies, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indicies, 32, 12);
	meshList.push_back(obj2);
	

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndicies, 32, 6);
	meshList.push_back(obj3);
}



void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main() 
{
	mainWindow = MyGLWindow(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// camera init (-180.0f can be put as -90.0f but have 2 find still
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureAlpha();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureAlpha();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureAlpha();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);
	//plainMaterial = Material(0.3f, 4);

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj", "Xwing");

	blackhawk = Model();
	blackhawk.LoadModel("Models/uh60.obj", "Blackhawk");

	//testModel = Model();
	//testModel.LoadModel("Models/voyager.obj", "Voyager"); 


	//imperialStarDestroyer = Model();
	//imperialStarDestroyer.LoadModel("Models/imperial.obj", "Imp");

	//sun = Model();
	//sun.LoadModel("Models/sol.obj", "Sun");

	//moon =  Model();
	//moon.LoadModel("Models/Death_Star.obj", "DeathStar");

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
									0.2f, 0.6f,
									0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 0.1f,
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.0f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
								0.0f, 2.0f, 
								0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f,
								1.0f, 0.0f, 0.0f,
								20.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;


	// uniformView is for camera
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	

	// loop until window closed
	while (mainWindow.getShouldClose() == false)
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now; 

		// s cim ocistiti sto
		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// what needs to be "cleared"
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		// uniformView is still for camera
		uniformView = shaderList[0]->GetViewLocation();
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();

		// lowering light that will be attached to camera
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		lowerLight.x += 1.0f;
		//attaching spot light to camera
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0]->SetDirectionalLight(&mainLight);
		shaderList[0]->SetPointLight(pointLights, pointLightCount);
		shaderList[0]->SetSpotLights(spotLights, spotLightCount);


		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));		
		//model = glm::scale(model, glm::vec3(0.4f, 0.4, 1.0f));	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4{ 1.0f };
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		model = glm::mat4{ 1.0f };
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// FOR XWING
		model = glm::mat4{ 1.0f };
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		xwing.RenderModel();

		// FOR blackhawk
		model = glm::mat4{ 1.0f };
		model = glm::translate(model, glm::vec3(-3.0f, 2.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		blackhawk.RenderModel();

		//// FOR voyager
		//model = glm::mat4{ 1.0f };
		//model = glm::translate(model, glm::vec3(-3.0f, 4.0f, 3.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//testModel.RenderModel();

		//// FOR imperial
		//model = glm::mat4{ 1.0f };
		//model = glm::translate(model, glm::vec3(100.0f, 0.0f, 50.0f));
		//model = glm::scale(model, glm::vec3(0.0033f, 0.0033f, 0.0033f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//imperialStarDestroyer.RenderModel();

		//// FOR moon
		//model = glm::mat4{ 1.0f };
		//model = glm::translate(model, glm::vec3(-3.0f, -10.0f, 3.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//moon.RenderModel();

	

	

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