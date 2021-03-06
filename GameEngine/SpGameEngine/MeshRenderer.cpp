#include"MeshRenderer.h"
#include"GameObject.h"
#include"SpCommon\EngineDefine.h"
#include"Engine.h"
#include"Application.h"
#include"EditorTool.h"

IMPL_CLASS(CMeshRenderer)

void CMeshRenderer::OnStart()
{
	m_material = Material::GetDefaltMaterial();
}

void CMeshRenderer::RenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawAxis(modelMatrix);
}

void CMeshRenderer::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (!m_mesh) return;
	m_material->Bind();
	m_material->SetParam("M", modelMatrix);
	m_material->SetParam("V", viewMatrix);
	m_material->SetParam("P", projectionMatrix);
	m_mesh->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh->GetVertexNum());
	m_material->Unbind();
}

void CMeshRenderer::Render(PShader shader)
{
	if (!m_mesh) return;
	m_mesh->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh->GetVertexNum());
}

CMeshRenderer* CMeshRenderer::SetModel(PMeshBuffer mesh)
{
	this->m_mesh = mesh;
	return this;
}

CMeshRenderer* CMeshRenderer::SetMaterial(PMaterial material)
{
	this->m_material = material;
	return this;
}