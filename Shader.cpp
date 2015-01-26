#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <errno.h>
#include <strings.h>

Shader::Shader() {
}

Shader::~Shader() {
	glDeleteProgram(program);

	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		glDeleteShader(*it);
	}
}

GLuint Shader::getProgram() {
	GLint success;
	GLchar infoLog[512];

	if (program != 0) {
		return program;
	}

	program = glCreateProgram();

	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		glAttachShader(program, *it);
	}

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n\t" << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	return program;
}

int Shader::add(const char *file, GLenum type) {
	std::ifstream fh(file);
	std::string   source;
	GLint success;
	GLchar infoLog[512];

	if (!fh.is_open()) {
		std::cout << "Unable to open shader '" << file << "': " << strerror(errno) << std::endl;
		return GL_FALSE;
	}

	while (fh.peek() != EOF) {
		source.push_back(fh.get());
	}

	fh.close();

	GLuint shader = glCreateShader(type);
	const char *csrc = source.c_str();
	glShaderSource(shader, 1, &csrc, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return GL_FALSE;
	}

	shaders.push_back(shader);

	return GL_TRUE;
}
