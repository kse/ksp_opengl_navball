#version 330
layout (location = 0) in vec3 position;
out vec4 vertexColor;
out vec2 texcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
	vertexColor = vec4(0.0, 0.0, 0, 1.0);
	//vertexColor = vec4((atan(position.y, position.x)/3.1415926 + 1.0) * 0.5,  (asin(position.z) / 3.1415926 + 0.5), 0, 1.0);
	texcoord =  vec2(atan(-position.x, -position.z)/(2*3.14159265359) + 0.5,  (asin(-position.y) / 3.14159265359 + 0.5));
}
