#version 330
in vec4 vertexColor;
in vec2 texcoord;
out vec4 outColor;

uniform sampler2D textureData;

void main() {
	outColor = texture(textureData, texcoord);
}
