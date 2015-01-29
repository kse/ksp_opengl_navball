#include "Indicator.hpp"

#include <stdio.h>

GLfloat a = 0.008,
		b = 0.003,
		c = 0.2,
		d = 0.1;


GLfloat vertices[][2] = {
	{0.0,    0.0},
	{a,   b}, // 1
	{b,   a}, // 2

	{-b,  a}, // 3
	{-a,  b}, // 4

	{-a, -b}, // 5
	{-b, -a}, // 6
	{b,  -a}, // 7
	{a,  -b}, // 8

	{c,   b}, // 9
	{c,  -b}, // 10
	{d,   b}, // 11
	{d,  -b}, // 12

	{-c,  b}, // 13
	{-c, -b}, // 14
	{-d,  b}, // 15
	{-d, -b}, // 16

	{a,  -d}, // 17
	{a,  -d -2*b}, // 18

	{-a, -d}, // 19
	{-a, -d -2*b}, // 20
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 5,
	0, 5, 6,
	0, 6, 7,
	0, 7, 8,
	0, 8, 1,

	9, 10, 11,
	12, 11, 10,

	13, 14, 15,
	14, 15, 16,

	11, 12, 17,
	12, 17, 18,

	15, 16, 19,
	16, 19, 20,

	17, 18, 19,
	18, 19, 20
};

void Indicator::Draw(glm::mat4 view) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.001f));

	glUniformMatrix4fv(modelIndex, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewIndex, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionIndex, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
	GLenum e = glGetError();
	switch (e) {
		case GL_INVALID_ENUM:
			fprintf(stderr, "Invalid enum\n");
			break;
		case GL_INVALID_VALUE:
			fprintf(stderr, "Invalid value\n");
			break;
		case GL_INVALID_OPERATION:
			fprintf(stderr, "Invalid operation\n");
			break;
		default:
			break;
	}

	glBindVertexArray(0);
}

Indicator::Indicator(glm::mat4 projection) {
	this->projection = projection;

	// Generate and bind vertex array object.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader *shader = new Shader();	

	shader->add("shaders/indicator_vertex.glsl", GL_VERTEX_SHADER);
	shader->add("shaders/indicator_fragment.glsl", GL_FRAGMENT_SHADER);

	shaderProgram = shader->getProgram();
	glBindFragDataLocation(shaderProgram, 0, "color");

	GLuint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	projectionIndex = glGetUniformLocation(shaderProgram, "projection");
	viewIndex       = glGetUniformLocation(shaderProgram, "view");
	modelIndex      = glGetUniformLocation(shaderProgram, "model");

	glBindVertexArray(0);
};
