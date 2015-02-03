#ifndef SPHERE
#define SPHERE
#define GLEW_STATIC

#include "Shader.hpp"
#include "ModelLoader.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere {
	private:
		std::vector<unsigned char> textureData;
		unsigned textureWidth, textureHeight;

		glm::mat4 projection;

		GLint viewLoc,
			  projLoc,
			  modelLoc;

		GLuint VAO,
			   VBO[2],
			   EBO,
			   texture,
			   shaderProgram;

		Shader *shader;

		ModelLoader *modelloader;

	public:
		Sphere(glm::mat4 projection);
		~Sphere();

		void Draw(glm::mat4 camera, float heading, float pitch, float roll, float deltas);
};

#endif
