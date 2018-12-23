#include "MyGLWindow.h"



MyGLWindow::MyGLWindow()
{
	width = 800;
	height = 600;

	InitKeys();
}




MyGLWindow::~MyGLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

MyGLWindow::MyGLWindow(GLfloat windowWidth, GLfloat windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	InitKeys();
}

int MyGLWindow::Initialise()
{
	// init GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialisation failed!");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// means that we are setting profile so it is not bkwards compatible
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// forward compatibility enabled
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set context for GLEW to use - tells gl to draw on this window
	glfwMakeContextCurrent(mainWindow);

	// handle key + mouse input
	createCallbacks();

	// locking mouse to screen
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// allow modern extension features
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//INDEXED DRAWS : DEPTH BUFFER
	glEnable(GL_DEPTH_TEST);

	// setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// camera da bi moglo pokazivati samo na sebe (1. dio spajanja)
	glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat MyGLWindow::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat MyGLWindow::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void MyGLWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void MyGLWindow::handleKeys(GLFWwindow * window, int key, int code, int action, int mode)
{

	// 2. dio spajanja
	MyGLWindow * theWindow = static_cast<MyGLWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}

}

void MyGLWindow::handleMouse(GLFWwindow * window, double xPos, double yPos)
{
	MyGLWindow * theWindow = static_cast<MyGLWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}
	// cam left right?
	theWindow->xChange = xPos - theWindow->lastX;
	//camera up/down, if I switch thing on right it will go in reverse
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("x:%.6f, y:%:6f\n", theWindow-> xChange, theWindow->yChange);
}



void MyGLWindow::InitKeys()
{
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
}
