#include "Sphere.hpp"

#include <iostream>
#include <stdio.h>

#include "lodepng.h"

bool loadpng(const std::string file, std::vector<unsigned char>& textureData,
		unsigned& textureWidth, unsigned& textureHeight) {

	unsigned error = lodepng::decode(textureData, textureWidth, textureHeight, file);

	if (error) {
		fprintf(stderr, "Lodepng decoder error: %s\n", lodepng_error_text(error));
		return false;
	}

	return true;
}

void Sphere::Draw(glm::mat4 view, float heading, float pitch, float roll, float deltas) {
	glm::mat4 model;

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);

	model = glm::rotate(model, roll,    glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, pitch,   glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, heading, glm::vec3(0.0f, -1.0f, 0.0f));

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDrawElements(GL_TRIANGLES, modelloader->indices.size(), GL_UNSIGNED_INT, 0);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	GLenum e = glGetError();
	switch (e) {
		case GL_INVALID_ENUM:
			fprintf(stderr, "Invalid in sphere rendering\n");
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
	glBindTexture(GL_TEXTURE_2D, 0);
}

Sphere::Sphere(glm::mat4 projection) {
	modelloader = new ModelLoader("sphere.dae");
	this->projection = projection;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//printf("Vertices: %lu, Indices: %lu\n", modelloader->vertices.size(), modelloader->indices.size());

	// Create vertex buffer object (VBO) //
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*modelloader->vertices.size(), modelloader->vertices.data(), GL_STATIC_DRAW);

	loadpng("navball_brownblue3.png", textureData, textureWidth, textureHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum e = glGetError();
	switch (e) {
		case GL_INVALID_ENUM:
			fprintf(stderr, "Invalid enum after texture\n");
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

	// Create EBO (element buffer object) //
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * modelloader->indices.size(), modelloader->indices.data(), GL_STATIC_DRAW);

	e = glGetError();
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
