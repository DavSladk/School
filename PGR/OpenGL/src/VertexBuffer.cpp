#include <VertexBuffer.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID); // generating buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // select buffer as array buffer
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); // where to get data
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
