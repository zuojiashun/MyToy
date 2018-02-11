﻿//#define _TEST_
#ifdef _TEST_

#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<memory>
#include"SpRendering/GLRendering.h"
#include"SpRendering/Material.h"
#include"SpRendering/MeshBuffer.h"
#include"SpRendering/MeshFactory.h"
#include"SpRendering/Texture2D.h"
#include"SpRendering/FontRenderer.h"
#include"SpCommon/FastPainter.h"
#include"SpCommon/Input.h"

USING_NAMESPACE_ENGINE;

int width = 800;
int height = 600;
bool isExiting = false;
GLRendering* rendering;
FontRenderer* font;
HWND m_Hwnd;
RenderingObject obj;
Matrix4x4 textMat;
Matrix4x4 modelMat;
Matrix4x4 viewMat;
Matrix4x4 projectionMat;

const int font_size = 14;
const int canvas_w = 512;
const int canvas_h = 512;
float font_interval_x = 0.0f;
float font_interval_y = 0.0f;
uint pixels[canvas_w * canvas_h];
PTexture2D canvasTexture;

#define FONT_PATH "D:/GitHub/MyToy/GameEngine/SpGameEngine/fonts/msyh.ttf"
//#define FONT_PATH "C:/Windows/Fonts/DengXian.ttf"
#define SHOW_TEXT L"/* handle to face object2018北美车展：新款MINI家族正式发布 handle to face object2018北美车展：新款MINI家族正式发布"
//#define SHOW_TEXT L"/*款MINI家族正式发布 handle to face object"
//#define SHOW_TEXT L"D:/GitHub/MyToy/GameEngine/SpGameEngine/fonts/msyh.ttf/nD:/GitHub/MyToy/GameEngine/SpGameEngine/"
//#define SHOW_TEXT L"+-*()[]{}:;',.?!"
#define CLASS_NAME L"NAME"

bool isCreated = false;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

void my_draw_bitmap(FT_Bitmap* bm, FT_Int left, FT_Int top)
{
	int x, y;
	for (int i = 0; i < bm->width; ++i)
	{
		for (int j = 0; j < bm->rows; ++j)
		{
			x = left + i;
			y = (top + j);
			if (x < 0 || x >= canvas_w || y < 0 || y >= canvas_h)
				continue;
			float grey = bm->buffer[j * bm->width + i];
			pixels[y * canvas_w + x] = _RGBA32((byte)grey, (byte)grey, (byte)grey, 255);
		}
	}
}

void CreateText(const wchar_t* text)
{
	FT_Library  library;   /* handle to library     */
	FT_Face     face;      /* handle to face object */

	int error = FT_Init_FreeType(&library);
	if (error) {  }

	error = FT_New_Face(library,
		FONT_PATH,
		0,
		&face);
	if (error == FT_Err_Unknown_File_Format)
	{
		//... the font file could be opened and read, but it appears
		//	... that its font format is unsupported
	}
	else if (error)
	{
		// another error code means that the font file could not
		//	... be opened or read, or that it is broken...
	}
	error = FT_Set_Char_Size(
		face,    /* handle to face object           */
		0,       /* char_width in 1/64th of points  */
		font_size * 64,   /* char_height in 1/64th of points */
		0,     /* horizontal device resolution    */
		128);   /* vertical device resolution      */

	FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */
	int           pen_x, pen_y, n;
	pen_x = 0;
	pen_y = 100;

	for (n = 0; n < lstrlenW(text); n++)
	{
		FT_UInt  glyph_index;


		/* retrieve glyph index from character code */
		glyph_index = FT_Get_Char_Index(face, text[n]);

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
			continue;  /* ignore errors */

					   /* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
			continue;

		/* now, draw to our target surface */
		my_draw_bitmap(&slot->bitmap,
			pen_x + slot->bitmap_left,
			pen_y - slot->bitmap_top);

		/* increment pen position */
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6; /* not useful for now */
	}

	//memset(pixels, 255, sizeof(uint) * canvas_w * canvas_h);
	canvasTexture = Texture2D::Create((UCHAR*)pixels, canvas_w, canvas_h);
	canvasTexture->SetWrapMode(ETexWrapMode::Clamp);
}

