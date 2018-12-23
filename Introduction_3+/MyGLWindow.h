#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class MyGLWindow
{
public:
	MyGLWindow();
	void InitKeys();
	~MyGLWindow();

	MyGLWindow(GLfloat windowWidth, GLfloat windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; };
	GLint getBufferHeight() { return bufferHeight; };

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	//camera stuff::SHOULD USE GETTERS AND SETTERS
	bool* getKeys() { return keys; }
	// oversimplifying, in game should use bool check
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }


private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	// camera input
	bool keys[1024];
	

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

