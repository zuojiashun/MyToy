#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define _Resources CResources::GetInstance()

#include<Windows.h>
#include<map>
#include<string>
#include"SpAssetLoader\IAssetLoader.h"
#include"DynamicFactory.h"
#include"SpCommon\Singleton.h"
#include"SpRendering\SkeletonAnimation.h"

using namespace std;
using namespace spgameengine;

class ENGINE_API CResources : public Singleton<CResources>
{
private:
	map<string, IAssetLoader*> m_sources;

public:
	template<typename T>
	T* Load(string path)
	{
		auto it = m_sources.find(path);
		IAssetLoader* sourceFile = NULL;
		if (it == m_sources.end())
		{
			sourceFile = new T();
			sourceFile->LoadFromFile(path.c_str());
			m_sources.insert(make_pair(path, sourceFile));
		}
		else
		{
			sourceFile = it->second;
		}
		return dynamic_cast<T*>(sourceFile);
	}

	PAnimationClip LoadAnimation(string path);

	template<typename T>
	PAnimationClip LoadAnimation(string path)
	{
		T* modelFile = _Resources->Load<T>(path);
		PModel model = modelFile->m_model;
		PAnimationClip clip;
		if (model->animationsCount() > 0)
			clip = model->m_Animations[0];
		delete(modelFile);
		return clip;
	}

	void Unload(string& path);
};

#endif