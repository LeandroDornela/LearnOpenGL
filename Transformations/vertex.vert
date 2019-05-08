#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 tex;

// Cor do presente vertice.
out vec4 vertexColor;
// Coordenada de textura do presente vertice.
out vec2 texCoord;

void main()
{
	gl_Position = vec4(pos, 1.0);
	vertexColor = vec4(col, 1.0);
	texCoord = tex;
}