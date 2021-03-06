#include"GUIElement.h"
#include"GUIManager.h"
#include<SpGameEngine\RawRenderer.h>

CGUIElement::CGUIElement() : m_enable(true), m_depth(0) {}
CGUIElement::~CGUIElement() {}

void CGUIElement::SetState(EElementState state)
{
	m_state = state;
	OnStateChanged();
}

void CGUIElement::OnMouseDown(Vector2 mousePos)
{
	OnGotFocus();
	m_haveFocus = true;
	if (onMouseDown.size() > 0)
		for (auto it = onMouseDown.begin(); it != onMouseDown.end(); ++it)
			(*it)(mousePos);
}

void CGUIElement::OnMouseUp(Vector2 mousePos)
{
	if (onMouseUp.size() > 0)
		for (auto it = onMouseUp.begin(); it != onMouseUp.end(); ++it)
			(*it)(mousePos);
}

void CGUIElement::OnMouseClick(Vector2 mousePos)
{
	if (onMouseClick.size() > 0)
		for (auto it = onMouseClick.begin(); it != onMouseClick.end(); ++it)
			(*it)(mousePos);
}

void CGUIElement::OnMouseEnter(Vector2 mousePos)
{
	if (onMouseEnter.size() > 0)
		for (auto it = onMouseEnter.begin(); it != onMouseEnter.end(); ++it)
			(*it)(mousePos);
}

void CGUIElement::OnMouseExit(Vector2 mousePos)
{
	if (onMouseExit.size() > 0)
		for (auto it = onMouseExit.begin(); it != onMouseExit.end(); ++it)
			(*it)(mousePos);
}

void CGUIElement::OnMouseOver(Vector2 mousePos)
{
	if (onMouseOver.size() > 0)
		for (auto it = onMouseOver.begin(); it != onMouseOver.end(); ++it)
			(*it)(mousePos);
}

void CGUIElement::OnGotFocus()
{

}

void CGUIElement::OnLostFocus()
{

}

void CGUIElement::OnStart()
{
	
}

void CGUIElement::OnDestroy()
{
	
}

void CGUIElement::OnUpdate()
{
	
}

#include"GUIHorizontalLayout.h"
void CGUIElement::OnRender()
{
	if (m_fill)
	{
		m_manager->GetRenderer()->DrawRect(m_rect, m_fillColor + m_addColor);
	}
}

bool CGUIElement::Overlay(Vector2 pos)
{
	return m_rect.Overlay(pos);
}

bool CGUIElement::IsCollide()
{
	return m_collide;
}

bool CGUIElement::IsEnalbe()
{
	return m_enable;
}

SRect2D CGUIElement::GetRect()
{
	return m_rect;
}

int CGUIElement::GetLayer()
{
	return m_layer;
}

int CGUIElement::GetDepth()
{
	return m_depth;
}

float CGUIElement::GetWidth()
{
	return m_width;
}

float CGUIElement::GetHeight()
{
	return m_height;
}

EElementState CGUIElement::GetState()
{
	return m_state;
}

bool CGUIElement::IsState(EElementState state)
{
	return m_state == state;
}

bool CGUIElement::Visible()
{
	return (m_cell && m_cell->m_visible) || !m_cell;
}

void CGUIElement::EnableClip(bool clip)
{
	if (clip)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_rect.center_x - m_rect.half_size_x, m_rect.center_y - m_rect.half_size_y, m_rect.half_size_x * 2, m_rect.half_size_y * 2);
	}
	else glDisable(GL_SCISSOR_TEST);
}

CGUIElement* CGUIElement::SetCollide(bool isCollide)
{
	m_collide = isCollide;
	return this;
}

CGUIElement* CGUIElement::SetFill(bool isFill)
{
	m_fill = isFill;
	return this;
}

CGUIElement* CGUIElement::SetFillColor(Color fillColor)
{
	this->m_fillColor = fillColor;
	return this;
}

