#include "Sphere.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <stdio.h>

GLfloat X = 0.525731112119133606f;
GLfloat Z = 0.850650808352039932f;

GLfloat vertices[] = {
	-X, 0.0f, Z,
	X, 0.0f, Z,
	-X, 0.0f, -Z,
	X, 0.0f, -Z,
	0.0f, Z, X,
	0.0f, Z, -X,
	0.0f, -Z, X,
	0.0f, -Z, -X,
	Z, X, 0.0f,
	-Z, X, 0.0f,
	Z, -X, 0.0f,
	-Z, -X, 0.0f
};

GLuint indices[] = {
	0,4,1,
	0,9,4,
	9,5,4,
	4,5,8,
	4,8,1,
	8,10,1,
	8,3,10,
	5,3,8,
	5,2,3,
	2,7,3,
	7,10,3,
	7,6,10,
	7,11,6,
	11,0,6,
	0,1,6,
	6,1,10,
	9,0,11,
	9,11,2,
	9,2,5,
	7,2,11 

};

void Sphere::Draw(glm::mat4 view, float deltas) {
	//printf("Velocity: %f\n", this->v);

	glm::mat4 model;

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_LINE_STRIP, 60, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Sphere::Sphere() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create vertex buffer object (VBO) //
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create EBO (element buffer object) //
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shader = new Shader();

	// Compile Vertex Shader //
	shader->add("shaders/sphere_vertex.glsl", GL_VERTEX_SHADER);

	// Fragment Vertex Shader //
	shader->add("shaders/sphere_fragment.glsl", GL_FRAGMENT_SHADER);

	// Create shader program //
	shaderProgram = shader->getProgram();
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	// Get location/offset of attribute 'position'.
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "projection");
	modelLoc = glGetUniformLocation(shaderProgram, "model");

	glBindVertexArray(0);
}

Sphere::~Sphere() {
	delete shader;
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
