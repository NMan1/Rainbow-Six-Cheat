#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"  // really dx9 just to lazy to rename xDDD
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <string>
#include "Vector.h"
#pragma comment(lib, "d3d11.lib") 

extern std::string Status; 
extern int PasswordAttempt;

using namespace ImGui;

extern std::string Status;
namespace Menu
{
	/* DirectX Pointers */
	extern ID3D11Device*		 g_pd3dDevice;
	extern ID3D11DeviceContext* g_pd3dDeviceContext;
	extern IDXGISwapChain*      g_pSwapChain;
	extern ID3D11RenderTargetView* g_mainRenderTargetView;
	extern ID3D11BlendState*    g_pBlendState;

	/* Menu Colors */
	extern ImVec4 TabSlide;
	extern ImVec4 TabTitleText;
	extern ImVec4 RedText;
	extern ImVec4 ActiveButton;
	extern ImVec4 GreenText;
	extern ImVec4 Underline;
	extern ImVec4 ActiveBut;
	extern ImVec4 RegButton;

	/* Fonts*/
	extern ImFont* Font12;
	extern ImFont* Font14;
	extern ImFont* Font16;
	extern ImFont* Font18;
	extern ImFont* Font24;
	extern ImFont* Icons;

	/* Handle*/
	extern WNDCLASSEX wc;
	extern HWND hwnd;
	extern bool bDisplayAdmin;


	/* Functions */
	void BeginDraw();
	void EndDraw();

	void SetWindowLoop();

	void RenderLogin();
	void RenderButtons();
	void RenderMenu();
	void MenuShutDown();

	void Init();
	void SetUpLoginMenu();
	void SetupMenu();

	extern LPCTSTR Logo;
	extern ID3D11ShaderResourceView* LogoTexture;
}
