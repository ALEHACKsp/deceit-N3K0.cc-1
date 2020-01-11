#include "hook.h"

#include "..//d3d_render/render.hpp"
#include "..//sdk/class.h"
#include "..//utils/vtable_hook.h"

#include "..//feature/esp.h"
#include "..//feature/misc.h"

namespace Hook {
	D3D11PresentHook                phookD3D11Present = nullptr;
	D3D11DrawIndexedHook            phookD3D11DrawIndexed = nullptr;
	D3D11CreateQueryHook			phookD3D11CreateQuery = nullptr;
	D3D11PSSetShaderResourcesHook	phookD3D11PSSetShaderResources = nullptr;
	D3D11ClearRenderTargetViewHook  phookD3D11ClearRenderTargetViewHook = nullptr;

	DWORD_PTR* pSwapChainVTable = nullptr;
	DWORD_PTR* pDeviceVTable = nullptr;
	DWORD_PTR* pDeviceContextVTable = nullptr;


	void __stdcall DrawIndexedHook(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {
		return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}
	void __stdcall hookD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery) {
		if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
		{
			D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
			(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
			(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;

			return phookD3D11CreateQuery(pDevice, &oqueryDesc, ppQuery);
		}

		return phookD3D11CreateQuery(pDevice, pQueryDesc, ppQuery);
	}

	UINT pssrStartSlot;
	D3D11_SHADER_RESOURCE_VIEW_DESC Descr;

	void __stdcall hookD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) {
		pssrStartSlot = StartSlot;

		for (UINT j = 0; j < NumViews; j++)
		{
			ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[j];
			if (pShaderResView)
			{
				pShaderResView->GetDesc(&Descr);

				if ((Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFER) || (Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFEREX))
				{
					continue; //Skip buffer resources
				}
			}
		}

		return phookD3D11PSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);
	}
	void __stdcall ClearRenderTargetViewHook(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]) {
		return phookD3D11ClearRenderTargetViewHook(pContext, pRenderTargetView, ColorRGBA);
	}

}


vtable_hook CFireMode;

vtable_hook CLog;
vtable_hook CWeapon;
vtable_hook physical;

typedef bool(__thiscall* canfire_fn)(void*, bool);
bool __fastcall canfire_hook(bool forrce) {
	return true;
}

signed __int64 __fastcall GetFireRate(void* v,bool a2) {
	uintptr_t rax = *(uintptr_t*)((uintptr_t)v + 0x10);
	uintptr_t rcx = *(uintptr_t*)(rax + 0x8);
	*(__int16*)(rcx + 0x90) = (__int16)2000; // I have some problem with firerate idk why when i edit value from class it just access violation crash
	return CFireMode.get_original<signed __int64(__thiscall*)(void*, bool)>(102)(v,a2);
}

typedef int(__thiscall* ray_function)(void*, SRWIParams*, const char*, int);
int __stdcall ray_function_hook(void* this_, SRWIParams* rp, const char* pNameTag, int iCaller) {
	// i can't make silent aim :(
	int result = physical.get_original<ray_function>(35)(sdk::global_var::pPhysicalWorld, rp, pNameTag, iCaller);
	return result;
}




#define is_null(var) (var == 0)

std::unique_ptr<Renderer> renderer;

