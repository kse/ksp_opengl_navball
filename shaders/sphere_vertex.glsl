#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 coordinates;
out vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y,
		position.z, 1.0f);
	norm = mat3(model) * normal;
	coordinates = vec3(position.x, position.y, position.z);
}
