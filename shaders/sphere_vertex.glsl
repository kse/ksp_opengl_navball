#version 330
layout (location = 0) in vec3 position;
out vec4 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
	//vertexColor = vec4(0.6, 0.6, 0, 1.0);
	vertexColor = vec4((atan(position.y, position.x)/3.1415926 + 1.0) * 0.5,  (asin(position.z) / 3.1415926 + 0.5), 0, 1.0);
}