HRESULT __stdcall Hook::PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) 
{

	static bool print_localpalyer = false;
	static bool dump_convar		  = false;
	static bool cur_weapon_hook   = false;
	static bool hook_bullshit     = false;

	if (!hook_bullshit) {
		if (sdk::global_var::pPhysicalWorld != 0) {
			physical.setup(sdk::global_var::pPhysicalWorld);
			physical.hook_index(35, ray_function_hook);
			hook_bullshit = true;
		}
	}

	std::call_once(g_isInitialized, [&]() {	
		pSwapChain->GetDevice(__uuidof(g_pd3dDevice), reinterpret_cast<void**>(&g_pd3dDevice));
		g_pd3dDevice->GetImmediateContext(&g_pd3dContext);
		printf("g_pd3dDevice -> %p \n", g_pd3dDevice);
		renderer = std::make_unique<Renderer>(g_pd3dDevice);
		});

	renderer->begin();

	renderer->drawText(Vec2(10.0f,  50.0f), "N3k0.cc", XMCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FW1_LEFT, 15.0f);
	renderer->drawText(Vec2(10.0f,  66.0f), "DECEIT V1 rebuild", XMCOLOR(1.0f, 0, 0, 1.0f), FW1_LEFT, 15.0f);



	if (sdk::global_var::pGameFramework) {

		if (!dump_convar) {
			misc::dump_all_cvar();
			dump_convar = true;
		}

		IEntity* localplayer = sdk::global_var::pGameFramework->get_client_entity();
		IActor* local_actor = sdk::global_var::pGameFramework->get_client_actor();

		if (!is_null(localplayer)) {
			if (!print_localpalyer) {
				printf("LocalEntity : %p LocalActor %p CEntityRender %p \n",
					localplayer,
					sdk::global_var::pGameFramework->get_client_actor(),
					localplayer->get_entity_render());
				print_localpalyer = true;
			}

			renderer->drawText(Vec2(10.0f, 81.0f), XMCOLOR(1.0f, 1.0f, 0.0f, 1.0f), FW1_LEFT, 15.0f, "LocalPlayer %p", localplayer);
			renderer->drawText(Vec2(10.0f, 96.0f), XMCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FW1_LEFT, 15.0f, "x : %.2f y : %.2f z : %.2f ", localplayer->m_position.x, localplayer->m_position.y, localplayer->m_position.z);
			renderer->drawText(Vec2(10.0f, 111.0f), XMCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FW1_LEFT, 15.0f, "teleport_key : V + ALT distance : num7 fov : num9 ,%d ", misc::sort_mode);

			if (localplayer) {
				if (localplayer->is_invisible())
					renderer->drawText(Vec2(10.0f, 127.0f), XMCOLOR(1.0f, 0, 0, 1.0f), FW1_LEFT, 15.0f, "Invisible");

				IItem* current_item = local_actor->get_current_item();
				if (current_item) {
					IWeapon* cur_weapon = current_item->GetIWeapon();
					if (cur_weapon) {

						if (cur_weapon->m_ammo)
							cur_weapon->m_ammo->count = 0x539;

						if (cur_weapon->m_fm) {
							if (!cur_weapon_hook) { // I  can just detour jmp this thing but Im to lazy 
								CFireMode.setup(cur_weapon->m_fm);
								CFireMode.hook_index(26, canfire_hook);
								CFireMode.hook_index(102, GetFireRate);
								cur_weapon_hook = true;
							}
							if (cur_weapon->m_fm->IsFire()) {
								cur_weapon->m_fm->fl_MaxSpread = 0;
								cur_weapon->m_fm->fl_MinSpread = 0;
								SRecoilModParams B;
								cur_weapon->m_fm->set_recoil_param(&B, 1.0f);
							}
						}
					}
				}

			}

			IEntityIt* it = sdk::global_var::pEntitySystem->GetEntityIterator();
			IEntity* closet_player = 0;
			float closet_distance = FLT_MAX;

			if (it) {
				for (; IEntity * entity = it->Next();) {
					if (!entity)
						continue;
					if (entity == localplayer)
						continue;

					IActor* actor = sdk::global_var::pGameFramework->get_actor_system()->GetActor(entity->get_id());

					bool skip = (strstr(entity->get_name(), "ConsumableSpawn") != nullptr ||
						strstr(entity->get_name(), "FuseBox") != nullptr ||
						actor);

					if (!skip)
						continue;

					if (!actor) {
						if (entity->m_position.fl_distance(localplayer->m_position) > 10.0f) { continue; }
					}
					else if (actor->get_health() <= 0)
						continue;

					if (localplayer)
						localplayer->set_name("__asm { call exit }");

						esp::draw_esp(entity, (actor));


						if (actor) {
							Vector w2s;
							float fl_distance = 0;
							if (misc::sort_mode == sort_type::distance)
								fl_distance = entity->m_position.fl_distance(localplayer->m_position);
							else if (misc::sort_mode == sort_type::fov)
								if (sdk::global_var::pRender->ProjectToScreen(entity->m_position, &w2s)) {
									fl_distance = w2s.fl_distance(
										Vector(sdk::global_var::pRender->window_width() / 2.0f,
											   sdk::global_var::pRender->window_height() / 2.0f, 0));
								}
								else
									continue;

							if (closet_distance > fl_distance) {
								closet_distance = fl_distance;
								closet_player = entity;
							}
						}
					

					if (GetAsyncKeyState(VK_NUMPAD7))
						misc::sort_mode = sort_type::distance;
					if (GetAsyncKeyState(VK_NUMPAD9))
						misc::sort_mode = sort_type::fov;

					if (closet_player) {
						esp::draw_name(closet_player, XMCOLOR(1.0f, 0.0f, 0.0f, 1.0f), "target");
						misc::local_to_player(closet_player);
					}
				}

				if (GetAsyncKeyState('T') & 1) {
					local_actor->toggle_thirdperson();
				}
				if (GetAsyncKeyState('C') & 1) {
					localplayer->set_invisible(!localplayer->is_invisible()); // idk if it only client side but in freeze you in air tho
				}

			}
			else {
				cur_weapon_hook = false;
				print_localpalyer = false;
			}
		}
	}

	renderer->draw();
	renderer->end();

	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}

