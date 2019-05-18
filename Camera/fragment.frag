#version 330 core

out vec4 FragColor;

// Cor do vertice interpolada nos fragmentos.
in vec4 vertexColor;
// Coordenada da textura interpolada nos fragmentos.
in vec2 texCoord;

// Referencia a textura salva fora do sahder de onde
// irá se obter a cor do texel na coordenada interpolada.
// Se nenhuma textura for setada diretamente pelo usuario
// o driver fará isso automaticamente escolhendo a textura
// identificada no local 0, locais são determinados
// utilizando glActieTexture().
uniform sampler2D texture0;
uniform sampler2D texture1;


void main()
{
	//vec4 newCol = texture(texture1, texCoord);
	vec4 newCol = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2)*vertexColor;
	//FragColor = newCol;
	FragColor = vertexColor;
};
