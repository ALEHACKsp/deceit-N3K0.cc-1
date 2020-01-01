#pragma once


#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <mutex>

#include <MinHook.h>
#pragma comment(lib,"MinHook.x64.lib")

#include <memory>

typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
typedef void(__stdcall* D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
typedef void(__stdcall* D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

static HWND                     g_hWnd = nullptr;
static HMODULE					g_hModule = nullptr;
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static std::once_flag           g_isInitialized;

namespace Hook {
	extern D3D11PresentHook                phookD3D11Present;
	extern D3D11DrawIndexedHook            phookD3D11DrawIndexed;
	extern D3D11CreateQueryHook			phookD3D11CreateQuery;
	extern D3D11PSSetShaderResourcesHook	phookD3D11PSSetShaderResources;
	extern D3D11ClearRenderTargetViewHook  phookD3D11ClearRenderTargetViewHook;

	extern DWORD_PTR* pSwapChainVTable;
	extern DWORD_PTR* pDeviceVTable;
	extern DWORD_PTR* pDeviceContextVTable;

	HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	void __stdcall DrawIndexedHook(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void __stdcall hookD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
	void __stdcall hookD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews); 
	void __stdcall ClearRenderTargetViewHook(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

	DWORD Hook(HWND hwnd);
	void UnHook();
}


