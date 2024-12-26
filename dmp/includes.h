#pragma warning(disable : 4530)
#define _CRT_SECURE_NO_WARNINGS
//namespace menu {
//	if (VarSs::stuff::rainbow_ui) {
//
//		static float r = 1.00f, g = 0.00f, b = 1.00f;
//		static int cases = 0;
//		switch (cases)
//		{
//		case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
//		case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
//		case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
//		case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
//		default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
//		}
//
//		static float color_ui[4] = { r, g, b, 255 };
//	}
//}
static float color_ui[4] = { 0, 255, 0, 255 };

#define StrA
#define StrW

#define noauth
//#define disable
#include <stdint.h>
#include <Windows.h>
#include <psapi.h>
#include <d3d11.h>
#include <string>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <emmintrin.h>
#include "winternl.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "Cheat/Hooks/offsets.h"
#include "Utils/Math/crc32.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <thread>
#include <comdef.h>
#include "Cheat/Visuals/menu/imgui/imgui.h"
#include <Lmcons.h>
#include "Utils/Il2cpp/Memory.h"
#include "Utils/Math/structs.h"
#include "Utils/Math/vector.h"
#include "Utils/Math/math.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "Cheat/Visuals/menu/imgui/imgui_freetype.h"
#include "Cheat/Visuals/menu/imgui/imgui_impl_win32.h"
#include "Cheat/Visuals/menu/imgui/imgui_impl_dx11.h"
#include "Cheat/Visuals/menu/imgui/imgui_internal.h"
#include <string_view>
#include <unordered_map>

namespace pic
{
	ID3D11ShaderResourceView* background = nullptr;

	ID3D11ShaderResourceView* aimbot = nullptr;
	ID3D11ShaderResourceView* visuals = nullptr;
	ID3D11ShaderResourceView* skins = nullptr;
	ID3D11ShaderResourceView* settings = nullptr;

	ID3D11ShaderResourceView* combo_widget = nullptr;
	ID3D11ShaderResourceView* input_widget = nullptr;

	ID3D11ShaderResourceView* menu_settings_icon = nullptr;
	ID3D11ShaderResourceView* sapphire_icon = nullptr;

	ID3D11ShaderResourceView* circle_success = nullptr;
	ID3D11ShaderResourceView* circle_error = nullptr;

}

namespace font
{
	ImFont* inter_bold = nullptr;
	ImFont* inter_bold_esp_only = nullptr;
	ImFont* inter_default = nullptr;
	ImFont* icon = nullptr;
	ImFont* load_anim_inter_bold = nullptr;
}
ImFont* originalsapphire;
ImFont* Small_fonts;
ImFont* semi_fonts;
ImFont* Small_;
ImFont* AeroFonts;
ImFont* WurstHack;
ImFont* EasyCheats;
ImFont* AdrenalineFonts;
ImFont* Radianceproject;
ImFont* Eassss;
ImFont* Contrax;
ImFont* Impact;
ImFont* Dissconect;
ImFont* Dark;
ImFont* Blichf;
ImFont* Cubixf;
ImFont* Blasthook;
ImFont* Motolith;
ImFont* Binery;
ImFont* Grathafont;
ImFont* BRLNSRGGFONT;
ImFont* FontAue;
ImFont* Cust;
ImFont* flogs;
ImFont* Icons;

#include "Utils/VarSs.h"
int font_size = 12;
#include "Utils/Adrenaline.h"
#include "Utils/class.h"
#include "Utils/Il2cpp/il2cpp.h"
#include "Cheat/Game/game.h"
#include "Utils/Fonts.h"
#include "Cheat/Visuals/menu/Elements/ImAdd.h"

ID3D11ShaderResourceView* Logotype = nullptr;
static ImVec2 menu_size = ImVec2(495, 540);
static ImVec2 prev_size = ImVec2(200, 350);
HWND game_window;
HRESULT(*present_original)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resize_original)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* immediate_context = nullptr;
ID3D11RenderTargetView* render_target_view = nullptr;
Vector2 screen_size = { 0, 0 };
D3D11_VIEWPORT viewport;
UINT vps = 1;
WNDPROC original_windowproc = nullptr;
namespace keybind {
	ImVec2 pos;
}
namespace desync {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void desync_indicator_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
namespace fly {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void fly_indicator_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
namespace flist {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void flist_tab_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
namespace belt {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void belt_tab_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
namespace wear {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void wear_tab_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
namespace radar {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void radar_tab_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
namespace main {
	Vector2 pos = Vector2(200, 200);
	bool should_drag = false;
	bool should_move = false;

	POINT cursor;
	POINT cursor_corrected;
	void main_tab_mov(Vector2 size) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
			should_drag = true;
			if (!should_move) {
				cursor_corrected.x = cursor.x - pos.x;
				cursor_corrected.y = cursor.y - pos.y;
				should_move = true;
			}
		}

		if (should_drag) {
			pos.x = cursor.x - cursor_corrected.x;
			pos.y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
}
float size_name = 15.f;
#include "Utils/WepaonIcon.h"
#include "Cheat/Render.h"
#include "Cheat/Hooks/minhook/include/MinHook.h"
#include "Cheat/Visuals/Hitmarker/HitMarker.h"
#include "Cheat/Misc/aim.h"
#include "Cheat/Misc/misc.h"
#include "Cheat/Misc/other.hpp"
#include "Cheat/Visuals/esp.h"
#include "Cheat/Hooks/hooks.h"
#include "Cheat/Game/entities.h"
#include "Cheat/Config/Config.h"
#include "Cheat/Visuals/Rendering.h"
#include "Cheat/Visuals/menu/Menu.h"
#include "Startup.h"
#pragma comment(lib, "winmm.lib")