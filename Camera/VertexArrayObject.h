#include <glad\glad.h>
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include <iostream>


class VertexArrayObject
{
private:
	unsigned int id = 0;
	VertexBufferObject vbo = NULL;
	ElementBufferObject ebo = NULL;

public:
	VertexArrayObject();
	VertexArrayObject(const float *vertices, const unsigned int *indices, int *format);
	void Bind();
	void Draw();
	void AttribPointers(int *format);
	~VertexArrayObject();
};


VertexArrayObject::VertexArrayObject(const float *vertices, const unsigned int *indices, int *format)
{
	glGenVertexArrays(1, &id);

	Bind();
		vbo = VertexBufferObject(vertices);
		ebo = ElementBufferObject(indices);
		AttribPointers(format);
};


void VertexArrayObject::Bind()
{
	// Ativa o VAO para atribuir oque for definido para o vbo e attrib pointer.
	// Por isso deve ser chamado antes.
	glBindVertexArray(id);
};


void VertexArrayObject::Draw()
{
	Bind();
		// Desenha oque esta no vao usando os indices do ebo.
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, ebo.indicesCount);
	glBindVertexArray(0);
};

// 0 -> number of pointers
// 1 -> elements in fist pointer
void VertexArrayObject::AttribPointers(int *format)
{
	
	int totalElements = 0;

	for (int i = 1; i <= format[0]; i++)
	{
		totalElements += format[i];
	}

	Bind();

	for (int i = 1; i <= format[0]; i++)
	{
		if (i == 1)
		{
			glVertexAttribPointer(i - 1, format[i], GL_FLOAT, GL_FALSE, totalElements * sizeof(float), (void*)0);
		}

		glVertexAttribPointer(i-1, format[i], GL_FLOAT, GL_FALSE, totalElements * sizeof(float), (void*)(format[i-1] * sizeof(float)));
		glEnableVertexAttribArray(i-1);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// E para as coordenadas de textura.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
};


VertexArrayObject::~VertexArrayObject()
{

};
