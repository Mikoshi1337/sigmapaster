#pragma warning(disable : 4530)
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <dwmapi.h>
#include <d3d11.h>
#include <stdio.h>
#include <chrono>
#include <psapi.h>
#include <string>
#include <cstringt.h>
#include <atlstr.h>
#include <array>
#include <stdlib.h>
#include <math.h>
#include "winternl.h"
#include "time.h"
#include <map>
#include <cstdint>
#include <stdint.h>
#include <emmintrin.h>
#include <sstream>
#include <cstringt.h>
#include <atlstr.h>
#include <d3d11.h>
#include <D3DX11.h>
#include "menu/imgui/imgui_tricks.hpp"

#pragma comment (lib, "d3dx11.lib")
static void Pulsate(float& clr, float min, float max, float pulsate_speed)
{
	static bool state = false;

	if (!state)
	{
		if (clr < max)
			clr += pulsate_speed;
		else
		{
			clr = max; state = !state;
		};
	}
	else
	{
		if (clr > min)
			clr -= pulsate_speed;
		else
		{
			clr = min; state = !state;
		};
	};
};
class c_texture2
{
public:
	ID3D11ShaderResourceView* texture = nullptr;
	void* item_byte = 0;
	int size = 0;

};
ID3D11ShaderResourceView* Logotype2 = nullptr;

class c_texture_cache2
{
private:
	std::vector<c_texture2> texture_cache{};
	ID3D11ShaderResourceView* create_texture(ID3D11Device* device, LPCVOID byte, int size)
	{

		static ID3D11ShaderResourceView* tex = nullptr;
		D3DX11_IMAGE_LOAD_INFO iInfo;
		ID3DX11ThreadPump* threadPump{ nullptr };
		// From Bytes
		D3DX11CreateShaderResourceViewFromMemory(device, byte, size, &iInfo, threadPump, &tex, 0);
		return tex;
	}
public:
	c_texture2 get_texture(void* byte, int size)
	{
		for (auto texture : texture_cache)
			if (texture.item_byte == byte)
				return texture; // check if the texture's already cached

		c_texture2 ret;
		ret.item_byte = byte;
		ret.texture = create_texture(device, byte, size);

		if (!ret.texture)
			return ret; // this issue sorts itself out

		texture_cache.push_back(ret); // put the new texture into the cache

		return ret; // return the texture

	}
}; inline c_texture_cache2 texture_cache_byte;
class c_texture
{
public:
	ID3D11ShaderResourceView* texture = nullptr;
	std::string item_name = "";

};

class c_texture_cache
{
private:
	std::vector<c_texture> texture_cache{};

	ID3D11ShaderResourceView* create_texture(ID3D11Device* device, std::string name)
	{

		std::string image_path = "C:/Users/refer/Desktop/custom-made-icons/" + name; // todo: make path dynamic
		ID3D11ShaderResourceView* tex = nullptr;
		D3DX11_IMAGE_LOAD_INFO iInfo;
		ID3DX11ThreadPump* threadPump{ nullptr };
		// From Bytes
		D3DX11CreateShaderResourceViewFromFileA(device, image_path.c_str(), &iInfo, threadPump, &tex, 0);


		return tex;

	}

	

public:

	c_texture get_texture(std::string item_name)
	{

		for (auto texture : texture_cache)
			if (texture.item_name == item_name)
				return texture; // check if the texture's already cached

		c_texture ret;
		ret.item_name = item_name;
		ret.texture = create_texture(device, item_name + ".png");

		if (!ret.texture)
			return ret; // this issue sorts itself out

		texture_cache.push_back(ret); // put the new texture into the cache

		return ret; // return the texture

	}

}; inline c_texture_cache texture_cache;
namespace texture {
	enum e_textutre {
		medical_Syringe = 0,
		ak47 = 1 << 1,
		mp5 = 2 << 2,
		boltactionrifle = 3 << 3,
		crossbow = 4 << 4,
		Spas12 = 5 << 5,
		doublebarrel = 6 << 6,
		compundbow = 7 << 7,
		huntingbow = 8 << 8,
		M92 = 9 << 9,
		thompson = 10 << 10,
		Sar_berda = 11 << 11,
		P250 = 12 << 12,
		PumpShotgun = 13 << 13,
		Revolver = 14 << 14,
		Python = 15 << 15,
		hmlmg = 16 << 16,
		eoka = 17 << 17,
		customSmg = 18 << 18,
		LR300 = 19 << 19,
		M249 = 20 << 20,
		m39 = 21 << 21,


	};

}
Vector2 CosTanSinLineH(float flAngle, float range, int x, int y, int LineLength) {
	float oxunem = flAngle;
	oxunem += 45.f;

	float flYaw = (oxunem) * (PI / 180.0);

	float viewcosyawzzzzzzz = cos(flYaw);
	float viewsinyawzzzzzzz = sin(flYaw);

	float x2 = range * (-viewcosyawzzzzzzz) + range * viewsinyawzzzzzzz;
	float y2 = range * (-viewcosyawzzzzzzz) - range * viewsinyawzzzzzzz;

	int posonscreenX = x + int(x2 / range * (LineLength));
	int posonscreenY = y + int(y2 / range * (LineLength));

	return Vector2(posonscreenX, posonscreenY);
}
#include <cstringt.h>
#include <atlstr.h>
#include <list>

