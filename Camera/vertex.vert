#version 330 core

layout(location = 0) in vec3 pos;
//layout(location = 1) in vec3 col;
layout(location = 1) in vec2 tex;

// Cor do presente vertice.
out vec4 vertexColor;
// Coordenada de textura do presente vertice.
out vec2 texCoord;
// Matriz de tranformação
uniform mat4 transform;

uniform float pass;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;


void main()
{
	gl_Position = projectionMat*viewMat*modelMat*transform * vec4(pos, 1.0f);
	vertexColor = vec4(pos, 1.0f)+vec4(0.5f,0.5f,0.5f,1.0f);
	texCoord = vec2(tex.x, tex.y);
}
