#version 330
//in vec4 vertexColor;
in vec3 coordinates;
in vec3 norm;
out vec4 outColor;

uniform sampler2D textureData;
uniform vec3 lightDir = normalize(vec3(3.0f, 3.0f, 3.0f));
uniform vec4 ambient = vec4(0.4f, 0.4f, 0.4f, 1.0f);

void main() {
	vec2 mapped = vec2(atan(-coordinates.x, -coordinates.z)/(2*3.14159265359) +
		0.5,  (asin(-coordinates.y) / 3.14159265359 + 0.5));
	float fDot = max(0.0f, dot(norm, lightDir));
	outColor =(texture(textureData, mapped) * (ambient + fDot));
}