namespace otherEsp {
	void miscvis(DWORD64 ObjectClass, char* buff, bool boolean, bool showDistance, float drawDistance, const char* substring, const wchar_t* targettext, ImColor color, c_texture2 texture, ImVec2 icon_size = ImVec2(0,0)) {
		int pos = 0;
		if (boolean) {
			if (strstr(buff, substring)) {
				Vector2 screen_Pos;
				if (ObjectClass) {
					//float health = reinterpret_cast<BaseCombatEntity*>(ObjectClass)->health();
					//float maxhealth = reinterpret_cast<BaseCombatEntity*>(ObjectClass)->maxHealth();
					if (utils::w2s(reinterpret_cast<BaseEntity*>(ObjectClass)->transform()->position(), screen_Pos)) {
						if ((int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), reinterpret_cast<BaseEntity*>(ObjectClass)->transform()->position()) <= drawDistance) {
							std::string _name;
							char buf[256] = { 0 }; sprintf_s(buf, ("%ls"), targettext);
							_name = buf;

							g_Render->DrawTexture(ImVec2(screen_Pos.x, screen_Pos.y + pos), render2::centered_x, icon_size, ImColor(255, 255, 255), reinterpret_cast<void*>(texture.texture));
							pos += icon_size.y;

							std::string string; char buf2[256] = { 0 };
							sprintf_s(buf2, ("%dm"), (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), reinterpret_cast<BaseEntity*>(ObjectClass)->transform()->position()));
							string += buf2; std::string name_distance = _name;
							if (showDistance) name_distance = _name + ("(") + string + (")"); else name_distance = _name;

							if (showDistance)
								g_Render->DrawString(screen_Pos.x, screen_Pos.y + pos, color, render2::outline | render2::centered_x, 2, 10, string.c_str());
						}
					}
				}
			}
		}
	}
	void miscvis(DWORD64 ObjectClass, char* buff, bool boolean, bool showDistance, float drawDistance, const char* substring, const wchar_t* targettext, ImColor color) {
		int pos = 0;
		if (boolean) {
			if (strstr(buff, substring)) {
				Vector2 screen_Pos;
				if (ObjectClass) {
					if (utils::w2s(reinterpret_cast<BaseEntity*>(ObjectClass)->transform()->position(), screen_Pos)) {
						if ((int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), reinterpret_cast<BaseEntity*>(ObjectClass)->transform()->position()) <= drawDistance) {
							std::string _name;
							char buf[256] = { 0 }; sprintf_s(buf, ("%ls"), targettext);
							_name = buf;

							std::string string; char buf2[256] = { 0 };
							sprintf_s(buf2, ("%dm"), (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), reinterpret_cast<BaseEntity*>(ObjectClass)->transform()->position()));
							string += buf2; std::string name_distance = _name;

							if (showDistance) name_distance = _name + ("(") + string + (")"); else name_distance = _name;
							g_Render->DrawString(screen_Pos.x, screen_Pos.y, color, render2::outline | render2::centered_x, 2, 10, name_distance.c_str());
						}
					}
				}
			}
		}
	}
	void bradley_( DWORD64 Object, char* buff) {
		if (VarSs::visuals::bradley_apc) {
			if (strstr(buff, ("bradleyapc.prefab"))) {

				BaseCombatEntity* bradley = (BaseCombatEntity*)Object;
				if (bradley) {
					Vector3 pos = bradley->transform()->position();/*utils::GetEntityPosition(object);*/
					int distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
					float health = bradley->health();
					Vector2 screen;
					if (utils::w2s(pos, screen)) {
						if (health > 0) {
							std::string string;
							char buf[256] = { 0 }; sprintf_s(buf,("Bradley[%dm]"),(int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos));
							string = buf;
							int pos = 0;
							//g_Render->DrawTexture(ImVec2(screen.x, screen.y + pos), render2::centered_x, ImVec2(20, 11), ImColor(255, 255, 255), reinterpret_cast<void*>(texture_cache_byte.get_texture(bradleyvehicle, sizeof(bradleyvehicle)).texture));
							pos += 11;
							g_Render->DrawString(screen.x, screen.y + pos, ImColor(255, 0, 0, 255), render2::outline | render2::centered_x, 2, 10, string.c_str());
				
							g_Render->FilledRect(screen.x - 30, screen.y - 15, 60, 6, ImColor(0, 0, 0), 0, 0);
							g_Render->FilledRect(screen.x - 30, screen.y - 15, 60 * (health / 1000.f), 6, ImColor(0,255,0), 0, 0);
							g_Render->Rect(screen.x - 30, screen.y - 15, 60, 6, ImColor(0, 0, 0), 0, 0);
						}
					}
				}
			}
		}
	}
	void corpse( DWORD64 Object, char* buff) {
		if (VarSs::visuals::other::corpse) {
			if (strstr(buff, ("player_corpse.prefab"))) {

				BaseEntity* corpse = (BaseEntity*)Object;
				if (corpse) {
					Vector3 pos = corpse->transform()->position();
					float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
					Vector2 screen;
					if (utils::w2s(pos, screen) && distance <= VarSs::visuals::other::draw_distance) {
						std::string string2;
						char buf2[256] = { 0 };
						if (VarSs::visuals::other::show_distance)
							sprintf_s(buf2, ("Corpse(%dm)"), (int)distance);
						else
							sprintf_s(buf2, ("Corpse"));
						string2 = buf2;
						g_Render->DrawString(screen.x, screen.y, ImColor(255, 0, 0, 255), render2::outline | render2::centered_x, 2, 10, string2.c_str());
					}
				}
			}
		}
	}
	void tc( DWORD64 Object, char* buff) {
		if (VarSs::visuals::base::tc) {
			if (strstr(buff, ("cupboard.tool.deployed.prefab"))) {

				BaseEntity* cupboard = (BaseEntity*)Object;
				if (cupboard) {
					Vector3 pos = cupboard->transform()->position();
					float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
					Vector2 screen;
					if (utils::w2s(pos, screen) && distance <= VarSs::visuals::base::draw_distance) {
						int pos = 15;
						int upkeep = read(cupboard + 0x598, float); // private float cachedProtectedMinutes;
						std::string string;
						char buf[256] = { 0 }; sprintf_s(buf,("Tool Cupboard | %d hours"),upkeep / 60);
						string = buf;
						g_Render->DrawString(screen.x, screen.y , ImColor(0, 255, 0, 255), render2::outline | render2::centered_x, 2, 10, string.c_str());

						if (VarSs::visuals::base::show_distance) {
							std::string string2;
							char buf2[256] = { 0 }; sprintf_s(buf2,("%dm"),(int)distance);
							string2 = buf2;
	
							g_Render->DrawString(screen.x, screen.y + 10, ImColor(128, 128, 128), render2::outline | render2::centered_x, 2, 10, string2.c_str());

							pos += 15;
						}
					}
				}
			}
		}
	}
	void sleepingbag( DWORD64 Object, char* buff) {
		if (VarSs::visuals::base::sleeping_bag) {
			if (strstr(buff, ("sleepingbag_leather_deployed.prefab"))) {

				BaseEntity* bag = (BaseEntity*)Object;
				if (bag) {
					Vector3 pos = bag->transform()->position();
					float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
					Vector2 screen;
					if (utils::w2s(pos, screen) && distance <= VarSs::visuals::base::draw_distance) {
						std::string string2;
						char buf2[256] = { 0 };
						if (VarSs::visuals::base::show_distance)
							sprintf_s(buf2, ("Sleeping Bag(%dm)"), (int)distance);
						else
							sprintf_s(buf2, ("Sleeping Bag"));
						string2 = buf2;
						g_Render->DrawString(screen.x, screen.y, ImColor(107, 142, 35), render2::outline | render2::centered_x, 2, 10, string2.c_str());
					}
				}
			}
		}
	}
	void bed( DWORD64 Object, char* buff) {
		if (VarSs::visuals::base::bed) {
			if (strstr(buff, ("bed_deployed.prefab"))) {

				BaseEntity* bag = (BaseEntity*)Object;
				if (bag) {
					Vector3 pos = bag->transform()->position();
					float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
					Vector2 screen;
					if (utils::w2s(pos, screen) && distance <= VarSs::visuals::base::draw_distance) {

						std::string string2;
						char buf2[256] = { 0 };
						if (VarSs::visuals::base::show_distance)
							sprintf_s(buf2, ("Bed(%dm)"), (int)distance);
						else
							sprintf_s(buf2, ("Bed"));
						string2 = buf2;

						g_Render->DrawString(screen.x, screen.y , ImColor(107, 142, 35), render2::outline | render2::centered_x, 2, 10, string2.c_str());
					}
				}
			}
		}
	}
	//void stash( DWORD64 Object, char* buff) {
	//	typedef bool(__stdcall* IsHidden)(BaseEntity*);
	//	if (VarSs::visuals::other::stash) {
	//		if (strstr(buff, ("small_stash_deployed.prefab"))) {

	//			BaseEntity* stash = (BaseEntity*)Object;
	//			if (stash) {
	//				Vector3 pos = stash->transform()->position();
	//				float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
	//				Vector2 screen;
	//				if (utils::w2s(pos, screen) && distance <= VarSs::visuals::other::draw_distance) {
	//					int pos = 0;
	//					//g_Render->DrawTexture(ImVec2(screen.x, screen.y), render2::centered_x, ImVec2(15, 12), ImColor(255, 255, 255), reinterpret_cast<void*>(texture_cache_byte.get_texture(toolcrate, sizeof(toolcrate)).texture));
	//					pos += 11;
	//					if (((IsHidden)(InGame::stor::gBase + CO::IsHidden))(stash)) {
	//						g_Render->DrawString(screen.x, screen.y+ pos, ImColor(255, 0, 0), render2::outline | render2::centered_x, 2, 10, ("Stash [Hidden]"));

	//					}
	//					else {
	//						
	//						g_Render->DrawString(screen.x, screen.y + pos, ImColor(255, 255, 0), render2::outline | render2::centered_x, 2, 10, ("Stash [Open]"));

	//					}
	//					if (VarSs::visuals::other::show_distance) {
	//						std::string string;
	//						char buf[256] = { 0 }; sprintf_s(buf,("%dm"),(int)distance);
	//						string = buf;
	//						g_Render->DrawString(screen.x, screen.y + 12 + pos, ImColor(128, 128, 128), render2::outline | render2::centered_x, 2, 10, string.c_str());
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//void hackablecrate( DWORD64 Object, char* buff) {
	//	typedef bool(__stdcall* IsBeingHacked)(BaseEntity*);
	//	typedef bool(__stdcall* IsFullyHacked)(BaseEntity*);
	//	if (VarSs::visuals::crates::chinook) {
	//		if (strstr(buff, ("codelockedhackablecrate.prefab"))) {

	//			BaseEntity* crate = (BaseEntity*)Object;
	//			if (crate) {
	//				Vector3 pos = crate->transform()->position();
	//				float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
	//				Vector2 screen;
	//				if (utils::w2s(pos, screen)) {
	//					uintptr_t timerText = read(crate + 0x470, uintptr_t); // public Text timerText; || public class HackableLockedCrate
	//					auto* m_Text = reinterpret_cast<pUncStr>(read(timerText + 0xD0, DWORD64));
	//					ImColor color;
	//					int pos = 0;
	//					g_Render->DrawTexture(ImVec2(screen.x, screen.y), render2::centered_x, ImVec2(11, 15), ImColor(255, 255, 255), reinterpret_cast<void*>(texture_cache_byte.get_texture(lockedcrate, sizeof(lockedcrate)).texture));
	//					pos += 11;
	//					std::string _name((LPCSTR)CStringA(m_Text->str));
	//					if (!((IsFullyHacked)(InGame::stor::gBase + CO::IsFullyHacked))(crate)) {
	//						if (((IsBeingHacked)(InGame::stor::gBase + CO::IsBeingHacked))(crate)) {
	//							color = ImColor(250, 250, 210);
	//
	//							std::string chinook_name = ("Chinook Crate | ") + _name;

	//							g_Render->DrawString(screen.x, screen.y+ pos, color, render2::outline | render2::centered_x, 2, 10, chinook_name.c_str());

	//						}
	//						else {
	//							color = ImColor(255, 69, 0);

	//							g_Render->DrawString(screen.x, screen.y + pos, color, render2::outline | render2::centered_x, 2, 10, ("Chinook Crate"));

	//						}
	//					}
	//					else {
	//						color = ImColor(32, 178, 170);
	//						g_Render->DrawString(screen.x, screen.y + pos, color, render2::outline | render2::centered_x, 2, 10, ("Chinook Crate [Open]"));

	//					}
	//					std::string string;
	//					char buf[256] = { 0 }; sprintf_s(buf,("%dm"),(int)distance);
	//					string = buf;
	//					g_Render->DrawString(screen.x, screen.y + 10+ pos, ImColor(128, 128, 128), render2::outline | render2::centered_x, 2, 10, string.c_str());
	//				}
	//			}
	//		}
	//	}
	//}
	void world( DWORD64 Object, char* buff) {
		typedef uintptr_t(__stdcall* GetItem)(uintptr_t);
		if (VarSs::visuals::other::dropped_items) {
			if (strstr(buff, ("world"))) {

				BaseEntity* item = (BaseEntity*)Object;
				if (item) {
					Vector3 pos = item->transform()->position();
					float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos);
					Vector2 screen;
					if (utils::w2s(pos, screen) && distance <= VarSs::visuals::other::draw_distance) {
						uintptr_t Item = read(item + 0x180, uintptr_t); // public Item item; || public class WorldItem : BaseEntity
						uintptr_t itemDefinition = read(Item + 0x18, uintptr_t);
						uintptr_t displayName = read(itemDefinition + 0x28, uintptr_t);
						auto* english = reinterpret_cast<pUncStr>(read(displayName + 0x18, DWORD64)); // get name of item

						std::wstring eng = std::wstring(english->str);
						if (!(eng.find((L"Arrow")) != std::wstring::npos) && !(eng.find((L"Nail")) != std::wstring::npos)) {
							std::string string2;
							char buf2[256] = { 0 };
							if (VarSs::visuals::other::show_distance)
								sprintf_s(buf2, ("%ls(%dm)"), english->str,(int)distance);
							else
								sprintf_s(buf2, ("%ls"), english->str);
							string2 = buf2;
							g_Render->DrawString(screen.x, screen.y, ImColor(VarSs::colors::WorldEspColor.x, VarSs::colors::WorldEspColor.y, VarSs::colors::WorldEspColor.z, 255.f), render2::outline | render2::centered_x, 2, 10, string2.c_str());
						}
					}
				}
			}
		}
	}
}

