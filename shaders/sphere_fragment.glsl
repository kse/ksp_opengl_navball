#version 330
//in vec4 vertexColor;
in vec3 coordinates;
out vec4 outColor;

uniform sampler2D textureData;

void main() {
	vec2 mapped = vec2(atan(-coordinates.x, -coordinates.z)/(2*3.14159265359) + 0.5,  (asin(-coordinates.y) / 3.14159265359 + 0.5));
	outColor = texture(textureData, mapped);
}
