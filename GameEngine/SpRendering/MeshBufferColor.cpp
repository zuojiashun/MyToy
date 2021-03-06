#include "MeshBufferColor.h"

USING_NAMESPACE_ENGINE;

MeshBufferColor::MeshBufferColor() : MeshBuffer(), m_VboColorHandle(0)
{
}

MeshBufferColor::~MeshBufferColor() 
{
	DeleteBufer(&m_VboColorHandle);
}

MeshBufferColor::MeshBufferColor(MeshPtr mesh) : MeshBuffer(mesh), m_VboColorHandle(0)
{
	MakeColorBuffer(mesh->m_Colors, mesh->m_VertexCount);
}

void MeshBufferColor::MakeColorBuffer(const Color* colors, int size)
{
	BindBuffer();
	MakeVertexBuffer(&m_VboColorHandle, sizeof(Color) * size, 4, colors, ATTR_POS_COLOR, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferColor::MakeBuffer(MeshPtr mesh)
{
	MeshBuffer::MakeBuffer(mesh);
	MakeColorBuffer(mesh->m_Colors, mesh->m_VertexCount);
}
