#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	// Codigo fonte do vertex shader.
	const char *vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0);\n"
		"vertexColor = vec4(0.5, 1.0, 0.0, 1.0);\n"
		"}\0";

	// Codigo fonte do fragment shader.
	const char *fragmentShaderSource = 
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"in vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = ourColor;\n"
		"}\n\0";


	// ======================== INICIALIZANDO AS BIBLIOTECAS ===============================

	// ============================= GLFW ==============================
	glfwInit();
	// Versão e quais recursos do OpenGL serão utilizados.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Cria um objeto janela da biblioeca GLFW.
	GLFWwindow* window = glfwCreateWindow(400, 400, "LearnOpenGL", NULL, NULL);
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
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
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
		0, 1, 3,
		1, 2, 3
	};
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// ==================== INSTRUÇÃO PARA INTERPRETAR O BUFFER DE VERTICES ======================
	// So temos 1 atributo logo id é 0.
	unsigned int id = 0;
	// Especifica como o OpenGL deve ler os dados do vertice. A forma de interpretar
	// os atributos do vertice. Por ex. se os dados são coordenadas de 2 dimensoes
	// ou coordenadas de 3 dimensoes mais uma cor com 3 valores.
	// Neste caso temos como dados apenas a posição do vertice com 3 valores.
	// Entenda como configurações dos atributos de vertices.
	glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(id);



	// ==================== CRIANDO O VERTEX SHADER ===========================
	// Id de referencia para o objeto de OpenGL que será criado.
	unsigned int vertexShader;
	// Cria um objeto shader vazio recebendo como parametro o tipo de shader
	// e retorna o id do objeto.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Atribui o codigo fonte do shader ao objeto shader.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compila o shader.
	glCompileShader(vertexShader);
	// Verifica se o shader compilou, se não retorna uma mensagem de erro.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// ======================CRIANDO O FRAGMENT SHADER ==========================
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// ==================== CRIANDO O SHADER PROGRAM ==============================
	unsigned int shaderProgram;
	// Cria um objeto program vazio.
	shaderProgram = glCreateProgram();
	// Atribui os shaders ao programa.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Liga os shader atribuidos verificando os inputs e outputs.
	glLinkProgram(shaderProgram);
	// Verifica se o programa foi criado com sucesso.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// =================== Finalizando a crianção do program ========================
	// Os objetos shader criados não são mais nescessarios pois agora temos eles dentro do program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// ======================= Render loop =============================

	while (!glfwWindowShouldClose(window))
	{
		// Limpa a tela.
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Desenha os vertices do vbo diretamente ou indiretamente do vbo atribuido ao vao atual.
		// Usando tamben o shader program atual.
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Enviando uma cor para o fragment shader atravez de uma uniform.
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


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

