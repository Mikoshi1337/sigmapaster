#include "Utils/Il2cpp/lazy_importer.hpp"
#include <d3d11.h>
#include <D3DX11.h>
//#include "HitSounds.h"
#include "Cheat/Visuals/menu/Elements/images.h"
#include "Cheat/Visuals/menu/Elements/fonts.h"
#include "discord/discord.h"
#pragma comment (lib, "d3dx11.lib")
inline ImDrawList* DrawList;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	g_KeyManager::HandleMessage(uMsg, wParam);

	if (show && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	static auto is_down = true;
	static auto is_clicked = false;

	if (GetAsyncKeyState(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GetAsyncKeyState(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked)
	{
		show = !show;
	}
	auto pressed_buttons = false;
	auto pressed_menu_key = uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_MOUSEWHEEL;

	if (!pressed_menu_key)
		pressed_buttons = true;
	if (!pressed_buttons && show && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(original_windowproc, hWnd, uMsg, wParam, lParam);
}
HRESULT resize_hook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {

	if (render_target_view) render_target_view->Release();

	if (immediate_context) immediate_context->Release();
	if (device) device->Release();

	device = nullptr;

	return resize_original(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
}
HRESULT present_hook(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags) {

	if (!device) {
		swapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
		device->GetImmediateContext(&immediate_context);
		ID3D11Texture2D* renderTarget = nullptr;
		swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
		device->CreateRenderTargetView(renderTarget, nullptr, &render_target_view);
		renderTarget->Release();
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(game_window);
		ImGui_ImplDX11_Init(device, immediate_context);
		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig cfg;
		cfg.FontBuilderFlags != ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_NoHinting;
		ImFontConfig cfg2;
		cfg2.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Bold;
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::StyleColorsDark( );
		

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		ImFont* font = io.Fonts->AddFontFromMemoryTTF(&inter_bold, sizeof inter_bold, 16, NULL, io.Fonts->GetGlyphRangesDefault());
		io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);
		Small_fonts = io.Fonts->AddFontFromMemoryTTF(smallestpixel7, sizeof(smallestpixel7), 35.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());		
		originalsapphire = io.Fonts->AddFontFromMemoryTTF(sapphirefont, sizeof(sapphirefont), 12, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		font::inter_default = io.Fonts->AddFontFromMemoryTTF(inter_medium, sizeof(inter_medium), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ));
		font::inter_bold_esp_only = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ));
		font::inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		font::icon = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ));
		font::load_anim_inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

		D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump { nullptr };
		if ( pic::aimbot == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, aimbot, sizeof(aimbot), &info, pump, &pic::aimbot, 0);
		if ( pic::visuals == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, visuals, sizeof(visuals), &info, pump, &pic::visuals, 0);
		if ( pic::skins == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, Combat, sizeof(Combat), &info, pump, &pic::skins, 0);
		if ( pic::settings == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, settings, sizeof(settings), &info, pump, &pic::settings, 0);
		if ( pic::combo_widget == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, combo_widget, sizeof(combo_widget), &info, pump, &pic::combo_widget, 0);
		if ( pic::input_widget == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, input_widget, sizeof(input_widget), &info, pump, &pic::input_widget, 0);
		if ( pic::circle_success == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, circle_success, sizeof(circle_success), &info, pump, &pic::circle_success, 0);
		if ( pic::circle_error == nullptr ) D3DX11CreateShaderResourceViewFromMemory(device, circle_error, sizeof(circle_error), &info, pump, &pic::circle_error, 0);
		if (pic::menu_settings_icon == nullptr) D3DX11CreateShaderResourceViewFromMemory(device, menu_settings_icon, sizeof(menu_settings_icon), &info, pump, &pic::menu_settings_icon, 0);


		static bool inited = false;

		if (!inited) {
			CreateDirectoryA(("Sapphire.wtf"), NULL);
			CreateDirectoryA(("C:\\System Volume Notify"), NULL);			
			config.Initialize();
			inited = true;
		}
	}

	immediate_context->OMSetRenderTargets(1, &render_target_view, nullptr);
	immediate_context->RSGetViewports(&vps, &viewport);
	screen_size = { viewport.Width, viewport.Height };
	screen_center = { viewport.Width / 2.0f, viewport.Height / 2.0f };
	VarSs::stuff::ScreenHeight = viewport.Height;
	VarSs::stuff::ScreenWidth = viewport.Width;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	DrawList = ImGui::GetBackgroundd1awList();
	g_Render->BeginScene();

	g_Render->EndScene();

	if (show) net_menu();
	

	//setup_circles();	
	//draw_circles_and_lines(ImColor(color_ui[0], color_ui[1], color_ui[2]));
	ImGui::RenderNotifications( );
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (VarSs::stuff::Panic) {
		Discord* g_Discord;
		g_Discord->Stop();
		LocalPlayer::Entity()->unloadadm();
		config.Load("");
		MH_DisableHook(MH_ALL_HOOKS);
	}

	if (VarSs::stuff::hide_me) {
		Discord* g_Discord;
		g_Discord->Stop();
	}

	return present_original(swapChain, SyncInterval, Flags);
}
uintptr_t find_pattern(uintptr_t base, const PBYTE Pattern, const char* Mask) {
	PBYTE Start = (PBYTE)base;
	PIMAGE_NT_HEADERS NTHead = (PIMAGE_NT_HEADERS)(Start + ((PIMAGE_DOS_HEADER)Start)->e_lfanew);
	DWORD Len = NTHead->OptionalHeader.SizeOfImage;

	for (PBYTE region_it = Start; region_it < (Start + Len); ++region_it) {
		if (*region_it == *Pattern) {
			bool found = true;
			const unsigned char* pattern_it = Pattern, * mask_it = (const PBYTE)Mask, * memory_it = region_it;
			for (; *mask_it && (memory_it < (Start + Len)); ++mask_it, ++pattern_it, ++memory_it) {
				if (*mask_it != 'x') continue;
				if (*memory_it != *pattern_it) {
					found = false;
					break;
				}
			}

			if (found)
				return (uintptr_t)region_it;
		}
	}

	return 0;
}


void hk2_(void* Function, void** Original, void* Detour) {
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED) {
		return;
	}
	MH_CreateHook(Function, Detour, Original);
	MH_EnableHook(Function);
}

void Start() {
	InGame::stor::gBase = (DWORD64)GetModuleHandleA(("GameAssembly.dll"));
	InGame::stor::uBase = (DWORD64)GetModuleHandleA(("UnityPlayer.dll"));
	il2cpp::init();

	while (!game_window) {
		EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
			DWORD pid = 0;
			GetWindowThreadProcessId(hWnd, &pid);
			if (pid == GetCurrentProcessId()) {
				*reinterpret_cast<HWND*>(lParam) = hWnd;
				return FALSE;
			}

			return TRUE;
			}, reinterpret_cast<LPARAM>(&game_window));
	}

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	ZeroMemory(&sd, sizeof sd);
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = game_window;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
		MessageBoxW(0, (L"Failed to create D3D11 device and swap chain"), (L"Failure"), MB_ICONERROR);
	}
	else {
		auto table = *reinterpret_cast<PVOID**>(swapChain);
		present_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT)>(table[8]);
		resize_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)>(table[13]);

		context->Release();
		device->Release();
		swapChain->Release();

		original_windowproc = (WNDPROC)SetWindowLongPtrW(game_window, GWLP_WNDPROC, (LONG_PTR)WndProcHook);

		hk2_((void*)table[8], (void**)&present_original, present_hook);
		hk2_((void*)table[13], (void**)&resize_original, resize_hook);
	}
}