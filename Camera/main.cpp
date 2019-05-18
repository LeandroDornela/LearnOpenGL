#include <glad\glad.h>
#include "Shader.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "VertexArrayObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

#define WIDTH 800
#define HEIGTH 600


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = WIDTH/2, lastY = HEIGTH/2;
float yaw = 0;
float pitch = 0;
bool firstMouse = true;

// Matrizes de transformação
glm::mat4 transfMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::mat4(1.0f);

void Log(const char* log)
{
	std::cout << log << std::endl;
};


/*
	void AtributePointerXYZUV()
	{
		Bind();
		// Entenda como configurações dos atributos de vertices.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// E para as coordenadas de textura.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void AtributePointerXYZRGBUV()
	{
		
	};
	void Delete()
	{
		glDeleteVertexArrays(1, &id);
		glDeleteBuffers(1, &vbo.id);
		glDeleteBuffers(1, &ebo.id);
	};*/



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


	// Escolho um local para a textura e a ativo.
	void Bind(unsigned int location)
	{
		glActiveTexture(location);
		glBindTexture(GL_TEXTURE_2D, id);
	}
};


struct Camera {
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 *view;
	glm::mat4 *projection;
	float fov = 0;


	Camera(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix, float _fov)
	{
		view = viewMatrix;
		projection = projectionMatrix;

		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		*view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		*projection = glm::perspective(glm::radians(_fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
		fov = _fov;
	}

	void Update(GLFWwindow *window)
	{
		float cameraSpeed = 0.05f*deltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		*view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		*projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
	}
};

// Camera
Camera camera = Camera(&viewMatrix, &projectionMatrix, 45.0f);


/*
Modifica o tamanho da viewport quando o tamnho da janela é modificado.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


/*
Cria uma janela e uma viewport da tamanho desta.
*/
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

	// Inicializa o GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	// Cria a viewpor do openGL com o tamanho da janela (obs: pode ser menor).
	glViewport(0, 0, width, heigth);

	return window;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camera.cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (camera.fov >= 1.0f && camera.fov <= 45.0f)
		camera.fov -= yoffset;
	if (camera.fov <= 1.0f)
		camera.fov = 1.0f;
	if (camera.fov >= 45.0f)
		camera.fov = 45.0f;
}


int main()
{
	GLFWwindow *window = CreateViewportWindow(WIDTH, HEIGTH);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	float* vertices = NULL;
	unsigned int* indices = NULL;
	glm::vec3 cubePositions[] = {
								  glm::vec3(0.0f,  0.0f,  0.0f),
								  glm::vec3(2.0f,  5.0f, -15.0f),
								  glm::vec3(-1.5f, -2.2f, -2.5f),
								  glm::vec3(-3.8f, -2.0f, -12.3f),
								  glm::vec3(2.4f, -0.4f, -3.5f),
								  glm::vec3(-1.7f,  3.0f, -7.5f),
								  glm::vec3(1.3f, -2.0f, -2.5f),
								  glm::vec3(1.5f,  2.0f, -2.5f),
								  glm::vec3(1.5f,  0.2f, -1.5f),
								  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Vertex Buffer Object
	int dataFormat[] = { 2,3,2 };

	VertexArrayObject vao = VertexArrayObject(vertices, indices, dataFormat);

	// shader
	Shader shader = Shader("vertex.vert", "fragment.frag");

	// Texturas
	Texture container = Texture("container.jpg");
	Texture face = Texture("awesomeface.jpg");


	// Render loop.
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		deltaTime = deltaTime * 100;
		lastFrame = currentFrame;

		// Limpa a tela.
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		camera.Update(window);

		shader.Use();
			
			// Update das matrizes da camera.
			shader.SetMat4f("viewMat", viewMatrix);
			shader.SetMat4f("projectionMat", projectionMatrix);

			// Update texturas ativas.
			container.Bind(GL_TEXTURE0);
			face.Bind(GL_TEXTURE1);
			shader.SetInt("texture0", 0);
			shader.SetInt("texture1", 1);
			

			for (int i = 0; i < 10; i++)
			{
				modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
				float angle = 20.0f * i;
				modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(i + 0.5f, 0.3f, i - 0.3f));
				
				shader.SetMat4f("transform", transfMatrix);
				shader.SetMat4f("modelMat", modelMatrix);
				
				vao.Draw();
			}

		// Troca (swap) o buffer com as cores de cada pixel exibindo na tela o buffer
		// onde acabamos de gravar esses pixels.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

