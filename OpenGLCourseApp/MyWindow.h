#ifndef _MY_WINDOW_H
#define _MY_WINDOW_H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class MyWindow
{
public:
	MyWindow();
	MyWindow(GLint windowWidth, GLint windowHeight);
	
	int Initialise(); 
	GLfloat getBufferWidth()  { return static_cast<GLfloat>(bufferWidth); }   // add const qualifier if OpenGl is OK
	GLfloat getBufferHeight() { return  static_cast<GLfloat>(bufferHeight); }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers()	  { return glfwSwapBuffers(mainWindow); }

	~MyWindow();
private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

#endif // !_MY_WINDOW_H
