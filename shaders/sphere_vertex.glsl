#version 330
layout (location = 0) in vec3 position;
//out vec2 texcoord;
out vec3 coordinates;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
	//texcoord =  vec2(atan(-position.x, -position.z)/(2*3.14159265359) + 0.5,  (asin(-position.y) / 3.14159265359 + 0.5));
	coordinates = vec3(position.x, position.y, position.z);
}
