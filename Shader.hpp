#ifndef SHADER
#define SHADER

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Shader {
	public:
		Shader();
		~Shader();
		
		/**
		 * Adds a shader file, the type of which is specified by type.
		 */
		int add(const char *file, GLenum type);

		GLuint getProgram();
	private:
		GLuint program = 0;
		std::vector<GLuint> shaders;
};

#endif
