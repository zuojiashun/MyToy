#include"ColladaTest.h"
#include"SpAssetLoader\ColladaLoader.h"
#include"Maker.h"
#include"SpRendering\SkyBox.h"
#include"SpRendering\MeshFactory.h"
#include"SpRendering\MeshBufferSkinning.h"
#include<glm\glm.hpp>

void CColladaTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 6.8, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));
	
	_FontManager->LoadFont(1, "C:/Windows/Fonts/simsun.ttc");

	model = _Maker->Instantiate(L"model");

	model->SetLocalPosition(Vector3(0, 1, 0));
	model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 0.05f);
	model->SetLocalEulerAngles(Vector3(0, -70, 0));
	PMaterial model_mat = make_shared<Material>();
	model_mat->SetShader(Shader::Get("skinning"))
		->SetMainTexture(Texture2D::Create("../Assets/shake.png"));
	collada = _Resources->Load<AdvModelLoader>("../Assets/models/shake_skin.xml");
	m_model = collada->m_model;
	//MeshBuffer* buffer = new MeshBuffer(m_model->m_Meshes[0], m_model->m_Skeleton->m_weights, m_model->m_Skeleton->m_indices);
	PMeshBufferSkinning buffer = make_shared<MeshBufferSkinning>(m_model->m_Meshes[0]);

	CSkinnedMeshRenderer* renderer = model->AddComponent<CSkinnedMeshRenderer>()->SetSkinningMesh(buffer, m_model->m_Skeleton)->SetMaterial(model_mat);
	
	_MainCameraGo->LookAt(model->GetLocalPosition());

	//m_clips.push_back(_Resources->LoadAnimation<AdvModelLoader>("../Assets/models/shake_move.xml"));
	m_clips.push_back(_Resources->LoadAnimation("../Assets/models/shake_move.xml"));
	m_clips.push_back(_Resources->LoadAnimation("../Assets/models/shake_hit.xml"));
	m_clips.push_back(_Resources->LoadAnimation("../Assets/models/shake_death.xml"));
	m_clips[0]->m_IsLooping = true;
	m_clips[1]->m_IsLooping = true;
	m_clips[2]->m_IsLooping = false;

	//box = SkyBox::Create("textures/skybox2/top.tga",
	//	"textures/skybox2/bottom.tga",
	//	"textures/skybox2/front.tga",
	//	"textures/skybox2/back.tga",
	//	"textures/skybox2/left.tga",
	//	"textures/skybox2/right.tga");

	//_MainCamera->SetSkyBox(box);
}

void CColladaTest::OnUpdate()
{
	if (m_clips.size() > 0)
	{
		vector<PAnimationClip> v(2);
		v[0] = m_clips[0];
		v[1] = m_clips[1];
		float times[] = { CTime::time, CTime::time };
		float weights[] = { 1.0f, 0.0f };
		//vector<JointPose> jointPoses = SkeletonAnimation::Blend(&v[0], times, weights, 2, m_model->m_Skeleton);
		vector<JointPose> jointPoses = SkeletonAnimation::Sample(v[0], m_model->m_Skeleton, CTime::time, 1);
		SkeletonAnimation::CalculateGlobalMatrix(m_model->m_Skeleton, jointPoses);
		SkeletonAnimation::CalculateSkinningMatrix(m_model->m_Skeleton);
	}

	CEditorTool::WatchTarget(*_MainCameraGo, model->GetLocalPosition());
}

void CColladaTest::OnRender()
{
	CEditorTool::DrawSkeleton(Matrix4x4::Scale(Vector3::one * 0.3f), *m_model->m_Skeleton);
}

void CColladaTest::OnClose()
{

}

void CColladaTest::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = _Config->GetValue<int>("window", "width");
		info->windowHeight = _Config->GetValue<int>("window", "height");
		info->isFullScreen = _Config->GetValue<bool>("window", "full_screen");
		info->windowBits = 32;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}