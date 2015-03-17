#define GLFW_DLL 
#include "GLFW/glfw3.h"
#include "rply.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

static int vertex_cb(p_ply_argument argument) {
	long eol;
	ply_get_argument_user_data(argument, NULL, &eol);
	printf("%g", ply_get_argument_value(argument));
	if (eol) printf("\n");
	else printf(" ");
	return 1;
}

static int face_cb(p_ply_argument argument) {
	long length, value_index;
	ply_get_argument_property(argument, NULL, &length, &value_index);
	switch (value_index) {
	case 0:
	case 1:
		printf("%g ", ply_get_argument_value(argument));
		break;
	case 2:
		printf("%g\n", ply_get_argument_value(argument));
		break;
	default:
		break;
	}
	return 1;
}
//http://w3.impa.br/~diego/software/rply/
///< Only wrapping the glfw functions
namespace glfwFunc
{
	GLFWwindow* glfwWindow;
	const unsigned int WINDOW_WIDTH = 512;
	const unsigned int WINDOW_HEIGHT = 512;
	const float NCP = 0.01f;
	const float FCP = 52.f;
	const float fAngle = 45.f;
	string strNameWindow = "Hello GLFW";

	///< Init all data and variables.
	bool initialize()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		long nvertices, ntriangles;
		p_ply ply = ply_open("surfaceAB.ply", NULL, 0, NULL);
		if (!ply) return 1;
		if (!ply_read_header(ply)) return false;
		nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
		ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
		ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
		ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
		printf("%ld\n%ld\n", nvertices, ntriangles);
		if (!ply_read(ply)) return false;
		ply_close(ply);

    return true;
	}
	
	///< Callback function used by GLFW to capture some possible error.
	void errorCB(int error, const char* description)
	{
		cout << description << endl;
	}

	///
	/// The keyboard function call back
	/// @param window id of the window that received the event
	/// @param iKey the key pressed or released
	/// @param iScancode the system-specific scancode of the key.
	/// @param iAction can be GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	/// @param iMods Bit field describing which modifier keys were held down (Shift, Alt, & so on)
	///
	void keyboardCB(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMods)
	{
		if (iAction == GLFW_PRESS)
		{
			switch (iKey)
			{
				case GLFW_KEY_ESCAPE:
				case GLFW_KEY_Q:
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
			}
		}
	}
	
	///< A private function to replace the classic gluPerspective from OGL
	void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
	{
			const GLdouble pi = 3.1415926535897932384626433832795;
			GLdouble fW, fH;
			fH = tan( fovY / 360 * pi ) * zNear;	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
			fW = fH * aspect;
			glFrustum( -fW, fW, -fH, fH, zNear, zFar );
	}
	
	///< The resizing function
	void resizeCB(GLFWwindow* window, int iWidth, int iHeight)
	{
		float ratio = iWidth / float(iHeight);
		if(iHeight == 0) iHeight = 1;
		glViewport(0, 0, iWidth, iHeight);
		
		glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
		gluPerspective(fAngle, ratio, NCP, FCP);
    glMatrixMode(GL_MODELVIEW);
	}

	///< The main rendering function.
	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.15f, 0.15f, 0.15f, 1.f);
		glLoadIdentity();
		glTranslatef(0.f,0.f,-15.f);
		// glBegin(GL_TRIANGLES);
		// 	glColor3f(1.f, 0.f, 0.f);
		// 	glVertex3f(-0.6f, -0.4f, 0.f);
		// 	glColor3f(0.f, 1.f, 0.f);
		// 	glVertex3f(0.6f, -0.4f, 0.f);
		// 	glColor3f(0.f, 0.f, 1.f);
		// 	glVertex3f(0.f, 0.6f, 0.f);
		// glEnd();
		glfwSwapBuffers(glfwFunc::glfwWindow);
	}
	
	/// Here all data must be destroyed + glfwTerminate
	void destroy()
	{
		glfwDestroyWindow(glfwFunc::glfwWindow);
		glfwTerminate();
	}
};

int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfwFunc::errorCB);
	if (!glfwInit())	exit(EXIT_FAILURE);
	if(!glfwFunc::initialize()) exit(0);
	glfwFunc::glfwWindow = glfwCreateWindow(glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT, glfwFunc::strNameWindow.c_str(), NULL, NULL);
	if (!glfwFunc::glfwWindow)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

	glfwMakeContextCurrent(glfwFunc::glfwWindow);
	glfwFunc::resizeCB(glfwFunc::glfwWindow, glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT);	//just the 1st time
	glfwSetKeyCallback(glfwFunc::glfwWindow, glfwFunc::keyboardCB);
	glfwSetWindowSizeCallback(glfwFunc::glfwWindow, glfwFunc::resizeCB);
	// main loop!
	while (!glfwWindowShouldClose(glfwFunc::glfwWindow))
  {
		glfwFunc::draw();
    glfwPollEvents();	//or glfwWaitEvents()
  }
	glfwFunc::destroy();
	return EXIT_SUCCESS;
}