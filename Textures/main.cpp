#include <glad\glad.h>
#include "Shader.h"
#include <GLFW\glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	// ======================== INICIALIZANDO AS BIBLIOTECAS ===============================

	// ============================= GLFW ==============================
	glfwInit();
	// Versão e quais recursos do OpenGL serão utilizados.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Cria um objeto janela da biblioeca GLFW.
	GLFWwindow* window = glfwCreateWindow(400, 400, "LearnOpenGL - Shaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Seta a função que irá mudar o tamanho da viewport quando o usuario mudar o tamanho da janela.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	// =============================== GLAD=============================
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// Cria a viewpor do openGL com o tamanho da janela (obs: pode ser menor).
	glViewport(0, 0, 400, 400);


	// ============================ CRIANDO O VAO ============================================
	unsigned int vao;
	// Cria 1 vertex array object vazio.
	glGenVertexArrays(1, &vao);
	// Ativa o VAO para atribuir oque for definido para o vbo e attrib pointer.
	// Por isso deve ser chamado antes.
	glBindVertexArray(vao);

	// ======================= CRIANDO 1 TRIANGULO EM UM BUFFER (VBO) ===========================
	// Vertices dos triangulos
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.8f,  0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.8f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.8f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
	};
	// id do vertex buffer object.
	unsigned int vbo;
	// Cria 1 buffer de vertices e salva o id para acesso na variavel vbo.
	glGenBuffers(1, &vbo);
	// Atribui o vbo ao buffer referente do openGL, nesse caso o para vertices é
	// o GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Preenche o buffer com os dados, nesse caso com nossos vertices.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ================================ CRIANDO O EBO ================================
	// Indices para a ordem de desenho dos vertices.
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// ==================== INSTRUÇÃO PARA INTERPRETAR O BUFFER DE VERTICES ======================
	// Especifica como o OpenGL deve ler os dados do vertice. A forma de interpretar
	// os atributos do vertice. Por ex. se os dados são coordenadas de 2 dimensoes
	// ou coordenadas de 3 dimensoes mais uma cor com 3 valores.
	// Neste caso temos como dados apenas a posição do vertice com 3 valores.
	// Entenda como configurações dos atributos de vertices.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Fazendo o mesmo para os atributos de cor.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// E para as coordenadas de textura.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	Shader shader = Shader("vertex.vert", "fragment.frag");


	// ===================== Carregando uma textura =============================
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// Cria o objeto textura sem dados.
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Preenche o objeto textura e cria mipmaps.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Ajusta os parametros da imagem.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Libera a imagem original carregada da memoria.
	stbi_image_free(data);

	// Para a textura 2
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data2 = stbi_load("awesomeface.jpg", &width2, &height2, &nrChannels2, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Preenche o objeto textura e cria mipmaps.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Ajusta os parametros da imagem.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Libera a imagem original carregada da memoria.
	stbi_image_free(data2);


	// ======================= Render loop =============================

	while (!glfwWindowShouldClose(window))
	{
		// Limpa a tela.
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Desenha os vertices do vbo diretamente ou indiretamente do vbo atribuido ao vao atual.
		// Usando tamben o shader program atual.
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		shader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shader.id, "texture1"), 1);

		// Determina qual vao usar.
		glBindVertexArray(vao);
		// Desenha oque esta no vao usando os indices do ebo.
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Desvincula o vao.
		glBindVertexArray(0);

		// Troca (swap) o buffer com as cores de cada pixel exibindo na tela o buffer
		// onde acabamos de gravar esses pixels.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	glfwTerminate();

	return 0;
}

