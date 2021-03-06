#include"Application.h"
#include"Engine.h"
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Input.h"
#include"Time.h"
#include"SpRendering\RenderTexture.h"
#include"SpAssetLoader\3DSModelLoader.h"
#include"Resources.h"
#include"SpCommon\Debug.h"
#include"Config.h"
#include"SkinnedMeshRenderer.h"
#include"SpAssetLoader\ColladaLoader.h"
#include"SpAssetLoader\AdvModelLoader.h"
#include<vector>

using namespace std;
using namespace spgameengine;

class CColladaTest : public CGameWindow
{
	AdvModelLoader* collada;
	PSkyBox box;
	PModel m_model;
	CGameObject* model;
	vector<PAnimationClip> m_clips;
	PTexture m_texture;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//应用程序必须实现的函数
	virtual void GetApplicationInfo(SApplicationInfo* info);
};

