#pragma once

#include"GLAppBase\GLAppBase.h"
#include"SpRendering\GLRendering.h"
#include"SpRendering\FontRenderer.h"
#include"SpCommon\Input.h"

USING_NAMESPACE_ENGINE;

#define SHOW_TEXT L"/* handle to face object2018北美车展：新款MINI家族正式发布 handle to face object2018北美车展：新款MINI家族正式发布"
//#define SHOW_TEXT L"h新款MINI家族正式发布 handle to face object2018北美车展：新款MINI家族正式发布"
//#define SHOW_TEXT L"Button"
#define FONT_PATH "../Assets/msyh.ttf"

class TextApp : public GLAppBase
{
	const int font_size = 14;
protected:
	virtual void InitGL(HWND hwnd) override;
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnWindowSizeChanged(int width, int height) override;
	virtual void OnRender() override;

	void DrawAtlas();

public:
	TextApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	float					font_interval_x = 0.0f;
	float					font_interval_y = 0.0f;
	float					m_Angle = 0.0f;
	Matrix4x4				modelMat;
	Matrix4x4				viewMat;
	Matrix4x4				projectionMat;
	FontRenderer*			font;
	IRenderingInterface*	m_RI;
	TexturePtr				m_Texture;
	MeshBufferTexcoordPtr		m_Buffer;
	MaterialPtr				m_Material;
};

