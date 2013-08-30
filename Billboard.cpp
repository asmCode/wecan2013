#include "Billboard.h"

#include <GL/glew.h>
#include <gl/gl.h>
#include <assert.h>

uint32_t Billboard::m_vboId = 0;

float Billboard::m_vertices[8] = 
{
	//  1 0
	//  0 0
	-0.5f, 0.5f,
	
	//  0 0
	//  1 0
	-0.5f, -0.5f,

	//  0 1
	//  0 0
	0.5f, 0.5f,

	//  0 0
	//  0 1
	0.5f, -0.5f,
};


bool Billboard::Initialize()
{
	assert(m_vboId == 0);

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void Billboard::Release()
{
	if (m_vboId != 0)
	{
		glDeleteBuffers(1, &m_vboId);
		m_vboId = 0;
	}
}

void Billboard::Setup()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

void Billboard::Clean()
{
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Billboard::Draw()
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
