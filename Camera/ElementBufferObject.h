#include <glad\glad.h>

class ElementBufferObject
{
private:
	unsigned int id = 0;

public:
	unsigned int indicesCount = 0;

	ElementBufferObject(const unsigned int *ind);
	void Bind();
	~ElementBufferObject();
};


ElementBufferObject::ElementBufferObject(const unsigned int *ind)
{
	// Indices para a ordem de desenho dos vertices.
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	indicesCount = 36;

	glGenBuffers(1, &id);

	Bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
};


void ElementBufferObject::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
};


ElementBufferObject::~ElementBufferObject()
{

}