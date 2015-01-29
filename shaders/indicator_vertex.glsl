#version 330
in vec2 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y, 0.0f, 1.0f);
}
