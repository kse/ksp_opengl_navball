#version 330
layout (location = 0) in vec3 position;
out vec3 coordinates;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
	coordinates = vec3(position.x, position.y, position.z);
}
