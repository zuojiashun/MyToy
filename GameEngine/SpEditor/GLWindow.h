#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#include"Window.h"
#include"GUIManager.h"
#include"GUIText.h"
#include"GUITexture.h"
#include"GUIElement.h"
#include"GUIGroup.h"
#include"GUITextEdit.h"
#include"GUIHorizontalLayout.h"
#include<SpGameEngine\RawRenderer.h>

class CGLWindow : public CWindow
{
protected:
	CRawRenderer m_renderer;
	CGUIManager m_gui;

public:

	CRawRenderer& GetRenderer();
	CGUIManager& GetGUIManager();
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnPositionChanged() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnDraw();
};

#endif