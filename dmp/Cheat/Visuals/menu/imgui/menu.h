#include <vector>
#include <algorithm>
#include <windows.h>
#include <urlmon.h>
#include "shit.h"
#pragma comment(lib, "urlmon.lib")
bool applied = false;
static bool sh_save_cfg = false;
static bool sh_load_cfg = false;
bool Remove = false;
bool create = false;
#include <chrono>


static bool _IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
    const int key_index = GImGui->IO.KeyMap[key];
    return (key_index >= 0) ? ImGui::IsKeyPressed((ImGuiKey)key_index, repeat) : false;
}
inline bool     FocusableItemRegister(ImGuiWindow* window, ImGuiID id) { IM_ASSERT(0); IM_UNUSED(window); IM_UNUSED(id); return false; } // -> pass ImGuiItemAddFlags_Inputable flag to ItemAdd()
bool KeyBind(const char* label, int& k, const ImVec2& size_arg = ImVec2(0, 0))
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 1.0f);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    const bool focus_requested = FocusableItemRegister(window, g.ActiveId == id);

    const bool hovered = ImGui::ItemHoverable(frame_bb, id);

    if (hovered) {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (focus_requested || user_clicked) {
        if (g.ActiveId != id) {
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            k = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    bool value_changed = false;
    int key = k;

    if (g.ActiveId == id) {
        for (auto i = 0; i < 5; i++) {
            if (io.MouseDown[i]) {
                switch (i) {
                case 0:
                    key = VK_LBUTTON;
                    break;
                case 1:
                    key = VK_RBUTTON;
                    break;
                case 2:
                    key = VK_MBUTTON;
                    break;
                case 3:
                    key = VK_XBUTTON1;
                    break;
                case 4:
                    key = VK_XBUTTON2;
                    break;
                }
                value_changed = true;
                ImGui::ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = VK_BACK; i <= VK_RMENU; i++) {
                if (io.KeysDown[i]) {
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
        }

        if (_IsKeyPressedMap(ImGuiKey_Escape)) {
            k = 0;
            ImGui::ClearActiveID();
        }
        else {
            k = key;
        }
    }


    char buf_display[64] = "[-]";
    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImColor(19, 19, 19), style.FrameRounding);
    window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, ImColor(46, 46, 46), style.FrameRounding);
    //ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, (ImColor(35, 35, 35)), true, 6.f);

    if (k != 0 && g.ActiveId != id) {
        strcpy_s(buf_display, KeyNames[k]);
    }
    else if (g.ActiveId == id) {
        strcpy_s(buf_display, "...");
    }

    const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 render_pos = frame_bb.Min + style.FramePadding;
    ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding * 0.8, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);

    if (label_size.x > 0)
        ImGui::RenderText(ImVec2(total_bb.Max.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

static const char* hitmaterial[]{
    "Default",
    "Glass",
    "Water",
    "Wood",
    "Metal",
    "Sand",
    "Grass",
    "Rock",
    "Concrete",
    "Forest",
    "Cloth",
    "Generic",
    "Null",
};
static const char* hitsound_type[]{
"NL",
"Blick",
"Minecraft",
};
// Main code
#include <map>
bool TabEx(const char* label, const bool active, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x, label_size.y + style.FramePadding.y);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
    if (pressed)
        ImGui::MarkItemEdited(id);
    ImGui::RenderNavHighlight(bb, id);
    struct TabAnimation {
        ImColor CheckboxFrame;
    };

    static std::map< ImGuiID, TabAnimation > TabMap;
    auto TabItemMap = TabMap.find(id);
    if (TabItemMap == TabMap.end()) {
        TabMap.insert({ id, TabAnimation() });
        TabItemMap = TabMap.find(id);
    }
    TabItemMap->second.CheckboxFrame = ImLerp(TabItemMap->second.CheckboxFrame.Value, active ? ImColor(accent_color[0], accent_color[1], accent_color[2]) : hovered ? ImColor(accent_color[0], accent_color[1], accent_color[2], 200.f) : ImColor(143, 143, 143), g.IO.DeltaTime * 8.f * 0.8);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(TabItemMap->second.CheckboxFrame));

    ImGui::RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x + style.FramePadding.x, bb.Max.y + style.FramePadding.y), label, NULL, &label_size, ImVec2(0.f, 0.f), &bb);

    ImGui::PopStyleColor();
    return pressed;
}
bool Tab(const char* label, const bool active, const ImVec2& size_arg)
{
    return TabEx(label, active, size_arg, 0);
}
static int pTabs = 0;
static int pSubTabs = 0;
static int pSubTabs2 = 0;
static int pSubTabsMisc2 = 0;
static float f = 0.0f;
static int counter = 0;
static float fov_color[] = { 120.f, 255.f, 120.f };
static float one_color[] = { 1.f, 1.f, 1.f, 1.f };
static float two_color[] = { 1.f, 1.f, 1.f, 1.f };
//static float tracer_color[] = { 1.f, 1.f, 1.f, 1.f };
static bool check[15];
ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;

