#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class Camera {
	private:
		glm::vec3 viewDirection,
			upDirection,
			cameraPosition;

		double yaw,
			   pitch;

	public:
		Camera();

		/**
		 * Move the camera based on activation of keyboard keys.
		 */
		void move(bool keys[]);

		/**
		 * Return the matrix to transform coordinates into camera view
		 */
		glm::mat4 transformMatrix();

		void lookAround(double deltaX, double deltaY);
};
