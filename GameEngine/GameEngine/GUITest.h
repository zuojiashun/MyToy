#include"EngineDefine.h"
#include"Math.h"
#include"Application.h"
#include"Input.h"
#include"Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"
#include"FileHead.h"
#include"GUISystem.h"
#include"GUIImage.h"
#include"GUILable.h"

using namespace std;
using namespace guisystem;

class GUITest : public CGameWindow
{
	CGameObject* go;
	CGameObject* go2;
	CTextRenderer* text;
	CGUIImage* widget;
	CGUILable* widget2;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);
};