CGUIElement* CGUIElement::SetRect(SRect2D rect)
{
	this->m_rect = rect;
	this->m_width = rect.half_size_x * 2;
	this->m_height = rect.half_size_y * 2;
	this->m_position.x = rect.center_x;
	this->m_position.y = rect.center_y;
	return this;
}

CGUIElement* CGUIElement::SetLayer(int layer)
{
	this->m_layer = layer;
	m_manager->UpdateWidgetLayer(this);
	return this;
}

CGUIElement* CGUIElement::SetWidth(float width)
{
	this->m_width = width;
	this->m_rect.half_size_x = width * 0.5f;
	return this;
}

CGUIElement* CGUIElement::SetHeight(float height)
{
	this->m_height = height;
	this->m_rect.half_size_y = height * 0.5f;
	return this;
}

CGUIElement* CGUIElement::SetEnable(bool enable)
{
	m_enable = enable;
	return this;
}

CGUIElement* CGUIElement::SetPosition(const Vector2& position)
{
	m_position = position;
	m_rect.center_x = position.x;
	m_rect.center_y = position.y;
	return this;
}

CGUIElement* CGUIElement::SetDepth(int depth)
{
	m_depth = depth;
	return this;
}

CGUIElement* CGUIElement::SetParent(CGUIElement* parent)
{
	m_parent = parent;
	return this;
}

void CGUIElement::OnStateChanged()
{
	switch (m_state)
	{
	case EElementState::Normal:
		m_addColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	case EElementState::Hover:
		m_addColor = Color(0.2f, 0.2f, 0.2f, 0.0f);
		break;
	case EElementState::Pressed:
		m_addColor = Color(-0.3f, -0.3f, -0.3f, 0.0f);
		break;
	case EElementState::Disabled:
		m_addColor = Color(-0.5f, -0.5f, -0.5f, 0.0f);
		break;
	}
}

void CGUIElement::OnLayoutChanged(const SRect2D& rect)
{
	SetRect(rect);
}

void CGUIElement::OnVisibleChanged(bool visible)
{

}

CGUIElement* CGUIElement::AddOnMouseDownListener(OnMouseDownCallback down)
{
	onMouseDown.push_back(down);
	return this;
}

CGUIElement* CGUIElement::AddOnMouseUpListener(OnMouseUpCallback up)
{
	onMouseUp.push_back(up);
	return this;
}

CGUIElement* CGUIElement::AddOnMouseClickListener(OnMouseClickCallback click)
{
	onMouseClick.push_back(click);
	return this;
}

CGUIElement* CGUIElement::AddOnMouseEnterListener(OnMouseEnterCallback enter)
{
	onMouseEnter.push_back(enter);
	return this;
}

CGUIElement* CGUIElement::AddOnMouseExitListener(OnMouseExitCallback exit)
{
	onMouseExit.push_back(exit);
	return this;
}

CGUIElement* CGUIElement::AddOnMouseOverListener(OnMouseOverCallback over)
{
	onMouseOver.push_back(over);
	return this;
}

CGUIElement* CGUIElement::RemoveAllOnMouseDownListener()
{
	onMouseDown.clear();
	return this;
}

CGUIElement* CGUIElement::RemoveAllOnMouseUpListener()
{
	onMouseUp.clear();
	return this;
}

CGUIElement* CGUIElement::RemoveAllOnMouseClickListener()
{
	onMouseClick.clear();
	return this;
}

CGUIElement* CGUIElement::RemoveAllOnMouseEnterListener()
{
	onMouseEnter.clear();
	return this;
}

CGUIElement* CGUIElement::RemoveAllOnMouseExitListener()
{
	onMouseExit.clear();
	return this;
}

CGUIElement* CGUIElement::RemoveAllOnMouseOverListener()
{
	onMouseOver.clear();
	return this;
}

bool CGUIElement::operator>(const CGUIElement& widget) const
{
	return m_layer < widget.m_layer;
}

bool CGUIElement::operator<(const CGUIElement& widget) const
{
	return m_layer > widget.m_layer;
}