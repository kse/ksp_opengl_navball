#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>


#include "Sphere.hpp"
#include "Indicator.hpp"
#include "Camera.hpp"
#include "Telemachus.hpp"

const int targetFPS = 60;

/**
 * Used for storing activated keys, is global because we use function pointers.
 */
bool keys[1024];

/**
 * Used for storing mouse position information, function pointers.
 */
double mouseX = 0,
	mouseY= 0,
	previousMouseX = 0,
	previousMouseY = 0;

bool cursorFirstEnter = false;

void glfw_error_callback(int code, const char *description) {
	fputs(description, stderr);
}

void glfw_quit_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_Q) {
		exit(EXIT_SUCCESS);
	}
}

GLFWmonitor *GetActiveMonitor(int windowx, int windowy) {
	int count = 0, i, xpos, ypos;
	GLFWmonitor *monitor = NULL;
	GLFWmonitor **monitors = glfwGetMonitors(&count);

	for (i = 0, monitor = monitors[0]; i < count; i++, monitor = monitors[i]) {
		glfwGetMonitorPos(monitor, &xpos, &ypos);
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);

		if (xpos <= windowx &&
				ypos <= windowy &&
				windowx <= mode->width + xpos &&
				windowy <= mode->height + ypos) {
			return monitor;
		}
	}

	return NULL;
}

GLFWwindow *initializeOpenGL() {
	GLFWwindow *window;
	glfwSetErrorCallback(glfw_error_callback);


	if (!glfwInit()) {
		fprintf(stderr, "ERROR::GLFW::INITIALIZATION\n");
		return NULL;
	}

	// Get information about primary monitor, where we want to create the
	// window.
	GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();

	int monXOffset, monYOffset;
	glfwGetMonitorPos(primaryMonitor, &monXOffset, &monYOffset);

	// Specify window/context options.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 2);

	window = glfwCreateWindow(10, 10, "KSP Navball", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return NULL;
	}

	// Warning: Sleep value was found by trial and error, and if missing
	// returned window positions will be 0.
	usleep(1e5);
	int windowx, windowy;
	glfwGetWindowPos(window, &windowx, &windowy);

	GLFWmonitor *monitor = GetActiveMonitor(windowx, windowy);
	const GLFWvidmode *monitorMode = glfwGetVideoMode(monitor);

	glfwSetWindowShouldClose(window, GL_TRUE);

	glfwWindowHint(GLFW_RED_BITS, monitorMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, monitorMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, monitorMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, monitorMode->refreshRate);

	window = glfwCreateWindow(monitorMode->width, monitorMode->height, "KSP Navball", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "ERROR::GLEW::INIT: %s\n", glewGetErrorString(err));
		return NULL;
	}

	// Initializing GLEW creates an invalid enum error, ignore this error.
	// https://www.opengl.org/wiki/OpenGL_Loading_Library
	glGetError();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);  
	glEnable(GL_DEPTH_TEST);

	return window;
}

void keyPressCallback(GLFWwindow *window, int key, int scancode,
				int action, int mods) {
	if(action == GLFW_PRESS) {
		keys[key] = true;
	} else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void cursorMovementCallback(GLFWwindow *window, double xpos, double ypos) {
	mouseX = xpos;
	mouseY = ypos;
}

void cursorEnterCallback(GLFWwindow *window, int entered) {
	cursorFirstEnter = (entered == GL_TRUE) ? true : false;
}

int main(int argc, char *argv[]) {
	GLFWwindow *window;
	glm::mat4 view; // Identity matrix, is where our camera is based.
	glm::mat4 projection;
	glm::mat4 rotation(1.0f);
	int width, height;

	window = initializeOpenGL();
	if (window == NULL) {
		exit(EXIT_FAILURE);
	}

	glfwGetWindowSize(window, &width, &height);
	projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

	Sphere *sphere = new Sphere(projection);
	Indicator *indicator = new Indicator(projection);
	Camera *camera = new Camera();

	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCursorPosCallback(window, cursorMovementCallback);
	glfwSetCursorEnterCallback(window, cursorEnterCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

	Telemachus *tm = new Telemachus();
	int pcount = 0;
	double heading = 0, pitch = 0, roll = 0;

	while (!glfwWindowShouldClose(window)) {
		auto start = std::chrono::system_clock::now();

		pcount++;
		if (pcount == 3) {
			bool res = tm->getPitchHeadingRoll(&pitch, &heading, &roll);
			if (res) {
				pcount = 0;
				//printf("Pitch: %f, Heading: %f, Roll: %f\n", pitch, heading, roll);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// Update the camera transformation matrix.
		if (cursorFirstEnter) {
			cursorFirstEnter = false;
			previousMouseX = mouseX;
			previousMouseY = mouseY;
		}

		double deltaX = mouseX - previousMouseX;
		double deltaY = mouseY - previousMouseY;
		if (deltaX != 0.0 || deltaY != 0.0) {
			camera->lookAround(deltaX, deltaY);
			previousMouseX = mouseX;
			previousMouseY = mouseY;
		}

		camera->move(keys);
		view = camera->transformMatrix();


		// Set a nice gray BG color.
		glClearColor(0.05, 0.05, 0.05, 0.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sphere->Draw(view, 3.141593f*((float)heading)/180.0f,
				3.141593f*((float)pitch)/180.0f,
				3.141593f*((float)roll)/180.0f,
				1.0f/targetFPS);
		indicator->Draw(view);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds> 
			(std::chrono::system_clock::now() - start);

		// Try to keep a constant amount of FPS.
		int sleep_us = 1e6/targetFPS - duration.count();
		if (sleep_us > 0) {
			usleep(sleep_us);
		} else {
			printf("FPS playing catch up.\n");
		}
	}

	delete sphere;
	delete indicator;
	
	glfwTerminate();
	return EXIT_SUCCESS;
}