LRESULT CALLBACK MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{	
		GLRenderingStartUpParams params;
		params.m_HWND = hWnd;
		params.m_ScreenWidth = width;
		params.m_ScreenHeight = height;
		rendering = new GLRendering();
		rendering->StartUp(&params);

		CreateText(SHOW_TEXT);

		TrueTypeFont* f = _FontManager->LoadFont(1, FONT_PATH);
		font = new FontRenderer();
		font->SetFont(f);
		font->SetColor(Color::green);
		font->SetEffect(EFontEffect::Shadow)->SetEffectVector(Vector3(1, -1, 0))->SetEffectColor(Color::black);
		font->SetFontSize(font_size); 
		font->SetTextAlignment(EAlignment::CENTER_MIDDLE);
		font->SetRenderType(ERenderType::Smart);
		font->SetTextRect(SRect2D(0, 0, 4, 1)); 
		font->SetText(SHOW_TEXT); 
		font->SetIntervalX(font_interval_x); 
		font->SetIntervalY(font_interval_y);
		obj.material = new Material();
		obj.material->SetShader(Shader::Get("texture"))->SetMainTexture(/*Texture2D::Create("D://GitHub//MyToy//GameEngine//SpGameEngine//textures//shake.png"*/canvasTexture);
		obj.mesh = _MeshFactory->SharedBuffer(EMeshType::Cube).get();

		shared_ptr<Material> m = make_shared<Material>();
		m->SetMainTexture(Texture2D::Create("D://GitHub//MyToy//GameEngine//SpGameEngine//textures//shake.png"));

		isCreated = true;
	}
	break;
	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		glViewport(0, 0, width, height);
		projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);
		FastPainter::Perspective(45, width / (float)height, 0.1, 1000);
	}
		break;
	case WM_MOVE:

		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CreateApp()
{
	HINSTANCE mHIntance = GetModuleHandle(NULL);
	WNDCLASSEX windowClass;
	RECT       windowRect;
	HWND hwnd;

	windowRect.left = (long)0;
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MessageHandle;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = mHIntance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass))
		return;

	int x = (GetSystemMetrics(0) - width) >> 1;
	int y = (GetSystemMetrics(1) - height) >> 1;

	m_Hwnd = hwnd = CreateWindowEx(NULL,
		CLASS_NAME,
		L"TestApp",
		WS_OVERLAPPEDWINDOW,
		x, y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		mHIntance,
		NULL);

	if (!hwnd)
		return;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	CInput::Init(mHIntance, hwnd);
}

void Update()
{
	CInput::GetState(RECT{});

	if (CInput::GetKeyDown(DIK_NUMPAD1))
	{
		font->SetTextAlignment(EAlignment::LEFT_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD2))
	{
		font->SetTextAlignment(EAlignment::CENTER_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD3))
	{
		font->SetTextAlignment(EAlignment::RIGHT_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD4))
	{
		font->SetTextAlignment(EAlignment::LEFT_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD5))
	{
		font->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD6))
	{
		font->SetTextAlignment(EAlignment::RIGHT_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD7))
	{
		font->SetTextAlignment(EAlignment::LEFT_TOP);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD8))
	{
		font->SetTextAlignment(EAlignment::CENTER_TOP);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD9))
	{
		font->SetTextAlignment(EAlignment::RIGHT_TOP);
	}

	if (CInput::GetKeyDown(DIK_1))
	{
		font->SetColor(Color::orange);
	}
	if (CInput::GetKeyDown(DIK_2))
	{
		font->SetColor(Color::cyan);
	}
	if (CInput::GetKeyDown(DIK_3))
	{
		font->SetColor(Color::green);
	}
	if (CInput::GetKeyDown(DIK_4))
	{
		font->SetColor(Color::red);
	}

	float h = CInput::GetAxis("Horizontal");
	float v = CInput::GetAxis("Vertical");
	if (h != 0.0f)
	{
		font_interval_x += h * 0.01f;
		font->SetIntervalX(font_interval_x);
	}
	if (v != 0.0f)
	{
		font_interval_y += v * 0.05f;
		font->SetIntervalY(font_interval_y);
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1);

	modelMat = Matrix4x4::Rotate(0, 0, 180) * Matrix4x4::Scale(Vector3::one * 5);
	//modelMat = Matrix4x4::Rotate(0, GetTickCount() / 10, 0);
	textMat = Matrix4x4::Identity();

	font->OnRender(textMat, viewMat, projectionMat);

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	FastPainter::DrawRect(font->GetTextRect(), textMat * Matrix4x4::Scale(Vector3::one));

}

int GameLoop()
{
	MSG msg;
	HDC hdc = GetDC(m_Hwnd);
	while (!isExiting)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (isCreated)
		{
			Update();
			Render();
			SwapBuffers(hdc);
		}
	}

	return 0;
}

int main()
{
	CreateApp();

	modelMat = Matrix4x4::Identity();
	viewMat = Matrix4x4::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);
	projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);

	FastPainter::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);

	GameLoop();
	return 0;
}
#endif