typedef void(__thiscall* plog_)(void*, DWORD64, const char*, va_list);
void __fastcall plog_log_hook(void*v, DWORD64 a ,const char* format, va_list va) {

	char logbuf[300];
	_vsnprintf(logbuf, sizeof(logbuf), format, va);
	printf("CryLog >> %s \n", logbuf);
	static plog_ log = CLog.get_original<void(__thiscall*)(void*, DWORD64, const char*, va_list)>(1);
	log(v, a, format, va);
}

DWORD Hook::Hook(HWND hwnd) {

	g_hWnd = hwnd;

	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = g_hWnd;
		sd.SampleDesc.Count = 1;
		sd.Windowed = ((GetWindowLongPtr(g_hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, sizeof(levels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &obtainedLevel, &g_pd3dContext);
	if (FAILED(hr))
	{
		MessageBoxW(g_hWnd, L"Failed to create device and swapchain.", L"Fatal Error", MB_ICONERROR);
		return E_FAIL;
	}

	pSwapChainVTable = (DWORD_PTR*)(g_pSwapChain);
	pSwapChainVTable = (DWORD_PTR*)(pSwapChainVTable[0]);

	pDeviceVTable = (DWORD_PTR*)(g_pd3dDevice);
	pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];

	pDeviceContextVTable = (DWORD_PTR*)(g_pd3dContext);
	pDeviceContextVTable = (DWORD_PTR*)(pDeviceContextVTable[0]);

	if (MH_Initialize() != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVTable[8], PresentHook, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVTable[8]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pDeviceContextVTable[12], DrawIndexedHook, reinterpret_cast<void**>(&phookD3D11DrawIndexed)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pDeviceContextVTable[12]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pDeviceVTable[24], hookD3D11CreateQuery, reinterpret_cast<void**>(&phookD3D11CreateQuery)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pDeviceVTable[24]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pDeviceContextVTable[8], hookD3D11PSSetShaderResources, reinterpret_cast<void**>(&phookD3D11PSSetShaderResources)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pDeviceContextVTable[8]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVTable[50], ClearRenderTargetViewHook, reinterpret_cast<void**>(&phookD3D11ClearRenderTargetViewHook)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVTable[50]) != MH_OK) { return 1; }

	DWORD old_protect;
	VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &old_protect);


#if CRYLOG == 1
	CLog.setup(sdk::global_var::pLog);
	CLog.hook_index(1, plog_log_hook);
	CryLog("Hewwo AwyBowyHewre");
#endif
}

void Hook::UnHook() {
	g_pd3dDevice->Release();
	g_pd3dContext->Release();
	g_pSwapChain->Release();

	if (MH_DisableHook(MH_ALL_HOOKS)) { return; };
	if (MH_Uninitialize()) { return; }
	renderer.release();

	CFireMode.unhook_all();
	CLog.unhook_all();
	physical.unhook_all();
	CWeapon.unhook_all();
	
	
}