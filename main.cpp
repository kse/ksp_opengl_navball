#define GLEW_STATIC

/* TODO:
 * - Make the single box 3D, so it looks nice.
 * - Maybe color shade the box depending on acceleration/tension of spring?
 * - Add several boxes with different spring parameters.
 * - Quarternions!
 */

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
	glfwWindowHint(GLFW_SAMPLES, 2);

	window = glfwCreateWindow(10, 10, "Spring Simulator", NULL, NULL);
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

	//std::cout << "Using monitor '"
	//	<< "'with characteristics\n\tWidth: " << monitorMode->width
	//	<< "\n\tHeight: " << monitorMode->height << "\n";

	// Center the window.
	//glfwSetWindowPos(window, monXOffset + monMode->width,
	//		monYOffset + monMode->height);

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "ERROR::GLEW::INIT: %s\n", glewGetErrorString(err));
		return NULL;
	}

	glEnable(GL_MULTISAMPLE);

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

	window = initializeOpenGL();
	if (window == NULL) {
		exit(EXIT_FAILURE);
	}

	Sphere *mass = new Sphere();
	Camera *camera = new Camera();

	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCursorPosCallback(window, cursorMovementCallback);
	glfwSetCursorEnterCallback(window, cursorEnterCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

	Telemachus *tm = new Telemachus();
	int pcount = 0;
	double heading, pitch, roll;
	
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		auto start = std::chrono::system_clock::now();

		pcount++;
		if (pcount == 5) {
			bool res = tm->getPitchHeadingRoll(&pitch, &heading, &roll);
			if (res) {
				pcount = 0;
				printf("Pitch: %f, Heading: %f, Roll: %f\n", pitch, heading, roll);
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
		glClearColor(0.1,0.1,0.1,0.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mass->Draw(view, 1.0f/targetFPS);

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

	delete mass;
	
	glfwTerminate();
	return EXIT_SUCCESS;
}
