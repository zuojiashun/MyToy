#include "MeshBuffer.h"

CMeshBuffer::CMeshBuffer() : m_vaoHandle(0), m_vboColorHandle(0), 
	m_vboNormalHandle(0), m_vboUVHandle(0), m_vboVertexHandle(0)
{
}

void CMeshBuffer::MakeBuffer(const Vector3* vertices, const Color* colors, const Vector3* normals, const Vector2* uvs, int size)
{
	MakeVertexBuffer(vertices, size);
	MakeColorBuffer(colors, size);
	MakeNormalBuffer(normals, size);
	MakeUVBuffer(uvs, size);
}

void CMeshBuffer::MakeVertexBuffer(const Vector3* vertices, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!vertices) return;
	m_vertexNum = size;
	if (!m_vboVertexHandle) glGenBuffers(1, &m_vboVertexHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertexHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * size, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTEX_POS);
	glVertexAttribPointer(VERTEX_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CMeshBuffer::MakeColorBuffer(const Color* colors, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!colors) return;
	if (!m_vboColorHandle) glGenBuffers(1, &m_vboColorHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboColorHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * size, colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(COLOR_POS);
	glVertexAttribPointer(COLOR_POS, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CMeshBuffer::MakeUVBuffer(const Vector2* uvs, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!uvs) return;
	if (!m_vboUVHandle) glGenBuffers(1, &m_vboUVHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboUVHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2) * size, uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(UV_POS);
	glVertexAttribPointer(UV_POS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CMeshBuffer::MakeNormalBuffer(const Vector3* normals, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!normals) return;
	if (!m_vboNormalHandle) glGenBuffers(1, &m_vboNormalHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormalHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * size, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_POS);
	glVertexAttribPointer(NORMAL_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CMeshBuffer::BindBuffer()
{
	glBindVertexArray(m_vaoHandle);
}

void CMeshBuffer::ReleaseBuffer()
{
	if (m_vboColorHandle) glDeleteBuffers(1, &m_vboColorHandle);
	if (m_vboUVHandle) glDeleteBuffers(1, &m_vboUVHandle);
	if (m_vboNormalHandle) glDeleteBuffers(1, &m_vboNormalHandle);
	if (m_vboVertexHandle) glDeleteBuffers(1, &m_vboVertexHandle);
	if (m_vaoHandle) glDeleteVertexArrays(1, &m_vaoHandle);
}

int CMeshBuffer::GetVertexNum() const
{
	return m_vertexNum;
}