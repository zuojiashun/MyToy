#include "ConsoleWindow.h"

CConsoleWindow::CConsoleWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	m_caption = L"Console";
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CConsoleWindow::GetType()
{
	return EWindowType::Console;
}

DWORD CConsoleWindow::GetStyle()
{
	return WS_CHILD;
}

LRESULT CALLBACK CConsoleWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChannel::WindowProc(hWnd, uMsg, wParam, lParam);
	static HGLRC hRC;
	static HDC hDC;
	static POINT p{ 0, 0 };
	static RECT rect;
	static int height, width;

	switch (uMsg)
	{
	case WM_CREATE:

		break;
	case WM_CLOSE:
		break;
	case WM_SIZE:

		break;
	case WM_COMMAND:

		break;
	case WM_MOVE:

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}