namespace elements {
    void AimBot() {
        ImGui::SetCursorPos(ImVec2(10, 48));
        ImGui::BeginChild("Aimbot", ImVec2(223, 421)); {
            ImGui::Checkbox(("Silent Aim"), &vars::combat::psilent);
            if (vars::combat::psilent)
            {
                ImGui::Checkbox(("Headshoot Bone"), &vars::combat::always_headshot);
            }
            ImGui::SliderFloat(("Fov Value"), &vars::combat::fov, 20.f, 2000.f, ("%.0f"));
            ImGui::Checkbox(("Draw Fov"), &vars::combat::visualize_fov); ImGui::SameLine(); ImGui::ColorEdit4((" ##Color"), vars::colors::fov_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox(("Aspect Ratio"), &vars::misc::aspectraratio);
            if (vars::misc::aspectraratio)
            {
                ImGui::SliderFloat("Aspect Ratio float", &vars::misc::aspectraratiofloat, 1000.f, 4000.f, "%.0f");
            }
            ImGui::Checkbox(("Heli Aim"), &vars::combat::psilentheli);
        } ImGui::EndChild();
        ImGui::SameLine(0, 9);
        ImGui::BeginChild("Aimbot2", ImVec2(223, 421));
        {
            ImGui::Checkbox(("Pierce Materials"), &vars::weapons::penetrate);
            ImGui::Checkbox(("Bullet Teleport"), &vars::combat::bullet_tp);
            ImGui::Checkbox(("DALBAEB"), &vars::combat::hitscan);
            ImGui::Checkbox(("Show Radius Bullet TP"), &vars::visuals::show_radius_bullettp);
            ImGui::Checkbox(("Manipulator"), &vars::combat::manipulator); KeyBind(("##Manipulator Key"), vars::keys::manipulated_key, ImVec2(70, 18));
            ImGui::Checkbox(("Auto Shoot"), &vars::combat::autoshoot);
            if (vars::combat::autoshoot) {
                ImGui::Combo(("Auto Shoot Mode"), &vars::combat::autoshoot_type, autoshoot_type, IM_ARRAYSIZE(autoshoot_type));
                ImGui::Combo(("Shoot Type"), &vars::combat::desync_autoshoot_type, desync_autoshoot_type, IM_ARRAYSIZE(desync_autoshoot_type));
            }
            ImGui::Checkbox(("Bullet Size"), &vars::weapons::weaponsize);
            if (vars::weapons::weaponsize)
            {
                ImGui::SliderFloat("Bullet Size X", &vars::weapons::weaponsizex, 1, 50);
                ImGui::SliderFloat("Bullet Size Y", &vars::weapons::weaponsizey, 1, 50);
                ImGui::SliderFloat("Bullet Size Z", &vars::weapons::weaponsizez, 1, 50);
            }
        } ImGui::EndChild();
    }
    void Visuals() {
        ImGui::SetCursorPos(ImVec2(10, 48));
        ImGui::BeginChild("SubTabs", ImVec2(455, 35));
        {
            ImGui::SetCursorPos(ImVec2(131, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25, 5));
            if (Tab(("Players"), pSubTabs == 0, ImVec2(0, 0))) pSubTabs = 0;
            ImGui::SameLine();
            if (Tab(("Entities"), pSubTabs == 1, ImVec2(0, 0))) pSubTabs = 1;
            ImGui::SameLine();
            if (Tab(("Misc"), pSubTabs == 2, ImVec2(0, 0))) pSubTabs = 2;
            ImGui::PopStyleVar();

        } ImGui::EndChild();
        switch (pSubTabs)
        {
        case 0:
            ImGui::SetCursorPosX(10);
            ImGui::BeginChild("Aimbot", ImVec2(223, 382)); {
                ImGui::Combo(("Box Style"), &vars::players::boxstyle, box_style, IM_ARRAYSIZE(box_style));
                ImGui::Combo(("HB St."), &vars::players::healthbarscolortyle, healthbar_color_style, IM_ARRAYSIZE(healthbar_color_style));
                ImGui::Combo(("HB St. Color"), &vars::players::healthbarscolor, healthbar_color, IM_ARRAYSIZE(healthbar_color));
                if (vars::players::healthbarscolor == 1)
                    ImGui::ColorEdit4(("Custom Health Color##Color"), vars::colors::Player_custom_healthColor, ImGuiColorEditFlags_NoInputs);
                if (vars::players::healthbarscolortyle == 1) {
                    ImGui::ColorEdit4(("Gradient Two##Color"), vars::colors::gradient_2, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Gradient One##Color"), vars::colors::gradient_1, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                }
                ImGui::Checkbox(("Name"), &vars::players::name);  ImGui::SameLine(); ImGui::ColorEdit4((" ##Color"), vars::visible::box_color, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Distance"), &vars::players::distance);
                ImGui::Checkbox(("Health Bar"), &vars::players::healthbar);
                ImGui::Checkbox(("Weapon"), &vars::players::weapon); 
                ImGui::Checkbox(("Skeleton"), &vars::players::skeleton); ImGui::SameLine(); ImGui::ColorEdit4(("##Color"), vars::invisible::skeleton_color, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Target Tracers"), &vars::players::targetline);
                if (vars::players::targetline) {
                    ImGui::Combo(("Target Tracer Position "), &vars::players::targetline_pos, target_tracer_pos, IM_ARRAYSIZE(target_tracer_pos));
                }
                ImGui::Checkbox(("Target Player Belt"), &vars::players::belt);
                ImGui::Checkbox(("Target Player Clothes"), &vars::players::clothes);
                ImGui::Checkbox(("Out Of Arrows"), &vars::players::oof_arrows);
                if (vars::players::oof_arrows) {
                    ImGui::Combo(("Out Of Arrows style"), &vars::players::Outstyle, Out_style, IM_ARRAYSIZE(Out_style));
                }
                ImGui::Checkbox(("Ignore Sleepers"), &vars::players::sleeperignore);//size_name

            } ImGui::EndChild();
            ImGui::SameLine(0, 9);
            ImGui::BeginChild("Aimbot2", ImVec2(223, 382));
            {
                ImGui::Checkbox(("NPCS"), &vars::npc::box);
                if (ImGui::BeginPopupContextItem()) {
                    ImGui::BeginChild("NPCS Settings", ImVec2(300, 300), false, ImGuiWindowFlags_NoMove); {

                        ImGui::Combo(("Box Style "), &vars::npc::boxstyle, npc_box_style, IM_ARRAYSIZE(npc_box_style)); ImGui::ColorEdit4(("  ##Color"), vars::colors::npc_box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                        ImGui::Combo(("Healthbar Style"), &vars::npc::healthbarscolortyle, npc_healthbar_color_style, IM_ARRAYSIZE(npc_healthbar_color_style)); 
                        ImGui::Combo(("Healthbar Style Color"), &vars::npc::healthbarscolor, npc_healthbar_color, IM_ARRAYSIZE(npc_healthbar_color)); 
                        if (vars::npc::healthbarscolor == 1)
                            ImGui::ColorEdit4(("##Color"), vars::colors::npc_custom_healthColor, ImGuiColorEditFlags_NoInputs);
                        if (vars::npc::healthbarscolortyle == 1) {
                            ImGui::ColorEdit4(("Grad 2##Color"), vars::colors::gradient_2, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4(("Grad 1##Color"), vars::colors::gradient_1, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                        }
                        ImGui::Checkbox(("Name"), &vars::npc::name); ImGui::ColorEdit4(("  ##Color"), vars::colors::npc_name_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                        ImGui::Checkbox(("Health Bar"), &vars::npc::healthbar);
                        ImGui::Checkbox(("Distance"), &vars::npc::distance);
                        ImGui::Checkbox(("Fill Box "), &vars::npc::fillbox); 
                        ImGui::Checkbox(("Skeleton "), &vars::npc::skeleton); ImGui::ColorEdit4(("  ##Color"), vars::colors::npc_skeleton_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                        ImGui::Checkbox(("Weapon "), &vars::npc::weapon); ImGui::ColorEdit4(("  ##Color"), vars::colors::npc_weapons_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    }
                    ImGui::EndChild();
                    ImGui::EndPopup();
                }
                ImGui::Checkbox(("Ignore Sleepers"), &vars::players::sleeperignore);
            } ImGui::EndChild();
            break;
        case 1:
            ImGui::SetCursorPosX(10);
            ImGui::BeginChild("SubTabs2", ImVec2(455, 35));
            {
                ImGui::SetCursorPos(ImVec2(32, 10));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25, 5));
                if (Tab(("Ore/Crate"), pSubTabs2 == 0, ImVec2(0, 0))) pSubTabs2 = 0;
                ImGui::SameLine();
                if (Tab(("Important/Animals"), pSubTabs2 == 1, ImVec2(0, 0))) pSubTabs2 = 1;
                ImGui::SameLine();
                if (Tab(("Vehicles/Traps"), pSubTabs2 == 2, ImVec2(0, 0))) pSubTabs2 = 2;
                ImGui::SameLine();
                if (Tab(("Misc"), pSubTabs2 == 3, ImVec2(0, 0))) pSubTabs2 = 3;
                ImGui::PopStyleVar();

            } ImGui::EndChild();
            switch (pSubTabs2)
            {
            case 0:
                ImGui::SetCursorPosX(10);
                ImGui::BeginChild("Aimbot", ImVec2(223, 342));
                {
                    ImGui::Checkbox(("Ore Path"), &vars::ores::closest_ore);
                    ImGui::Checkbox(("Stone"), &vars::ores::stone);
                    ImGui::Checkbox(("Sulfur"), &vars::ores::sulfur);
                    ImGui::Checkbox(("Metal"), &vars::ores::metal);
                    if (vars::ores::sulfur || vars::ores::stone || vars::ores::metal) {
                        ImGui::SliderFloat(("Ores Distance"), &vars::ores::draw_distance, 5.f, 400.f, "%.0f");
                        ImGui::Checkbox(("Show Collectables"), &vars::ores::show_collectables);
                        ImGui::Checkbox(("Show Distance"), &vars::ores::show_distance);
                    }


                } ImGui::EndChild();
                ImGui::SameLine(0, 9);
                ImGui::BeginChild("Aimbot2", ImVec2(223, 342));
                {
                    ImGui::Checkbox(("Chinook Crates"), &vars::visuals::crates::chinook);
                    ImGui::Checkbox(("Airdrops"), &vars::visuals::crates::supply);
                    ImGui::Checkbox(("Heli Box"), &vars::visuals::crates::heli);
                    ImGui::Checkbox(("Bradley Box"), &vars::visuals::crates::bradley);
                    ImGui::Checkbox(("Military Box"), &vars::visuals::crates::military);
                    ImGui::Checkbox(("Elite Box"), &vars::visuals::crates::elite);
                    if (vars::visuals::crates::supply || vars::visuals::crates::bradley || vars::visuals::crates::heli || vars::visuals::crates::military || vars::visuals::crates::elite) {
                        ImGui::SliderFloat(("Crates Distance"), &vars::visuals::crates::draw_distance, 5.f, 3000.f, "%.0f");
                        ImGui::Checkbox(("Show Distance "), &vars::visuals::crates::show_distance);
                    }
                } ImGui::EndChild();
                break;
            case 1:
                ImGui::SetCursorPosX(10);
                ImGui::BeginChild("Aimbot", ImVec2(223, 342));
                {
                    ImGui::Checkbox(("Bears"), &vars::visuals::animals::bear);
                    ImGui::Checkbox(("Boars"), &vars::visuals::animals::pig);
                    ImGui::Checkbox(("Wolfs"), &vars::visuals::animals::wolf);
                    ImGui::Checkbox(("Horses"), &vars::visuals::animals::deer);
                    ImGui::Checkbox(("Chickens"), &vars::visuals::animals::chicken);
                    if (vars::visuals::animals::bear || vars::visuals::animals::pig || vars::visuals::animals::wolf || vars::visuals::animals::deer || vars::visuals::animals::chicken) {
                        ImGui::SliderFloat("Animals Distance", &vars::visuals::animals::draw_distance, 5.f, 400.f, "%.0f");
                        ImGui::Checkbox(("Show Distance"), &vars::visuals::animals::show_distance);
                    }
                } ImGui::EndChild();
                ImGui::SameLine(0, 9);
                ImGui::BeginChild("Aimbot2", ImVec2(223, 342));
                {
                    ImGui::Checkbox(("Raid Esp"), &vars::visuals::raid_esp);
                    ImGui::Checkbox(("Dropped Items"), &vars::visuals::other::dropped_items);
                    ImGui::Checkbox(("Corpses"), &vars::visuals::other::corpse);
                    ImGui::Checkbox(("Bodybags"), &vars::visuals::other::bodybag);
                    ImGui::Checkbox(("Stashes"), &vars::visuals::other::stash);
                    ImGui::Checkbox(("Hemp"), &vars::visuals::other::hemp);
                    ImGui::Checkbox(("Berries"), &vars::visuals::other::berrie);
                    ImGui::Checkbox(("Corn"), &vars::visuals::other::corn);
                    ImGui::Checkbox(("Pumpkin"), &vars::visuals::other::pumpkin);
                    ImGui::Checkbox(("Potato"), &vars::visuals::other::potato);
                    ImGui::Checkbox(("Mushrooms"), &vars::visuals::other::mushrooms);
                    if (vars::visuals::other::corpse || vars::visuals::other::stash || vars::visuals::other::hemp || vars::visuals::other::dropped_items || vars::visuals::other::berrie || vars::visuals::other::corn || vars::visuals::other::potato || vars::visuals::other::pumpkin || vars::visuals::other::mushrooms) {
                        ImGui::SliderFloat(("Important Distance"), &vars::visuals::other::draw_distance, 5.f, 400.f, "%.0f");
                        ImGui::Checkbox(("Show Distance"), &vars::visuals::other::show_distance);
                    }
                } ImGui::EndChild();
                break;
            case 2:
                ImGui::SetCursorPosX(10);
                ImGui::BeginChild("Aimbot", ImVec2(223, 342)); {
                    ImGui::Checkbox(("Patrol Heli"), &vars::visuals::patrol_heli);
                    ImGui::Checkbox(("Bradley APC"), &vars::visuals::bradley_apc);
                    ImGui::Checkbox(("Minicopters"), &vars::visuals::vehicles::minicopter);
                    ImGui::Checkbox(("Scrap Helis"), &vars::visuals::vehicles::scrapheli);
                    ImGui::Checkbox(("Boats"), &vars::visuals::vehicles::boat);
                    ImGui::Checkbox(("RHIB's"), &vars::visuals::vehicles::rhib);
                    if (vars::visuals::vehicles::minicopter || vars::visuals::vehicles::scrapheli || vars::visuals::vehicles::boat || vars::visuals::vehicles::rhib) {
                        ImGui::SliderFloat(("Vehicles Distance"), &vars::visuals::vehicles::draw_distance, 5.f, 3000.f, "%.0f");
                        ImGui::Checkbox(("Show Distance"), &vars::visuals::vehicles::show_distance);
                    }
                } ImGui::EndChild();
                ImGui::SameLine(0, 9);
                ImGui::BeginChild("Aimbot2", ImVec2(223, 342)); {
                    ImGui::Checkbox(("Auto Turrets"), &vars::visuals::turrets::auto_turret);
                    ImGui::Checkbox(("Flame Turrets"), &vars::visuals::turrets::flame_turret);
                    ImGui::Checkbox(("Shotgun Traps"), &vars::visuals::turrets::shotgun_turret);
                    ImGui::Checkbox(("SAM Sites"), &vars::visuals::turrets::sam_site);
                    ImGui::Checkbox(("Landmines"), &vars::visuals::turrets::landmine);
                    ImGui::Checkbox(("Beartraps"), &vars::visuals::turrets::bear_trap);
                    if (vars::visuals::turrets::auto_turret || vars::visuals::turrets::flame_turret || vars::visuals::turrets::shotgun_turret || vars::visuals::turrets::landmine || vars::visuals::turrets::bear_trap) {
                        ImGui::SliderFloat(("Traps Distance"), &vars::visuals::turrets::draw_distance, 5.f, 400.f, "%.0f");
                        ImGui::Checkbox(("Show Distance"), &vars::visuals::turrets::show_distance);
                    }
                } ImGui::EndChild();
                break;
            case 3:
                ImGui::SetCursorPosX(10);
                ImGui::BeginChild("Aimbot", ImVec2(223, 342)); {
                    ImGui::Checkbox(("Tool Cupboards"), &vars::visuals::base::tc);
                    ImGui::Checkbox(("Sleeping Bags"), &vars::visuals::base::sleeping_bag);
                    ImGui::Checkbox(("Beds"), &vars::visuals::base::bed);
                    ImGui::Checkbox(("Boxes"), &vars::visuals::base::boxes);
                    if (vars::visuals::base::tc || vars::visuals::base::sleeping_bag || vars::visuals::base::bed || vars::visuals::base::boxes) {
                        ImGui::SliderFloat(("Base Distance"), &vars::visuals::base::draw_distance, 5.f, 400.f, "%.0f");
                        ImGui::Checkbox(("Show Distance"), &vars::visuals::base::show_distance);
                    }
                } ImGui::EndChild();
                ImGui::SameLine(0, 9);
                ImGui::BeginChild("Aimbot2", ImVec2(223, 342)); {

                } ImGui::EndChild();
                break;
            }
            break;
        case 2:
            ImGui::SetCursorPosX(10);
            ImGui::BeginChild("Aimbot", ImVec2(223, 382));
            {
                ImGui::Checkbox(("Noclip Bar"), &vars::misc::flyhack_indicator);
                ImGui::Checkbox(("Desync Indicator"), &vars::combat::instakill_indicator);
                ImGui::Checkbox(("Show Target Name"), &vars::visuals::target_name); ImGui::SameLine(); ImGui::ColorEdit4(("            ##Color"), (float*)vars::colors::target_name_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Hit Logs"), &vars::misc::hit_logs);
                ImGui::Checkbox(("Cadicall Cross"), &vars::crosshair::cadicall); ImGui::SameLine(); ImGui::ColorEdit4(("    ##Color"), vars::colors::cadicall_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Evil Cross"), &vars::crosshair::default); ImGui::SameLine(); ImGui::ColorEdit4((" ##Color"), vars::colors::default_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Dot Cross"), &vars::crosshair::dot); ImGui::SameLine(); ImGui::ColorEdit4(("##Color"), vars::colors::dot_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Custom Cross"), &vars::crosshair::custom); ImGui::SameLine(); ImGui::ColorEdit4(("   ##Color"), vars::colors::custom_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                if (vars::crosshair::custom) {
                    ImGui::SliderFloat(("Gap"), &vars::crosshair::gap, 0.f, 50.f, "%.0f");
                    ImGui::SliderFloat(("Length"), &vars::crosshair::length, 0.f, 150.f, "%.0f");
                }
                ImGui::Combo(("Hit Effect"), &vars::misc::hitmaterial, hitmaterial, IM_ARRAYSIZE(hitmaterial));
            } ImGui::EndChild();
            ImGui::SameLine(0, 9);
            ImGui::BeginChild("Aimbot2", ImVec2(223, 382));
            {
                ImGui::Checkbox(("Custom Time"), &vars::misc::custom_time);
                if (vars::misc::custom_time)
                    ImGui::SliderFloat(("Time"), &vars::misc::time, 1.f, 24.f, ("%.0f Hours"));
                ImGui::Checkbox(("Bullet Tracers"), &vars::misc::bullet_tracers); ImGui::SameLine(); ImGui::ColorEdit4(("              ##Color"), (float*)&vars::colors::tracers, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                if (vars::misc::bullet_tracers)
                    ImGui::SliderFloat(("Tracer duration"), &vars::misc::tracer_duration, 0.1f, 10.f, "%.1f");
                ImGui::Checkbox(("Hitmarker"), &vars::misc::hitmarker); ImGui::SameLine(); ImGui::ColorEdit4(("               ##Color"), (float*)&vars::colors::marker, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                if (vars::misc::hitmarker)
                    ImGui::SliderFloat(("HitMarker duration"), &vars::misc::hitmarker_duration, 0.1f, 10.f, "%.1f");
                ImGui::Checkbox(("Bright Ambient"), &vars::misc::bright_ambient); ImGui::SameLine(); ImGui::ColorEdit4(("                  ##Color"), (float*)&vars::colors::ambient_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(("Night Stars"), &vars::misc::night_stars);
                if (vars::misc::night_stars) {
                    ImGui::SliderFloat(("Stars Amount"), &vars::misc::stars, 0.f, 40000.f, "%.0f");
                }
                ImGui::Checkbox(("Mie Changer"), &vars::misc::mie_changer);
                if (vars::misc::mie_changer) {
                    ImGui::SliderFloat(("Mie Amount"), &vars::misc::mie, 1.f, 100.f, "%.0f");
                }
                ImGui::Checkbox(("Rayleigh Changer"), &vars::misc::rayleigh_changer);
                if (vars::misc::rayleigh_changer) {
                    ImGui::SliderFloat(("Rayleigh Amount"), &vars::misc::rayleigh, 1.f, 100.f, "%.0f");
                }
                ImGui::Checkbox(("Brightness Changer"), &vars::misc::brightness_changer);
                if (vars::misc::brightness_changer) {
                    ImGui::SliderFloat(("Brightness Amount"), &vars::misc::brightness, 1.f, 50.f, "%.0f");
                }

            } ImGui::EndChild();
            break;
        }
    }
    void Misc() {
        ImGui::SetCursorPos(ImVec2(10, 48));
        ImGui::BeginChild("SubTabs", ImVec2(455, 35));
        {
            ImGui::SetCursorPos(ImVec2(115, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25, 5));
            if (Tab(("Movement"), pSubTabsMisc2 == 0, ImVec2(0, 0))) pSubTabsMisc2 = 0;
            ImGui::SameLine();
            if (Tab(("Weapon"), pSubTabsMisc2 == 1, ImVec2(0, 0))) pSubTabsMisc2 = 1;
            ImGui::SameLine();
            if (Tab(("Misc"), pSubTabsMisc2 == 2, ImVec2(0, 0))) pSubTabsMisc2 = 2;
            ImGui::PopStyleVar();

        } ImGui::EndChild();
        switch (pSubTabsMisc2)
        {
        case 0:
            ImGui::SetCursorPosX(10);
            ImGui::BeginChild("Aimbot", ImVec2(223, 382));
            {
                ImGui::Checkbox(("No Collision"), &vars::misc::walker);
                ImGui::Combo(("Model State"), &vars::misc::modelstate, modelstate_pos, IM_ARRAYSIZE(modelstate_pos));
                ImGui::Checkbox(("Omni Sprint"), &vars::misc::omnidirectional_sprinting);
                ImGui::Checkbox(("Suicide"), &vars::misc::mass_suicide);
                ImGui::Checkbox(("Spiderman"), &vars::misc::spiderman);
                ImGui::Checkbox(("No Falldamage"), &vars::misc::no_fall);
                ImGui::Checkbox(("Infinity Jump"), &vars::misc::inf_jump);
                ImGui::Checkbox(("Speed Hack"), &vars::misc::speedhack); KeyBind(("Speed Hack Key"), vars::keys::speedkey, ImVec2(70, 18));

            } ImGui::EndChild();
            ImGui::SameLine(0, 9);
            ImGui::BeginChild("Aimbot2", ImVec2(223, 382));
            {

            } ImGui::EndChild();
            break;
        case 1:
            ImGui::SetCursorPosX(10);
            ImGui::BeginChild("Aimbot", ImVec2(223, 382));
            {
                ImGui::Checkbox(("Silent Melee"), &vars::combat::silent_melee);
                ImGui::Checkbox(("Weapon Spam"), &vars::misc::weapon_spam);
                KeyBind(("Weapon Spam Key"), vars::keys::weaponspam, ImVec2(70, 18));
                ImGui::Checkbox(("No Recoil"), &vars::weapons::no_recoil);
                if (vars::weapons::no_recoil) {
                    ImGui::SliderFloat(("Recoil"), &vars::weapons::recoil_control, 0.f, 100.f, ("%.0f"));
                }
                ImGui::Checkbox(("No Sway"), &vars::weapons::no_sway);
                ImGui::Checkbox(("No Spread"), &vars::weapons::no_spread);
                ImGui::Checkbox(("Fast Bullets"), &vars::weapons::fast_bullets);
                if (vars::weapons::fast_bullets)
                {
                    ImGui::SliderFloat("Fast Bullets Custom", &vars::weapons::fastbullet, 0.500f, 1.499f);
                }
                ImGui::Checkbox(("Can Hold Items"), &vars::weapons::minicopter_aim);
                ImGui::Checkbox(("Jump Shoot"), &vars::misc::can_attack);
            } ImGui::EndChild();
            ImGui::SameLine(0, 9);
            ImGui::BeginChild("Aimbot2", ImVec2(223, 382));
            {
                ImGui::Checkbox(("Automatic"), &vars::weapons::automatic);
                ImGui::Checkbox(("Thick bullet"), &vars::weapons::thick_bullet);
                if (vars::weapons::thick_bullet) {
                    ImGui::SliderFloat(("Thick"), &vars::weapons::thickness, 0.1f, 2.0f);
                }

                ImGui::Checkbox(("Instant Eoka"), &vars::weapons::eokatap);
                ImGui::Checkbox(("Fast Bow"), &vars::weapons::fastbow);
                ImGui::Checkbox(("Auto Reload"), &vars::combat::autoreload);
                ImGui::Checkbox(("No Attack Animation"), &vars::weapons::remove_attack_anim);
                ImGui::Checkbox(("Insta Charge Compound"), &vars::weapons::compound);
            } ImGui::EndChild();
            break;
        case 2:
            ImGui::SetCursorPosX(10);
            ImGui::BeginChild("Aimbot", ImVec2(223, 382));
            {
                ImGui::Checkbox(("Auto Healing"), &vars::misc::faster_healing);
                ImGui::Checkbox(("Zoom"), &vars::visuals::zoom);
                KeyBind(("Zoom Key"), vars::keys::zoom, ImVec2(70, 18));
                ImGui::Checkbox(("Insta Revive"), &vars::misc::auto_revive); KeyBind(("Insta Revive Key"), vars::keys::insta_revive, ImVec2(70, 18));

            } ImGui::EndChild();
            ImGui::SameLine(0, 9);
            ImGui::BeginChild("Aimbot2", ImVec2(223, 382));
            {
                ImGui::Checkbox(("Insta Loot"), &vars::misc::fast_loot);
                ImGui::Checkbox(("Interactive Debug"), &vars::misc::interactive_debug); KeyBind(("Debug Key"), vars::keys::debugging, ImVec2(70, 18));
                ImGui::Checkbox(("Admin Mode"), &vars::misc::fakeadmin);
                ImGui::SliderFloat(("Fov Changer"), &vars::misc::fov, 30.f, 180.f, "%.0f");
            } ImGui::EndChild();
            break;
        }

    }
    void Settings() {
        ImGui::SetCursorPos(ImVec2(10, 48));
        ImGui::BeginChild("Aimbot", ImVec2(223, 421));
        {
            ImGui::ColorEdit4(("Logo one Color"), (float*)&one_color, flags);
            ImGui::ColorEdit4(("Logo Two Color"), (float*)&two_color, flags);
            ImGui::ColorEdit4(("Accent Color"), (float*)&accent_color, flags);
        } ImGui::EndChild();
        ImGui::SameLine(0, 9);
        ImGui::BeginChild("Aimbot2", ImVec2(223, 421));
        {
        } ImGui::EndChild();
    }
    void Config() {
        ImGui::SetCursorPos(ImVec2(10, 48));
        ImGui::BeginChild("Aimbot", ImVec2(223, 421));
        {
            ImGui::BeginChild("Main", ImVec2(100, 100), true, 0);
            {
                std::vector<std::string> files;
                config_manager::read_directory(("C:/dxrust.wtf/"), files);
                for (auto cfg : config.GetConfigList())
                    if (ImGui::Selectable(cfg.c_str(), cfg == vars::stuff::selected_cfg))
                        vars::stuff::selected_cfg = cfg;

                ImGui::EndChild();

            }

            if (!sh_save_cfg) {
                if (ImGui::Button(("Save"), ImVec2(0, 0))) {

                    sh_save_cfg = true;
                }
            }
            else
            {
                if (ImGui::Button(("Apply"), ImVec2(0, 0))) {
                    SaveCFG((vars::stuff::selected_cfg));

                    notify::add_log("Config", "Saved", ImColor(accent_color[0], accent_color[1], accent_color[2]), ImColor(255, 255, 255), 4700);
                    sh_save_cfg = false;
                }
            }
            ImGui::SameLine();
            if (!sh_load_cfg) {
                if (ImGui::Button(("Load"), ImVec2(0, 0))) {

                    sh_load_cfg = true;
                }
            }
            else
            {
                if (ImGui::Button(("Apply"), ImVec2(0, 0))) {
                    LoadCFG((vars::stuff::selected_cfg));
                    notify::add_log("Config", "Loaded", ImColor(accent_color[0], accent_color[1], accent_color[2]), ImColor(255, 255, 255), 4700);


                    sh_load_cfg = false;
                }
            }
            ImGui::SameLine();
            if (!Remove) {
                if (ImGui::Button(("Remove"), ImVec2(0, 0))) {

                    Remove = true;
                }
            }
            else
            {
                if (ImGui::Button(("Apply"), ImVec2(0, 0))) {
                    config.Remove(vars::stuff::selected_cfg.c_str());
                    notify::add_log("Config", "Removed", ImColor(accent_color[0], accent_color[1], accent_color[2]), ImColor(255, 255, 255), 4700);


                    Remove = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(("Create"), ImVec2(0, 0))) {
                create = true;
            }
            if (create)
            {
                ImGui::InputText("", vars::stuff::ConfigName, 0x100);
                if (ImGui::ButtonEx(("Accept"), ImVec2(0, 0))) {
                    CREATY(vars::stuff::ConfigName);

                    notify::add_log("Config", "Created", ImColor(accent_color[0], accent_color[1], accent_color[2]), ImColor(255, 255, 255), 4700);
                    create = false;
                }
            }
            if (ImGui::Button(("Open Folder"), ImVec2(0, 0))) {
                system(("start C:\dxrust.wtf"));
            }

            if (ImGui::Button(("Unload Cheat"), ImVec2(0, 0))) {
                vars::misc::fakeadmin = false;
                vars::misc::rayleigh_changer = false;
                vars::misc::rayleigh = 0;
                vars::misc::brightness = 1;
                vars::misc::unloadfakeadmin = true;
                Sleep(3000);
                vars::stuff::Panic = true;
            }

        } ImGui::EndChild();
        ImGui::SameLine(0, 9);
        ImGui::BeginChild("Aimbot2", ImVec2(223, 421));
        {
        } ImGui::EndChild();
    }
    void Info() {
        ImGui::SetCursorPos(ImVec2(10, 48));
        ImGui::BeginChild("Aimbot", ImVec2(223, 421));
        {
            ImGui::Text("Made by lvd. with love.");
            ImGui::Text("If you find som bugs - dm lvd.");
        } ImGui::EndChild();
        ImGui::SameLine(0, 9);
        ImGui::BeginChild("Aimbot2", ImVec2(223, 421));
        {
        } ImGui::EndChild();
    }
}
void render_menu()
{

    static float testFloat = 1;
    ImGui::SetNextWindowSize(menu_size);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings);                          // Create a window called "Hello, world!" and append into it.
    auto p = ImGui::GetWindowPos();
    ImDrawList* d = ImGui::GetWindowDrawList();
    d->AddRectFilled(ImVec2(p.x + 5, p.y + 5), ImVec2(p.x + 5 + menu_size.x, p.y + 5 + 36), ImColor(23, 23, 23));//167
    d->AddRectFilled(ImVec2(p.x + 5, p.y + 52), ImVec2(p.x + 5 + menu_size.x, p.y + 52 + menu_size.y), ImColor(23, 23, 23));//167
    ImGui::SetCursorPos(ImVec2(10, 10));
    ImGui::BeginChild("", ImVec2(475, 26));
    {
        ImGui::SetCursorPos(ImVec2(5, 5));
        const char* text = "dxrust.wtf";
        ImGui::TextColored(ImColor(one_color[0], one_color[1], one_color[2]), ("dxrust"));
        ImGui::SameLine(0, 0);
        ImGui::TextColored(ImColor(two_color[0], two_color[1], two_color[2]), (".wtf"));
        ImGui::SameLine(0, 0);
        ImGui::TextColored(ImColor(143, 143, 143), (""), text);
    }
    ImGui::EndChild();
    ImGui::SetCursorPos(ImVec2(10, 57));
    ImGui::BeginChild(" ", ImVec2(475, menu_size.y - 63));
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25, 5));
        ImGui::SetCursorPos(ImVec2(10, 8));
        ImGui::BeginChild(" ", ImVec2(455, 35));
        {
            ImGui::SetCursorPos(ImVec2(17, 10));

            if (Tab(("AimBot"), pTabs == 0, ImVec2(0, 0))) pTabs = 0;
            ImGui::SameLine();
            if (Tab(("Visuals"), pTabs == 2, ImVec2(0, 0))) pTabs = 2;
            ImGui::SameLine();
            if (Tab(("Misc"), pTabs == 1, ImVec2(0, 0))) pTabs = 1;
            ImGui::SameLine();
            if (Tab(("Settings"), pTabs == 3, ImVec2(0, 0))) pTabs = 3;
            ImGui::SameLine();
            if (Tab(("Config"), pTabs == 4, ImVec2(0, 0))) pTabs = 4;
            ImGui::SameLine();
            if (Tab(("Info"), pTabs == 5, ImVec2(0, 0))) pTabs = 5;
            ImGui::PopStyleVar();
        } ImGui::EndChild();
        switch (pTabs)
        {
        case 0:
            elements::AimBot();
            break;
        case 1:
            elements::Misc();
            break;
        case 2:
            elements::Visuals();
            break;
        case 3:
            elements::Settings();
            break;
        case 4:
            elements::Config();
            break;
        case 5:
            break;
        }

    }
    ImGui::EndChild();
    ImGui::End();
}