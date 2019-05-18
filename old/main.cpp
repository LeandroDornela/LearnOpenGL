#include <glad\glad.h>
#include "Shader.h"
#include <GLFW\glfw3.h>
#include <iostream>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>


void Log(const char* log)
{
	std::cout << log << std::endl;
};


struct Texture
{
	unsigned int id;
	int width;
	int height;
	int nrChannels;

	Texture(const char* texPath)
	{
		// Flipar a textura ao carregar.
		stbi_set_flip_vertically_on_load(true);

		unsigned char *data = stbi_load(texPath, &width, &height, &nrChannels, 0);

		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);

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
	}

	void Bind(unsigned int location)
	{
		glActiveTexture(location);
		glBindTexture(GL_TEXTURE_2D, id);
	}
};

struct VertexBufferObj
{
	unsigned int id;

	void Bind()
	{
		// Atribui o vbo ao buffer referente do openGL, nesse caso o para vertices é
		// o GL_ARRAY_BUFFER.
		glBindBuffer(GL_ARRAY_BUFFER, id);
	};

	VertexBufferObj() {};

	VertexBufferObj(const float *vertices)
	{
		// Cria 1 buffer de vertices e salva o id para acesso na variavel vbo.
		glGenBuffers(1, &id);
		
		Bind();
			// Preenche o buffer com os dados, nesse caso com nossos vertices.
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	};
};


struct ElementBufferObj
{
	unsigned int id;

	void Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	};

	ElementBufferObj() {};

	ElementBufferObj(const unsigned int *indices)
	{
		glGenBuffers(1, &id);
		
		Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	};
};


struct VertexArrayObj
{
	unsigned int id;
	unsigned int indicesCount;
	VertexBufferObj vbo;
	ElementBufferObj ebo;


	void Bind()
	{
		// Ativa o VAO para atribuir oque for definido para o vbo e attrib pointer.
		// Por isso deve ser chamado antes.
		glBindVertexArray(id);
	};

	void Unbind()
	{
		glBindVertexArray(0);
	};

	VertexArrayObj() {};

	VertexArrayObj(const float *vertices, const unsigned int *indices)
	{
		// Cria 1 vertex array object vazio.
		glGenVertexArrays(1, &id);
		
		Bind();
			vbo = VertexBufferObj(vertices);
			ebo = ElementBufferObj(indices);
	};


	void AtributePointerXYZ()
	{
		Bind();
			// XYZ
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
	};

	void AtributePointerXYZRGB()
	{
		Bind();
			// XYZ
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// RGB
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
	};

	void AtributePointerXYZRGBUV()
	{
		Bind();
			// Especifica como o OpenGL deve ler os dados do vertice. A forma de interpretar
			// os atributos do vertice. Por ex. se os dados são coordenadas de 2 dimensoes
			// ou coordenadas de 3 dimensoes mais uma cor com 3 valores.
			// Neste caso temos como dados apenas a posição do vertice com 3 valores.
			// Entenda como configurações dos atributos de vertices.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// Fazendo o mesmo para os atributos de cor.
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// E para as coordenadas de textura.
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
	};


	void Draw()
	{
		Bind();	
		// Desenha oque esta no vao usando os indices do ebo.
			glPointSize(10);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		Unbind();
	};


	void Delete()
	{
		glDeleteVertexArrays(1, &id);
		glDeleteBuffers(1, &vbo.id);
		glDeleteBuffers(1, &ebo.id);
	}
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


GLFWwindow * CreateViewportWindow(int width, int heigth)
{
	glfwInit();
	// Versão e quais recursos do OpenGL serão utilizados.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Cria um objeto janela da biblioeca GLFW.
	GLFWwindow* window = glfwCreateWindow(width, heigth, "LearnOpenGL - Shaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	// Seta a função que irá mudar o tamanho da viewport quando o usuario mudar o tamanho da janela.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	// Cria a viewpor do openGL com o tamanho da janela (obs: pode ser menor).
	glViewport(0, 0, width, heigth);

	return window;
}


int main()
{
	GLFWwindow* window = CreateViewportWindow(400, 400);
	
	// Vertices dos triangulos
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.8f,  0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.8f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.8f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
	};
	// Indices para a ordem de desenho dos vertices.
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	// Vertex array object
	VertexArrayObj vao = VertexArrayObj(vertices, indices);
	vao.AtributePointerXYZRGBUV();

	// Shader
	Shader shader = Shader("vertex.vert", "fragment.frag");

	// Texturas
	Texture container = Texture("container.jpg");
	Texture face = Texture("awesomeface.jpg");
	
	// Cria uma matriz idenidade.
	glm::mat4 transfMatrix = glm::mat4(1.0f);

	shader.Use();
		// Associando as texturas
		container.Bind(GL_TEXTURE0);
		face.Bind(GL_TEXTURE1);
		shader.SetInt("texture0", 0);
		shader.SetInt("texture1", 1);

		unsigned int transformLocation = glGetUniformLocation(shader.id, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transfMatrix));

		// Setando uniform para fazer movimento na textura
		unsigned int passLocation = glGetUniformLocation(shader.id, "pass");
		glUniform1f(passLocation, 0.0f);




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
			glm::mat4 transfMatrix = glm::mat4(1.0f);
			//transfMatrix = glm::translate(transfMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
			//transfMatrix = glm::rotate(transfMatrix, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
			//transfMatrix = glm::scale(transfMatrix, glm::vec3(0.5, 0.5, 0.5));
			// glm::value_ptr é para deixa a matriz no formato q o OpenLG "gosta".
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transfMatrix));

			// Atualizando a uniform para mover a textura(no caso mover a posição do vertice em relação a textura).
			glUniform1f(passLocation, (float)glfwGetTime());
		
			// Determina qual vao usar.
			vao.Draw();

		// Troca (swap) o buffer com as cores de cada pixel exibindo na tela o buffer
		// onde acabamos de gravar esses pixels.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vao.Delete();
	glfwTerminate();

	return 0;
}

