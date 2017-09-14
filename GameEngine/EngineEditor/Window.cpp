#include"Window.h"
#include"Editor.h"
#include<GameEngine\Input.h>
#include<GameEngine\Debug.h>

CWindow::CWindow() {}

CWindow::CWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword)
{
	Create(class_name, instance, parent, width, height, dword);
}

CWindow::~CWindow()
{

}

void CWindow::Create(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword)
{
	int x = 0, y = 0;
	if (parent == NULL)
	{
		x = (GetSystemMetrics(0) - (int)width) >> 1;
		y = (GetSystemMetrics(1) - (int)height) >> 1;
	}
	else
	{
		RECT rect;
		GetWindowRect(parent, &rect);
		x = (((rect.right - rect.left) - (int)width) >> 1);
		y = (((rect.bottom - rect.top) - (int)height) >> 1);
	}
	m_state = EChannelState::Fixed;
	m_hwnd = CreateWindowEx(NULL, class_name, GetTitle(), dword | GetStyle(),
		x, y, width, height, parent, GetMenu(), instance, (void*)this);
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

EWindowType CWindow::GetType()
{
	return EWindowType::Other;
}

DWORD CWindow::GetStyle()
{
	return 	0;
}

HMENU CWindow::GetMenu()
{
	return NULL;
}

wchar_t* CWindow::GetTitle()
{
	return L"";
}

LRESULT CALLBACK CWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static FRect frect;
	static float width;
	static float height;

	switch (uMsg)
	{
	case WM_SIZE:
		CDebug::Log("window size col:%d row:%d", m_col, m_row);
		width = _Editor->WindowWidth;
		height = _Editor->WindowHeight;
		frect = _LocalClientToFRect(GetLocalRect(), width, height);
		if (frect.top >= 0.90f)
		{
			frect.top = m_normalizedRect.top;
		}
		_Editor->UpdateColumn(frect, m_col);
		_Editor->UpdateRow(frect, m_col, m_row);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CWindow::SetRect(RECT& rect)
{
	HWND parent = GetParent(m_hwnd);
	if (parent == NULL)
	{
		SetLocalRect(rect, rect.right - rect.left, rect.bottom - rect.top);
		return;
	}
	RECT parentRect = GetGlobalClientRect(parent);
	SetLocalRect(RECT{ rect.left - parentRect.left, rect.top - parentRect.top,
		rect.right - parentRect.left, rect.bottom - parentRect.top },
		parentRect.right - parentRect.left, parentRect.bottom - parentRect.top);
}

RECT CWindow::GetRect()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	return rect;
}

RECT CWindow::GetRectClient()
{
	return GetGlobalClientRect(m_hwnd);
}

void CWindow::SetLocalRect(RECT& rect, float parent_width, float parent_height)
{
	m_normalizedRect.left = (float)rect.left / parent_width;
	if((float)rect.top / parent_height < 0.95f)
	m_normalizedRect.top = (float)rect.top / parent_height;
	m_normalizedRect.right = (float)rect.right / parent_width;
	m_normalizedRect.bottom = (float)rect.bottom / parent_height;
	m_parentWidth = parent_width;
	m_parentHeight = parent_height;
	MoveWindow(m_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
}

RECT CWindow::GetLocalRect()
{
	HWND parent = GetParent(m_hwnd);
	if (parent == NULL)
		return GetRect();
	RECT parentRect = GetGlobalClientRect(parent);
	RECT childRect = GetRect();
	//int screenHeight = GetSystemMetrics(1);
	//if (childRect.top  >= screenHeight - parentRect.top)
	//{
	//	childRect.top = (parentRect.bottom - parentRect.top) * m_normalizedRect.top;
	//}
	return RECT{ childRect.left - parentRect.left, childRect.top - parentRect.top,
		childRect.right - parentRect.left, childRect.bottom - parentRect.top };
}

RECT CWindow::LocalToGlobalRect(const RECT& _rect)
{
	RECT rect;
	static POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(m_hwnd, &p);
	rect.left = p.x;
	rect.right = p.x + _rect.right - _rect.left;
	rect.top = p.y;
	rect.bottom = p.y + _rect.bottom - _rect.top;
	return rect;
}

RECT CWindow::GlobalToLocalRect(const RECT& rect)
{
	RECT childRect = GetRect();
	return RECT{ childRect.left - rect.left, childRect.top - rect.top,
		childRect.right - rect.left, childRect.bottom - rect.top };
}

RECT CWindow::GetGlobalClientRect(HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	RECT rect;
	static POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(hwnd, &p);
	rect.left = p.x;
	rect.right = p.x + clientRect.right - clientRect.left;
	rect.top = p.y;
	rect.bottom = p.y + clientRect.bottom - clientRect.top;
	return rect;
}

void CWindow::UpdateRect(float parent_width, float parent_height)
{
	SetLocalRect(_FRectToLocalClient(m_normalizedRect, parent_width, parent_height), parent_width, parent_height);
}

void CWindow::OnFieldChanged(const FRect& rect)
{
	CDebug::Log("col:%d row:%d left:%g top:%g right:%g bottom:%g", m_col, m_row, rect.left, rect.top, rect.right, rect.bottom);
	SetLocalRect(_FRectToLocalClient(rect, _Editor->WindowWidth, _Editor->WindowHeight)
		,_Editor->WindowWidth, _Editor->WindowHeight);
}
