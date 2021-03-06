#include"Editor.h"
#include"resource.h"
#include"Window.h"
#include"MainWindow.h"
#include"SceneWindow.h"
#include"WorldTreeWindow.h"
#include"ConsoleWindow.h"
#include"WatcherWindow.h"
#include"AssetsWindow.h"
#include"GridLayout.h"
#include<SpGameEngine\Config.h>
#include<SpGameEngine\SkyBox.h>
#include<SpGameEngine\Input.h>
#include<SpGameEngine\Time.h>

class CGridTest : public CGridLayoutElement
{
	virtual void OnLayoutChanged(const SRect2D& rect) 
	{

	}
};

int CEditor::InitEditor(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CConfig* config = CConfig::GetInstance();
	m_windowWidth = config->GetValue<float>("window", "width");
	m_windowHeight = config->GetValue<float>("window", "height");

	m_hInstance = hInstance;
	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = GetStockBrush(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass))
		return NULL;

	CWindow* mainWindow = OpenWindow<CMainWindow>(CLASS_NAME, hInstance, NULL, m_windowWidth, m_windowHeight, NULL);
	CWindow* sceneWindow = OpenWindow<CSceneWindow>(CLASS_NAME, hInstance, mainWindow->WindowHandle, 800, 600, NULL);
	CWindow* worldWindow = OpenWindow<CWorldTreeWindow>(CLASS_NAME, hInstance, mainWindow->WindowHandle, 250, 600, NULL);
	CWindow* consoleWindow = OpenWindow<CConsoleWindow>(CLASS_NAME, hInstance, mainWindow->WindowHandle, 250, 600, NULL);
	CWindow* watcherWindow = OpenWindow<CWatcherWindow>(CLASS_NAME, hInstance, mainWindow->WindowHandle, 250, 600, NULL);
	CWindow* assetsWindow = OpenWindow<CAssetsWindow>(CLASS_NAME, hInstance, mainWindow->WindowHandle, 250, 600, NULL);

	m_headerHeight = mainWindow->GetHeaderHeight();

	//m_layout.InsertColumn(FRect{ 0.0f, 0.0f, 0.2f, 1.0f }, 0);
	//m_layout.InsertColumn(FRect{ 0.2f, 0.0f, 0.8f, 1.0f }, 1);
	//m_layout.InsertColumn(FRect{ 0.8f, 0.0f, 1.0f, 1.0f }, 2);
	//m_layout[0].InsertRow(FRect{}, 0, true);
	//m_layout[1].InsertRow(FRect{}, 0, true);
	//m_layout[2].InsertRow(FRect{}, 0, true);
	//m_layout[0][0].set(worldWindow);
	//m_layout[1][0].set(sceneWindow);
	//m_layout[2][0].set(watcherWindow);
	//m_layout[1].InsertRow(FRect{ 0.2f, 0.7f, 0.8f, 1.0f }, 1);
	//m_layout[1][1].set(consoleWindow);

	//0--0.147--0.71--0.85--1
	m_layout.InsertColumn(FRect{ 0.0f, 0.0f, 0.147f, 1.0f }, 0);
	m_layout.InsertColumn(FRect{ 0.147f, 0.0f, 0.71f, 1.0f }, 1);
	m_layout.InsertColumn(FRect{ 0.71f, 0.0f, 0.85f, 1.0f }, 2);
	m_layout.InsertColumn(FRect{ 0.85f, 0.0f, 1.0f, 1.0f }, 3);
	m_layout[0].InsertRow(FRect{}, 0, true);
	m_layout[1].InsertRow(FRect{}, 0, true);
	m_layout[2].InsertRow(FRect{}, 0, true);
	m_layout[3].InsertRow(FRect{}, 0, true);
	m_layout[0][0].set(worldWindow);
	m_layout[1][0].set(sceneWindow);
	m_layout[2][0].set(assetsWindow);
	m_layout[3][0].set(watcherWindow);
	//m_layout[1].InsertRow(FRect{ 0.2f, 0.7f, 0.8f, 1.0f }, 1);
	//m_layout[1][1].set(consoleWindow);

	for (pair<EWindowType, CWindow*> kv : m_windows)
	{
		kv.second->OnWindowInitialized();
	}

	CInput::Mode = EMouseMode::Absolute;
	FontManager->LoadFont(1, "C:/Windows/Fonts/simkai.ttf");

	_Engine->MakeRenderContext();
	CSkyBox* box = CSkyBox::Create("textures/skybox2/top.tga",
		"textures/skybox2/bottom.tga",
		"textures/skybox2/front.tga",
		"textures/skybox2/back.tga",
		"textures/skybox2/left.tga",
		"textures/skybox2/right.tga");
	//_MainCamera->SetSkyBox(box);

	CMessageCenter::Send(SMessage{ MSG_ON_ENGINE_INIT_COMPLETE, this, _Engine });
	return 0;
}

int CEditor::EditorLoop()
{
	MSG msg;

	bool isExiting = false;

	while (!isExiting)
	{
		_Engine->Update();
		_Engine->MakeRenderContext();
		_Engine->Render();
		SwapBuffers(_Engine->DCHandle);
		UpdateWindow();
		RenderWindow();
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				isExiting = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CTime::UpdateTime();
		CTime::Lock();
	}
	return 0;
}

void CEditor::UpdateColumn(const FRect& rect, int index)
{
	m_layout.UpdateColumn(rect, index);
}

void CEditor::UpdateRow(const FRect& rect, int col, int row)
{
	m_layout[col].UpdateRow(rect, row);
}

void CEditor::UpdateSize()
{
	auto it = m_windows.find(EWindowType::Main);
	if (it == m_windows.end())
	{
		return;
	}
	RECT rect = it->second->GetLocalRect();
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	m_windowWidth = width;
	m_windowHeight = height;
	for (pair<EWindowType, CWindow*> p : m_windows)
	{
		if (p.first != EWindowType::Main)
			p.second->UpdateRect(width, height);
	}
}

void CEditor::UpdatePosition()
{
	for (pair<EWindowType, CWindow*> p : m_windows)
	{
		p.second->OnPositionChanged();
	}
}

vector<CWindow*> CEditor::GetWindowsExcept(EWindowType type)
{
	vector<CWindow*> list;
	for (pair<EWindowType, CWindow*> p : m_windows)
	{
		if (p.first != type && p.first != EWindowType::Main)
			list.push_back(p.second);
	}
	return list;
}

void CEditor::UpdateWindow()
{
	for (pair<EWindowType, CWindow*> p : m_windows)
	{
		p.second->OnUpdate();
	}
}

void CEditor::RenderWindow()
{
	for (pair<EWindowType, CWindow*> p : m_windows)
	{
		p.second->OnRender();
	}
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOVE)
	{
		int j = 0;
	}
	CWindow* window = (CWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	if (window)
		return window->WindowProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		window = (CWindow*)((LPCREATESTRUCT)(lParam))->lpCreateParams;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (long)window);
		if (window)
			window->WindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_Editor->InitEditor(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	return _Editor->EditorLoop();
}

