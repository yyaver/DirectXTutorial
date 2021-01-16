#include<Windows.h>
#include "windowsMessageMap.h"
#include <tchar.h>
#include<sstream>
#include<d3d11.h>
#pragma comment (lib,"d3d11.lib")

LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugStringA((mm(msg, lParam, wParam).c_str()));
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	
	case WM_KEYDOWN:
	
		if (wParam == 'F')
		{
			SetWindowText(hWnd, _T("zaa"));
		}
		break;


	case WM_KEYUP:

		if (wParam == 'F')
		{
			SetWindowText(hWnd, _T("eeee"));
		}
		break;
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowTextA(hWnd, oss.str().c_str());

	}
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	const auto pClassName= "pClassName";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = (LPCWSTR)pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindowEx(0, (LPCWSTR)pClassName, _T("zaa"),
		WS_CAPTION | WS_MINIMIZE | WS_SYSMENU,
		200, 200, 640, 240,
		nullptr, nullptr,
		hInstance, nullptr);
	MSG msg;
	BOOL gResult;
	ShowWindow(hWnd, SW_SHOW);
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	ID3D11RenderTargetView* pTarget = nullptr;
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
	
	ID3D11Resource* pBckbuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof ( ID3D11Resource), reinterpret_cast<void**>(&pBckbuffer));
	pDevice->CreateRenderTargetView(pBckbuffer, nullptr, &pTarget);
//	pBckbuffer->Release();
	const float color[4] = { 0.5f, 0.0f, 1.0f,1.0f};
	
	//pContext->ClearRenderTargetView(pTarget, color);
	pSwap->Present(1u, 0u);
	//pContext->ClearRenderTargetView(pTarget, color);
	
	while ((gResult =GetMessage(&msg, nullptr, 0, 0) > 0))
	{
		//const float c = sin(timer.Peek()) / 2.0f + 0.5f;ff
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		 pContext->ClearRenderTargetView(pTarget, color);
		pSwap->Present(2u, 0u);
		
		
	}
	if(gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}



}