class CornerBox
{
public:
	Vector3 left;
	Vector3 right;
	Vector3 top;
	Vector3 bottom;
	bool Valid = true;
	ImColor Color;
};

void Skeleton(BasePlayer* player, ImColor color) {
	if (VarSs::players::sleeperignore && player->HasPlayerFlag(PlayerFlags::Sleeping)) return;

	auto head_pos = player->get_bone_pos(head);
	auto spine4_pos = player->get_bone_pos(spine4);
	auto l_clavicle_pos = player->get_bone_pos(l_clavicle);
	auto l_upperarm_pos = player->get_bone_pos(l_upperarm);
	auto l_forearm_pos = player->get_bone_pos(l_forearm);
	auto l_hand_pos = player->get_bone_pos(l_hand);
	auto r_clavicle_pos = player->get_bone_pos(r_clavicle);
	auto r_upperarm_pos = player->get_bone_pos(r_upperarm);
	auto r_forearm_pos = player->get_bone_pos(r_forearm);
	auto r_hand_pos = player->get_bone_pos(r_hand);
	auto pelvis_pos = player->get_bone_pos(pelvis);
	auto l_hip_pos = player->get_bone_pos(l_hip);
	auto l_knee_pos = player->get_bone_pos(l_knee);
	auto l_ankle_scale_pos = player->get_bone_pos(l_ankle_scale);
	auto l_foot_pos = player->get_bone_pos(l_foot);
	auto r_hip_pos = player->get_bone_pos(r_hip);
	auto r_knee_pos = player->get_bone_pos(r_knee);
	auto r_ankle_scale_pos = player->get_bone_pos(r_ankle_scale);
	auto r_foot_pos = player->get_bone_pos(r_foot);
	Vector2 head, spine, l_clavicle, l_upperarm, l_forearm, l_hand, r_clavicle, r_upperarm, r_forearm, r_hand, pelvis, l_hip, l_knee, l_ankle_scale, l_foot, r_hip, r_knee, r_ankle_scale, r_foot;
	if (utils::w2s(head_pos, head) && utils::w2s(spine4_pos, spine) && utils::w2s(l_clavicle_pos, l_clavicle) && utils::w2s(l_upperarm_pos, l_upperarm) && utils::w2s(l_forearm_pos, l_forearm) && utils::w2s(l_hand_pos, l_hand) && utils::w2s(r_clavicle_pos, r_clavicle) && utils::w2s(r_upperarm_pos, r_upperarm) && utils::w2s(r_forearm_pos, r_forearm) && utils::w2s(r_hand_pos, r_hand) && utils::w2s(pelvis_pos, pelvis) && utils::w2s(l_hip_pos, l_hip) && utils::w2s(l_knee_pos, l_knee) && utils::w2s(l_ankle_scale_pos, l_ankle_scale) && utils::w2s(l_foot_pos, l_foot) && utils::w2s(r_hip_pos, r_hip) && utils::w2s(r_knee_pos, r_knee) && utils::w2s(r_ankle_scale_pos, r_ankle_scale) && utils::w2s(r_foot_pos, r_foot)) {
		g_Render->DrawLine(head.x, head.y, spine.x, spine.y,VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f,VarSs::players::skeleton_::outline);
		g_Render->DrawLine(spine.x, spine.y, l_upperarm.x, l_upperarm.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(l_upperarm.x, l_upperarm.y, l_forearm.x, l_forearm.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(l_forearm.x, l_forearm.y, l_hand.x, l_hand.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(spine.x, spine.y, r_upperarm.x, r_upperarm.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(r_upperarm.x, r_upperarm.y, r_forearm.x, r_forearm.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(r_forearm.x, r_forearm.y, r_hand.x, r_hand.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(spine.x, spine.y, pelvis.x, pelvis.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(pelvis.x, pelvis.y, l_hip.x, l_hip.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(l_hip.x, l_hip.y, l_knee.x, l_knee.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(l_knee.x, l_knee.y, l_foot.x, l_foot.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(pelvis.x, pelvis.y, r_hip.x, r_hip.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(r_hip.x, r_hip.y, r_knee.x, r_knee.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
		g_Render->DrawLine(r_knee.x, r_knee.y, r_foot.x, r_foot.y, VarSs::players::skeleton_::colorcheck ? color : VarSs::players::skeleton_::color, 1.5f, VarSs::players::skeleton_::outline);
	}
}
//void Radar() {
//	if (VarSs::players::radar)
//	{
//		radar::radar_tab_mov(Vector2(100.f, 100.f));
//		g_Render->CircleFilled(radar::pos.x, radar::pos.y, 100.f, ImColor(17, 17, 19, 200), 0);
//		g_Render->DrawCircle(radar::pos.x, radar::pos.y, 100.f, 2.0f, ImColor(200, 200, 200, 255));
//		g_Render->DrawCircle(radar::pos.x, radar::pos.y, 75.f, 2.0f, ImColor(200, 200, 200, 255));
//		g_Render->DrawCircle(radar::pos.x, radar::pos.y, 35.f, 2.0f, ImColor(200, 200, 200, 255));
//		g_Render->CircleFilled(radar::pos.x, radar::pos.y, 1.f, ImColor(200, 200, 200, 255), 0);
//
//		if (Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) > 350) {
//			if (LocalPlayer::Entity()->IsPlayer()) {
//				g_Render->CircleFilled(5.f, 5.f, 5.f, ImColor(255, 0, 0), 0);
//			}
//		}	
//	}
//}
void OOF(BasePlayer* plly, ImColor _color) {	

	if (VarSs::players::sleeperignore && plly->HasPlayerFlag(PlayerFlags::Sleeping)) return;
	if (!VarSs::npc::oof_arrows && plly->playerModel()->isNpc()) return; if (!plly) return;
	if (VarSs::players::oof_arrows) {
		if (utils::OOF(plly)) {
			Vector3 local = LocalPlayer::Entity()->get_bone_pos(head);
			Vector3 cordi = plly->get_bone_pos(head);
			float y = local.x - cordi.x;
			float x = local.z - cordi.z;
			Vector3 eulerAngles = Math::EulerAngles(LocalPlayer::Entity()->eyes()->get_rotation());
			float angleYawRad = atan2(y, x) * 57.29578f - 180.f - eulerAngles.y;
			Item* ActWeapon = plly->GetHeldItem();
			std::string _name(CStringA(plly->_displayName()));
			std::string string;	char buf[256] = { 0 };
			sprintf_s(buf, ("%ls"), ActWeapon->infom_());
			string += buf;
			std::string string1; char buf1[256] = { 0 };
			sprintf_s(buf1, ("Distance [ %dm ]"), (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), plly->get_bone_pos(head)));
			string1 += buf1;
			Vector2 tp0 = CosTanSinLineH(angleYawRad, 5.f, VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2, 270.f);
			Vector2 tp1 = CosTanSinLineH(angleYawRad + 2.f, 5.f, VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2, 260.f);
			Vector2 tp2 = CosTanSinLineH(angleYawRad - 2.f, 5.f, VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2, 260.f);
			if (VarSs::players::out_type == 0) {
				g_Render->Triangle(tp0.x, tp0.y, tp1.x, tp1.y, tp2.x, tp2.y, ImColor(VarSs::colors::off_color[0], VarSs::colors::off_color[1], VarSs::colors::off_color[2]), 2);
			}			
			if (VarSs::players::out_type == 1) {
				g_Render->TriangleFilled(tp0.x, tp0.y, tp1.x, tp1.y, tp2.x, tp2.y, ImColor(VarSs::colors::off_color[0], VarSs::colors::off_color[1], VarSs::colors::off_color[2]));
			}
		}
	}
}

class c_getitemicon
{
public:
	ImVec2 size = ImVec2(0,0);
	std::string name = "";
	c_texture2 texture;

}; inline c_getitemicon getitem;
c_getitemicon get_icon_name(BasePlayer* player) {
	c_getitemicon item;
	Item* ActWeapon = player->GetHeldItem();
	switch (ActWeapon->info()->itemid())
	{
	case 1079279582:

		item.texture = texture_cache_byte.get_texture(medicalsyringe, sizeof(medicalsyringe));
		item.name = ("medical-syringe");
		item.size = ImVec2(30, 30);
		break;
	case -2072273936:

		item.texture = texture_cache_byte.get_texture(bandage, sizeof(bandage));
		item.name = ("bandage");
		item.size = ImVec2(30, 30);
		break;
	case 254522515:

		item.texture = texture_cache_byte.get_texture(heal , sizeof(heal));
		item.name = ("largemedkit");
		item.size = ImVec2(30, 30);
		break;
	case 1540934679:

		item.texture = texture_cache_byte.get_texture(spwooden, sizeof(spwooden));
		item.name = ("spear.wooden");
		item.size = ImVec2(30, 30);
		break;
	case 1602646136:

		item.texture = texture_cache_byte.get_texture(spstone, sizeof(spstone));
		item.name = ("spear.stone");
		item.size = ImVec2(30, 30);
		break;
	case 1545779598:
		item.texture = texture_cache_byte.get_texture(ak47, sizeof(ak47));
		item.name = ("ak47");
		item.size = ImVec2(30, 30);
		break;
	case -1335497659:
		item.texture = texture_cache_byte.get_texture(ak47, sizeof(ak47));
		item.name = ("ak47");
		item.size = ImVec2(30, 30);
		break;
	case 1318558775:

		item.texture = texture_cache_byte.get_texture(mp5, sizeof(mp5));
		item.name = ("mp5");
		item.size = ImVec2(30, 30);
		break;
	case 963906841:

		item.texture = texture_cache_byte.get_texture(rock, sizeof(rock));
		item.name = ("rock");
		item.size = ImVec2(30, 30);
		break;
	case 1588298435:
		item.texture = texture_cache_byte.get_texture(boltactionrifle, sizeof(boltactionrifle));
		item.name = ("bolt-action-rifle");
		item.size = ImVec2(30, 30);
		break;
	case 143803535:
		item.texture = texture_cache_byte.get_texture(f1, sizeof(f1));
		item.name = ("grenade.f1");
		item.size = ImVec2(30, 30);
		break;
	case 1840822026:
		item.texture = texture_cache_byte.get_texture(bean, sizeof(bean));
		item.name = ("grenade.beancan");
		item.size = ImVec2(30, 30);
		break;
	case 1965232394:
		item.texture = texture_cache_byte.get_texture(crossbow, sizeof(crossbow));
		item.name = ("crossbow");
		item.size = ImVec2(30, 30);
		break;
	case -41440462:
		item.texture = texture_cache_byte.get_texture(spas12, sizeof(spas12));
		item.name = ("spas-12");
		item.size = ImVec2(30, 30);
		break;
	case -765183617:
		item.texture = texture_cache_byte.get_texture(doublebarrel, sizeof(doublebarrel));
		item.name = ("double-barrel");
		item.size = ImVec2(30, 30);
		break;
	case 884424049:
		item.texture = texture_cache_byte.get_texture(compoundbow, sizeof(compoundbow));
		item.name = ("compound-bow");
		item.size = ImVec2(30, 30);
		break;
	case 1443579727:
		item.texture = texture_cache_byte.get_texture(huntingbow, sizeof(huntingbow));
		item.name = ("hunting-bow");
		item.size = ImVec2(30, 30);
		break;
	case -852563019:
		item.texture = texture_cache_byte.get_texture(m92, sizeof(m92));
		item.name = ("m92");
		item.size = ImVec2(30, 30);
		break;
	case -1758372725:
		item.texture = texture_cache_byte.get_texture(thompson, sizeof(thompson));
		item.name = ("thompson");
		item.size = ImVec2(30, 30);
		break;
	case -904863145:
		item.texture = texture_cache_byte.get_texture(sar, sizeof(sar));
		item.name = ("sar");
		item.size = ImVec2(30, 30);
		break;
	case 818877484:
		item.texture = texture_cache_byte.get_texture(p250, sizeof(p250));
		item.name = ("p250");
		item.size = ImVec2(30, 30);
		break;
	case 795371088:
		item.texture = texture_cache_byte.get_texture(pumpshotgun, sizeof(pumpshotgun));
		item.name = ("pump-shotgun");
		item.size = ImVec2(30, 30);
		break;
	case 649912614:
		item.texture = texture_cache_byte.get_texture(revolver, sizeof(revolver));
		item.name = ("revolver");
		item.size = ImVec2(30, 30);
		break;
	case 1373971859:
		item.texture = texture_cache_byte.get_texture(python, sizeof(python));
		item.name = ("python");
		item.size = ImVec2(30, 30);
		break;
	case -1214542497:
		item.texture = texture_cache_byte.get_texture(hmlmg, sizeof(hmlmg));
		item.name = ("hmlmg");
		item.size = ImVec2(30, 30);
		break;
	case -75944661:
		item.texture = texture_cache_byte.get_texture(eoka, sizeof(eoka));
		item.name = ("eoka");
		item.size = ImVec2(30, 30);
		break;
	case 1953903201:
		item.texture = texture_cache_byte.get_texture(nailgun, sizeof(nailgun));
		item.name = ("pistol.nailgun");
		item.size = ImVec2(30, 30);
		break;
	case -1367281941:
		item.texture = texture_cache_byte.get_texture(waterpipe, sizeof(waterpipe));
		item.name = ("shotgun.waterpipe");
		item.size = ImVec2(30, 30);
		break;
	case 1248356124:
		item.texture = texture_cache_byte.get_texture(c4, sizeof(c4));
		item.name = ("explosive.timed");
		item.size = ImVec2(30, 30);
		break;
	case -1878475007:
		item.texture = texture_cache_byte.get_texture(satchel, sizeof(satchel));
		item.name = ("explosive.satchel");
		item.size = ImVec2(30, 30);
		break;
	case 442886268:
		item.texture = texture_cache_byte.get_texture(rocket_lanucher, sizeof(rocket_lanucher));
		item.name = ("rocket.launcher");
		item.size = ImVec2(30, 30);
		break;
	case 1796682209:
		item.texture = texture_cache_byte.get_texture(customsmg, sizeof(customsmg));
		item.name = ("custom-smg");
		item.size = ImVec2(30, 30);
		break;
	case -1812555177:
		item.texture = texture_cache_byte.get_texture(lr300, sizeof(lr300));
		item.name = ("lr300");
		item.size = ImVec2(30, 30);
		break;
	case -2069578888:
		item.texture = texture_cache_byte.get_texture(m249, sizeof(m249));
		item.name = ("m249");
		item.size = ImVec2(30, 30);
		break;
	case 28201841:
		item.texture = texture_cache_byte.get_texture(m39, sizeof(m39));
		item.name = ("m39");
		item.size = ImVec2(30, 30);
		break;
	}
	return item;
}
void CornerBox(float Entity_x, float Entity_y, float Entity_w, float Entity_h, ImColor color) {

	g_Render->DrawLine(Entity_x, Entity_y, Entity_x + Entity_w / 3.5f, Entity_y, color, 1.2f, true);
	g_Render->DrawLine(Entity_x, Entity_y, Entity_x, Entity_y + Entity_h / 3.5f, color, 1.2f, true);

	g_Render->DrawLine(Entity_x + Entity_w, Entity_y, (Entity_x + Entity_w) - Entity_w / 3.5f, Entity_y, color, 1.2f, true);
	g_Render->DrawLine(Entity_x + Entity_w, Entity_y, Entity_x + Entity_w, Entity_y + Entity_h / 3.5f, color, 1.2f, true);

	g_Render->DrawLine(Entity_x + Entity_w, Entity_y + Entity_h, (Entity_x + Entity_w) - Entity_w / 3.5f, Entity_y + Entity_h, color, 1.2f, true);
	g_Render->DrawLine(Entity_x + Entity_w, Entity_y + Entity_h, Entity_x + Entity_w, (Entity_y + Entity_h) - Entity_h / 3.5f, color, 1.2f, true);

	g_Render->DrawLine(Entity_x, Entity_y + Entity_h, Entity_x + Entity_w / 3.5f, Entity_y + Entity_h, color, 1.2f, true);

	g_Render->DrawLine(Entity_x, Entity_y + Entity_h, Entity_x, (Entity_y + Entity_h) - Entity_h / 3.5f, color, 1.2f, true);
}

//void PlayerChams(BasePlayer* player, bool draw = true)
//{
//	bool PlayerSleeping = player->get_flag(16);
//	if (VarSs::players::sleeperignore && PlayerSleeping) return;
//	if (draw) {
//		if (VarSs::players::player_chams) {
//			static float r = 1.00f, g = 0.00f, b = 1.00f;
//			static int cases = 0;
//			switch (cases) {
//			case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
//			case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
//			case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
//			case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
//			default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
//			}
//			/*auto _multiMesh = player->get_player_model2()->_multiMesh();*/
//			auto _multiMesh = player->playerModel()->_multiMesh();
//			if (_multiMesh) {
//				auto render = _multiMesh->Renderers();
//				for (int i = 0; i < render->get_size(); i++) {
//					auto renderer = render->get_value(i);
//					if (renderer) {
//						Material* material = renderer->GetMaterial();
//						if (!material) continue;
//
//						if (VarSs::players::player_chams_type == 0) {
//							material->SetShader(Flat);
//							material->SetColor((L"_ColorVisible"), col(VarSs::colors::vis_fl_pl.x, VarSs::colors::vis_fl_pl.y, VarSs::colors::vis_fl_pl.z, 1));
//							material->SetColor((L"_ColorBehind"), col(VarSs::colors::beh_fl_pl.x, VarSs::colors::beh_fl_pl.y, VarSs::colors::beh_fl_pl.z, 1));
//						}
//						else if (VarSs::players::player_chams_type == 1) {
//							material->SetShader(RainbowFrame);
//							material->SetColor((L"_WireColor"), col(VarSs::colors::wire_pl.x, VarSs::colors::wire_pl.y, VarSs::colors::wire_pl.z, 1));
//							material->SetColor((L"_BaseColor"), col(VarSs::colors::wire_pl.x, VarSs::colors::wire_pl.y, VarSs::colors::wire_pl.z, 1));
//							if (VarSs::players::wire_rainbow2) {
//								material->SetShader(RainbowFrame);
//								material->SetColor((L"_WireColor"), col(r, g, b, 1));
//								material->SetColor((L"_BaseColor"), col(r, g, b, 1));
//							}
//						}
//						else if (VarSs::players::player_chams_type == 2) {
//							material->SetShader(Glow);							
//							material->SetColor((L"_ColorVisible"), col(VarSs::colors::vis_pl2.x, VarSs::colors::vis_pl2.y, VarSs::colors::vis_pl2.z, 1));
//							material->SetColor((L"_ColorBehind"), col(VarSs::colors::beh_pl2.x, VarSs::colors::beh_pl2.y, VarSs::colors::beh_pl2.z, 1));
//						}
//						else if (VarSs::players::player_chams_type == 3) {
//							material->SetShader(SetTrought);
//						}
//					}
//				}
//			}
//		}
//	}
//}

void ESP(BasePlayer* BP, const ImColor color) {
	if (VarSs::players::sleeperignore && BP->HasPlayerFlag(PlayerFlags::Sleeping)) return;
	if (!BP) return;
	

	//PlayerChams(BP);
	//lol::update_chams();
	//Radar();

	Vector2 tempFeetR, tempFeetL;
	if (utils::w2s(BP->get_bone_pos(r_foot), tempFeetR) && utils::w2s(BP->get_bone_pos(l_foot), tempFeetL)) {
		if (tempFeetR.x == 0 && tempFeetR.y == 0) return;

		Vector2 tempHead;


		if (utils::w2s(BP->get_bone_pos(head) + Vector3(0, 0.2f, 0), tempHead)) {
			Vector2 tempFeet = (tempFeetR + tempFeetL) / 2.f;
			float Entity_h = tempHead.y - tempFeet.y;
			float w = Entity_h / 4;			
			float Entity_x = tempFeet.x - w;
			float Entity_y = tempFeet.y;
			float Entity_w = Entity_h / 2;
			Vector3 middlePointWorld = BP->get_bone_pos(l_foot).midPoint(BP->get_bone_pos(r_foot));
			Vector3 middlePointWorld_ = BP->get_bone_pos(head).midPoint(BP->get_bone_pos(head));

				Vector3 BodyForward = BP->eyes()->BodyForward();
								
				if (VarSs::players::box_rotate) {
					CBounds bounds = CBounds();

					if (BP->IsDucked()) {
						bounds.center = BP->get_bone_pos(l_foot).midPoint(BP->get_bone_pos(r_foot)) + Vector3(0.0f, 0.55f, 0.0f);
						bounds.extents = Vector3(0.4f, 0.65f, 0.f);
					}
					else {
						if (BP->HasPlayerFlag(PlayerFlags::Wounded) || BP->HasPlayerFlag(PlayerFlags::Sleeping)) {
							bounds.center = BP->get_bone_pos(pelvis);
							bounds.extents = Vector3(0.9f, 0.2f, 0.f);
						}
						else {
							bounds.center = BP->get_bone_pos(l_foot).midPoint(BP->get_bone_pos(r_foot)) + Vector3(0.0f, 0.85f, 0.0f);
							bounds.extents = Vector3(0.4f, 0.9f, 0.f);
						}
					}

					float y = Math::EulerAngles(BP->eyes()->get_rotation()).y;
					Vector3 center = bounds.center;
					Vector3 extents = bounds.extents;
					Vector3 frontTopLeft = Math::RotatePoint(center, Vector3(center.x - extents.x, center.y + extents.y, center.z - extents.z), y);
					Vector3 frontTopRight = Math::RotatePoint(center, Vector3(center.x + extents.x, center.y + extents.y, center.z - extents.z), y);
					Vector3 frontBottomLeft = Math::RotatePoint(center, Vector3(center.x - extents.x, center.y - extents.y, center.z - extents.z), y);
					Vector3 frontBottomRight = Math::RotatePoint(center, Vector3(center.x + extents.x, center.y - extents.y, center.z - extents.z), y);
					Vector3 backTopLeft = Math::RotatePoint( center, Vector3( center.x - extents.x, center.y + extents.y, center.z + extents.z ), y );
					Vector3 backTopRight = Math::RotatePoint( center, Vector3( center.x + extents.x, center.y + extents.y, center.z + extents.z ), y );
					Vector3 backBottomLeft = Math::RotatePoint( center, Vector3( center.x - extents.x, center.y - extents.y, center.z + extents.z ), y );
					Vector3 backBottomRight = Math::RotatePoint( center, Vector3( center.x + extents.x, center.y - extents.y, center.z + extents.z ), y );
					Vector2 frontTopLeft_2d, frontTopRight_2d, frontBottomLeft_2d, frontBottomRight_2d, backTopLeft_2d, backTopRight_2d, backBottomLeft_2d, backBottomRight_2d;
					if (utils::w2s(frontTopLeft, frontTopLeft_2d) &&
						utils::w2s(frontTopRight, frontTopRight_2d) &&
						utils::w2s(frontBottomLeft, frontBottomLeft_2d) &&
						utils::w2s(frontBottomRight, frontBottomRight_2d) &&
						utils::w2s(backTopLeft, backTopLeft_2d) &&
						utils::w2s(backTopRight, backTopRight_2d) &&
						utils::w2s(backBottomLeft, backBottomLeft_2d) &&
						utils::w2s(backBottomRight, backBottomRight_2d)) {
						
						g_Render->DrawLine(backTopLeft_2d.x, backTopLeft_2d.y, backTopRight_2d.x, backTopRight_2d.y, color, 1.25f, false);
						g_Render->DrawLine(backTopRight_2d.x, backTopRight_2d.y, backBottomRight_2d.x, backBottomRight_2d.y, color, 1.25f, false);
						g_Render->DrawLine(backBottomRight_2d.x, backBottomRight_2d.y, backBottomLeft_2d.x, backBottomLeft_2d.y, color, 1.25f, false);
						g_Render->DrawLine(backBottomLeft_2d.x, backBottomLeft_2d.y, backTopLeft_2d.x, backTopLeft_2d.y, color, 1.25f, false);
					}			
				}
			float SizeHelthbar = VarSs::players::healthbar_::font == 1 ? 12 : 10;
			float SizeWeapon = VarSs::players::weapon_::font == 1 ? 12 : 10;
			float SizeName = VarSs::players::name_::font == 1 ? 12 : 10;
			int cur = 0;						

			/*if (BP->IsPlayer()) {
				if (VarSs::players::healthbar)
				{

					bool set = true;

					if (BP) 
					{
						if (VarSs::players::healthsend0) 
						{
							
							g_Render->Rect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0), 10, 0);
							g_Render->FilledRectGradient(Entity_x + Entity_w / 2 - 19, Entity_y + cur + 6, 40 * (100 / 100.f), 3, ImColor(0, 255, 0), ImColor(255, 200, 0), 10, 10);					
						}

						if (set)
						{

							g_Render->Rect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0), 10, 0);
							g_Render->FilledRectGradient(Entity_x + Entity_w / 2 - 19, Entity_y + cur + 6, 40 * (100 / 100.f), 3, ImColor(0, 255, 0), ImColor(255, 200, 0), 10, 10);

						}
						cur += 11;

					}

					if (target_player)
					{						
						if (VarSs::players::healthsend1)
						{

							set = false;

							g_Render->Rect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0), 10, 0);
							g_Render->FilledRectGradient(Entity_x + Entity_w / 2 - 19, Entity_y + cur + 6, 40 * (35 / 100.f), 3, ImColor(255, 0, 0), ImColor(255, 200, 0), 10, 10);

							cur += 11;
						}
					}					
				}	
			}*/

			/*int reload_anim = ImTricks::Animations::FastIntLerp(("ReloadInd animation"), VarSs::players::reload_offer, 0, 255, 15);
			if (reload_anim > 1) {
				auto held = BP->GetHeldEntity<BaseProjectile>();
				float size = 150;
				if (held) {
					if (held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {


						if (did_reload == false && time_since_last_shot <= (held->reloadTime() - (held->reloadTime() / 10)) && time_since_last_shot > 0) {
							g_Render->Rect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0, reload_anim), 10, 0);
							float reloadDurationTotal = (held->reloadTime() - (held->reloadTime() / 10));
							float reloadDuration = time_since_last_shot;
							auto percentage = (((reloadDuration / reloadDurationTotal) * 100.0f) + 1.f) / 100;
							int reload_anim_active = ImTricks::Animations::FastIntLerp("ReloadInd1 animation", VarSs::players::reload_offer && percentage != 1, 0, 255, 15);

							if (reload_anim_active > 1) {																
								g_Render->FilledRect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0, reload_anim_active), 10, 0);								
							}
						}
						else {
							if (held->HasReloadCooldown()) {
								g_Render->FilledRect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0, reload_anim), 10, 0);
								float SamReloadDuration = held->nextReloadTime() - GLOBAL_TIME;
								float SamReloadDurationTotal = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
								auto SamPercentage = (((SamReloadDuration / SamReloadDurationTotal) * 100.0f) + 1.f) / 100;
								int reload_anim_active = ImTricks::Animations::FastIntLerp(("ReloadInd2 animation"), VarSs::players::reload_offer && SamPercentage != 1, 0, 255, 15);

								if (reload_anim_active > 1) {
									g_Render->FilledRect(Entity_x + Entity_w / 2 - 20, Entity_y + cur + 5, 40, 5, ImColor(0, 0, 0, reload_anim_active), 10, 0);									
								}
							}
						}
						cur += 11;
					}
				}
			}*/


			if (VarSs::players::distance || VarSs::players::name){
				std::string _name(CStringA(BP->_displayName()));
				std::string string; char buf[256] = { 0 };
				sprintf_s(buf, ("%dm"), (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), BP->get_bone_pos(head)));
				string += buf;
				std::string name_distance = _name;				

				if (VarSs::players::distance && VarSs::players::name)
					name_distance = _name + (" [") + string + ("]");
				else if (VarSs::players::distance && !VarSs::players::name)
					name_distance = (" [") + string + ("]");
				else
					name_distance = _name;
				g_Render->DrawString(Entity_x + Entity_w / 2, Entity_y + Entity_h - 26, ImColor(VarSs::colors::name_distance_color_player[0], VarSs::colors::name_distance_color_player[1], VarSs::colors::name_distance_color_player[2]), render2::centered_x | (VarSs::players::name_::outline ? render2::outline : 0), 2, 10, name_distance.c_str());				
			}
		
			if (VarSs::players::weapon && !BP->HasPlayerFlag(PlayerFlags::Sleeping) )
			{
				auto haveImage = get_icon_name(BP).texture.texture && VarSs::players::DisableWeaponName;
				if (!haveImage && VarSs::players::weapon && BP->GetHeldItem( )) {
					std::string string;	char buf[256] = { 0 };
					sprintf_s(buf, "%ls", BP->GetHeldItem()->info()->displayName()->english());
					string += buf;
					g_Render->DrawString(Entity_x + Entity_w / 2, Entity_y + cur + 1.f, ImColor(VarSs::colors::weapon_color_player[0], VarSs::colors::weapon_color_player[1], VarSs::colors::weapon_color_player[2]), render2::centered_x | (VarSs::players::weapon_::outline ? render2::outline : 0), 2, 10, string.c_str());
					cur += VarSs::stuff::font == 1 ? 10 : 13;
				}
				if (VarSs::players::WeaponIcon)
				{
					if (get_icon_name(BP).texture.texture && get_icon_name(BP).name != "")
						g_Render->DrawTexture(ImVec2(Entity_x + Entity_w / 2, Entity_y + cur + 1.f), render2::centered_x, get_icon_name(BP).size, ImColor(255, 255, 255), reinterpret_cast<void*>(get_icon_name(BP).texture.texture));
				}
			}
		}
	} 
}