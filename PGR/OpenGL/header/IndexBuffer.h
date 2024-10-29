#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	GLuint m_RendererID;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int m_Count;

	inline unsigned int GetCount() const { return m_Count; }
};