#pragma once

#include"SpRendering\Material.h"
#include"SpRendering\Rendering.h"
#include"SpRendering\MeshBufferUIInstance.h"
#include"GUIDefines.h"
#include"UIView.h"
#include<functional>

BEGIN_NAMESPACE_GUI

struct UIBatch
{
	int		m_StartingIndex;
	int		m_Count;
};

class UISystem
{
	typedef function<void(UIWidgetPtr)> ForeachCallback;
	const float ZNEAR = 0.001f;
	const float ZFAR = 1000.0f;
public:
	UISystem();
	~UISystem();

	void StartUp(IRenderingInterface* ri, int width, int height);
	void SetSize(int width, int height);
	void ShutDown();
	void UpdateAll(SMouseState mouseState);
	void RenderAll();
	void SubmitBatch(const vector<UIView*> list, const Matrix4x4& globalModelMatrix, MaterialPtr mat, 
		TexturePtr texture, int startingIndex, int count);
	void DrawInstance(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors, vector<SRect2D>& rects, 
		vector<Matrix4x4>& modelMatrices, const Matrix4x4& modelMatrix, MaterialPtr mat, TexturePtr texture);
	void AddChild(UIWidgetPtr widget);
	void RemoveChild(UIWidgetPtr widget);
	void Foreach(UIWidgetPtr widget, ForeachCallback callback);
	void ForeachAll(ForeachCallback callback);
	void ForeachWithModelMatrix(UIWidgetPtr widget, Matrix4x4& baseMatrix, ForeachCallback callback);
	void ForeachAllWithModelMatrix(ForeachCallback callback);

	inline ushort instanceCount() const { return m_InstanceCount; }
	inline ushort drawcalls() const { return m_DrawCalls; }
	inline TexturePtr GetSharedTexture() const { return m_SharedTexture; }
	inline MaterialPtr GetSharedMaterial() const { return m_SharedMaterial; }
	inline UIWidgetPtr GetRoot() const { return m_Root; }
	inline Vector2 ScreenPointToView(const Vector2& point) { return point - m_Root->GetHalfSize(); }
	inline Vector2 ViewPointToScreen(const Vector2& point) { return point + m_Root->GetHalfSize(); }

	template<typename T> shared_ptr<T> Create()
	{
		shared_ptr<T> ptr = make_shared<T>();
		ptr->m_System = this;
		return ptr;
	}

private:
	ushort					m_InstanceCount;
	ushort					m_DrawCalls;
	IRenderingInterface*	m_RI;
	Matrix4x4				m_ViewMatrix;
	Matrix4x4				m_ProjMatrix;
	MeshBufferUIInstancePtr	m_SharedMesh;
	TexturePtr				m_SharedTexture;
	MaterialPtr				m_SharedMaterial;
	UIWidgetPtr				m_Root;
	IInteractable*			m_LastIntracted;
	vector<UIView*>			m_ForRenderList;
	vector<IInteractable*>	m_ForInteractList;
	vector<TexcoordRange>	m_TexcoordRanges;
	vector<Color>			m_Colors;
	vector<SRect2D>			m_RectList;
	vector<Matrix4x4>		m_ModelMatrices;
};

END_NAMESPACE_GUI