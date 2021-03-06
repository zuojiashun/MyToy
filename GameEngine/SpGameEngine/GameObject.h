#ifndef _CGAMEOBJECT_
#define _CGAMEOBJECT_

#include<gl\glew.h>
#include<vector>
#include<map>
#include"SpCommon\Object.hpp"
#include"SpCommon\Math.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"SpRendering\Renderer.h"
#include"SpCommon\ApiDefine.h"

using namespace std;
using namespace spgameengine;

class ENGINE_API CGameObject : public Object
{
	REFLECT_CLASS(CGameObject)
protected:
	CGameObject();
	CGameObject(wstring name);

	string m_tag;
	int m_layer;
	bool m_active;
	vector<CComponent*> components;
	vector<CGameObject*> childs;
	CGameObject* parent;

	Vector3 localScale;
	Vector3 localPosition;
	Vector3 localEulerAngles;
	Vector3 realPosition;
	Vector3 right;
	Vector3 up;
	Vector3 forward;
	Quaternion rotation;

	Matrix4x4 localScaleMat;
	Matrix4x4 localMoveMat;
	Matrix4x4 localRotateMat;
	Matrix4x4 modelToWorldMat;

	void AddChild(CGameObject* child);
	void RemoveChild(CGameObject* child);
	void RemoveAllChilds();

public:
	friend class CEngine;
	friend class CMaker;

	void SetLocalScale(const Vector3& scale);
	const Vector3& GetLocalScale() const;
	void SetLocalPosition(const Vector3& pos);
	const Vector3& GetLocalPosition() const;
	void SetLocalEulerAngles(const Vector3& euler);
	const Vector3& GetLocalEulerAngles() const;
	void LookAt(const Vector3& target);
	void SetRotation(const Quaternion& q);
	Quaternion GetRotation() const;
	void SetParent(CGameObject* parent);
	void SetLayer(int layer);
	int GetLayer() const;
	void SetTag(const string& tag);
	string GetTag() const;
	void SetActive(bool active);
	bool IsActive() const;
	const Vector3& GetRealPosition() const;
	const Vector3& GetUp() const;
	const Vector3& GetRight() const;
	const Vector3& GetForward() const;
	CGameObject* GetParent() const;
	IRenderer* GetRenderer();
	Matrix4x4 ComputeModelToWorldMat();
	Matrix4x4 GetModelToWorldMat();
	Matrix4x4 GetRotateMatrix();
	void SiblingRiseOnce();
	void SiblingFallOnce();
	void SiblingRiseToTop();
	void SiblingFallToBottom();
	void SetSibling(int pos);

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnDestroy();

	template<typename T>
	T* AddComponent()
	{
		CComponent* t = new T();
		components.push_back(t);
		t->gameObject = this;
		t->OnStart();
		return (T*)t;
	}

	template<typename T>
	T* GetComponent()
	{
		vector<CComponent*>::iterator it = components.begin();
		while (it != components.end())
		{
			if (IS_TYPE(T, *it))
				return dynamic_cast<T*>(*it);
			++it;
		}
		return NULL;
	}

	template<typename T>
	bool DestroyComponent()
	{
		vector<CComponent*>::iterator it = components.begin();
		while (it != components.end())
		{
			if (IS_TYPE(T, *it))
			{
				components.erase(it);
				return true;
			}
			++it;
		}
		return false;
	}
};

#endif