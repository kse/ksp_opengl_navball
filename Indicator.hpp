#include "Shader.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Indicator {
	public:
		Indicator(glm::mat4 projection);
		void Draw(glm::mat4 view);

		GLuint
			VAO,
			VBO,
			EBO,
			shaderProgram,
			modelIndex,
			viewIndex,
			projectionIndex;

	private:
		glm::mat4 projection;
};
