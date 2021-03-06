#include"LightComponent.h"
#include"Maker.h"
#include"SpRendering\Renderer.h"

void LightComponent::RenderShadowMap(LightComponent* light)
{
	if (!light) return;

	PRenderTexture renderTex = Light::GetShadowMap();
	PShader shadowShader = Shader::Get("shadow");
	Matrix4x4 shadow_view = Matrix4x4::LookAt(-light->gameObject->GetForward(), Vector3::zero, Vector3::forward);
	Matrix4x4 shadow_projection = Matrix4x4::Ortho(-10, 10, -10, 10, -10, 20);
	shadowMapMatrix = Matrix4x4::Translate(Vector3(0.5, 0.5, 0.5)) * Matrix4x4::Scale(Vector3(0.5, 0.5, 0.5)) * shadow_projection * shadow_view;

	Light::GetShadowMap()->BindBuffer();
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, renderTex->GetWidth(), renderTex->GetHeight());
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	shadowShader->Run();
	shadowShader->SetUniformParam("V", shadow_view);
	shadowShader->SetUniformParam("P", shadow_projection);

	IRenderer* renderer = NULL;
	_Maker->ForeachGameObject([&renderer, &shadowShader](CGameObject* go, int depth, Matrix4x4& mat) {
		if (go->IsActive() && (renderer = go->GetRenderer()) != NULL)
		{
			shadowShader->SetUniformParam("M", mat);
			renderer->Render(shadowShader);
		}
	});

	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightComponent::OnStart()
{
	CComponent::OnStart();
	lights.push_back(PLight(this));
}

void LightComponent::OnDestroy()
{
	CComponent::OnDestroy();
	for (auto it = lights.begin(); it != lights.end(); ++it)
	{
		if ((*it).get() == this)
		{
			lights.erase(it);
			return;
		}
	}
}

Vector3 LightComponent::GetLightPosOrDirection() const
{
	if (m_type == ELightType::Directional)
		return gameObject->GetForward();
	return gameObject->GetLocalPosition();
}




