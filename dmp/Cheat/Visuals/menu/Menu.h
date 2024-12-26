#include <vector>
#include <algorithm>
#include <windows.h>
#include <urlmon.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <Wininet.h>
#include "Elements/norifications.h"

#include "imgui/imgui.h"
#include <vector>
#include <random>
#include <math.h>


#pragma comment(lib, "wininet.lib")
using namespace std;

#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA2 ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )

string update_logs = ("");
ImVec4 WinBgCol = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



void net_menu() {

    ////particle properties
    //static const int numParticles = 115;
    //static ImVec2 particlePositions[numParticles];
    //static ImVec2 particleDistance;
    //static ImVec2 particleVelocities[numParticles];
    //static bool initialized = false;


    //if (!initialized)
    //{
    //    for (int i = 0; i < numParticles; ++i)
    //    {
    //        particlePositions[i] = ImVec2(
    //            ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * static_cast<float>(rand()) / RAND_MAX,
    //            ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * static_cast<float>(rand()) / RAND_MAX
    //        );

    //        particleVelocities[i] = ImVec2(
    //            static_cast<float>((rand() % 11) - 5),
    //            static_cast<float>((rand() % 11) - 5)
    //        );

    //    }

    //    initialized = true;
    //}

    //ImVec2 cursorPos = ImGui::GetIO().MousePos;
    //for (int i = 0; i < numParticles; ++i)
    //{
    //    //draw lines to particles
    //    for (int j = i + 1; j < numParticles; ++j)
    //    {
    //        float distance = std::hypotf(particlePositions[j].x - particlePositions[i].x, particlePositions[j].y - particlePositions[i].y);
    //        float opacity = 1.0f - (distance / 55.0f);  // opacity cahnge

    //        if (opacity > 0.0f)
    //        {
    //            ImU32 lineColor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacity));
    //            ImGui::GetBackgroundd1awList()->AddLine(particlePositions[i], particlePositions[j], lineColor);
    //        }
    //    }

    //    //draw lines to cursor
    //    float distanceToCursor = std::hypotf(cursorPos.x - particlePositions[i].x, cursorPos.y - particlePositions[i].y);
    //    float opacityToCursor = 1.0f - (distanceToCursor / 52.0f);  // Adjust the divisor to control the opacity change

    //    if (opacityToCursor > 0.0f)
    //    {
    //        ImU32 lineColorToCursor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacityToCursor));
    //        ImGui::GetBackgroundd1awList()->AddLine(cursorPos, particlePositions[i], lineColorToCursor);
    //    }
    //}

    //float deltaTime = ImGui::GetIO().DeltaTime;
    //for (int i = 0; i < numParticles; ++i)
    //{
    //    particlePositions[i].x += particleVelocities[i].x * deltaTime;
    //    particlePositions[i].y += particleVelocities[i].y * deltaTime;

    //    // Stay in window
    //    if (particlePositions[i].x < ImGui::GetWindowPos().x)
    //        particlePositions[i].x = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x;
    //    else if (particlePositions[i].x > ImGui::GetWindowPos().x + ImGui::GetWindowSize().x)
    //        particlePositions[i].x = ImGui::GetWindowPos().x;

    //    if (particlePositions[i].y < ImGui::GetWindowPos().y)
    //        particlePositions[i].y = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
    //    else if (particlePositions[i].y > ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)
    //        particlePositions[i].y = ImGui::GetWindowPos().y;

    //    ImU32 particleColour = ImColor(color_ui[0], color_ui[1], color_ui[2]);

    //    //render particles behind components
    //    ImGui::GetBackgroundd1awList()->AddCircleFilled(particlePositions[i], 1.5f, particleColour);
    //}

    ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

    const char* hand_chams_type[13]{ "Gold", "Gradient", "Frame", "Ghost", "Ghost 2", "Storm", "Storm 2", "Galaxy", "Galaxy 2", "Galaxy 3", "Galaxy 4", "Galaxy 5", "Galaxy 6" };

    const char* player_chams_type[4]{ "Behind", "Lightframe", "Flow", "Shaded" };

    const char* chams_render_mode[3]{ "All", "Only hands", "Only Weapon" };

    const char* hitbox_type[6]{ "None", "Body", "Head", "Random", "Generated", "Splinted" };

    const char* modelstae[5]{ "None", "Lader", "Ground", "Ducked", "Sleep" };

    const char* anti_aim_type[5]{ "Random", "Spin", "Targeted", "Fixed", "Strafe" };

    const char* doubletaptype[3]{ "None", "Default", "Auto" };

    const char* fakelagtype[3]{ "None", "Default", "Maximal" };

    const char* font_type[2]{ "Default", "Pixel" };

    const char* cros_type[3]{ "Custom", "Swaston", "Circle" };

    const char* cros_type_color[2]{ "Colored", "Rainbow" };

    const char* out_type[3]{ "Triangle", "Outline ", "Filled" };

    const char* esp_type[2]{ "Colored", "Rainbow" };

    const char* indicators_type[3]{ "Outlined", "Filled", "Gradient" };

    const char* healthbarstyle[3]{ "Left", "Bottom" , "Right" };

    const char* hammer_effect[4]{ "Glass", "Water", "Forest", "Metal" };

    const char* pvs[5]{ "0.250", "0.500", "0.800", "1.000", "1.500" };

    const char* bullet_tp_type[3]{ "Small", "Medium", "Large" };

    const char* mani_vec_type[2]{ "Static Scan", "Dynamic Scan" };

    const char* scan_type[3]{ "Small", "Balance" , "Hite Over" };

    const char* scan_mode[2]{ "Static Bone", "Dynamic Bone" };

    const char* geasture_s[10]{ "None", "Clap" , "Friendly", "Thumbsdown" , "Thumbsup" , "Ok" , "Point" , "Shrug" , "Victory" , "Wawe" };

    const char* hitmaterial_c[13]{ "Default", "Glass" , "Water(Unreg)", "Wood", "Metal" , "Sand" , "Grass" , "Rock", "Concrete" , "Forest" , "Cloth", "Generic", "Null" };

    const char* boxtype[3]{ "Default", "Cornered", "3D" };

    const char* hp_type[3]{ "Left", "Bottom", "Right" };

    const char* boxtype2[3]{ "Default", "Cornered", "3D" };

    const char* custom_hitsound_type[6]{ "Neverloose", "Gamesense", "Headshoot", "Fatality", "Blick", "Minecraft" };

    const char* desync_autoshoot_type[2]{ "None", "Desync" };

    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(0, 0);
    style->WindowBorderSize = 0;
    style->ItemSpacing = ImVec2(20, 20);
    style->ScrollbarSize = 8.f;

    c::accent = { color_ui[0], color_ui[1], color_ui[2], 1.f };
    DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

    ImGui::SetNextWindowSize(ImVec2(760, 610));
    ImGui::Begin("Input.gg", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        const ImVec2& pos = ImGui::GetWindowPos();
        const ImVec2& spacing = style->ItemSpacing;
        const ImVec2& region = ImGui::GetContentRegionMax();

        ImGui::GetBackgroundd1awList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::bg::background), c::bg::rounding);

        ImGui::GetBackgroundd1awList()->AddRectFilled(pos + spacing, pos + ImVec2(180, 62 + +spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
        ImGui::GetBackgroundd1awList()->AddRect(pos + spacing, pos + ImVec2(180, 62 + spacing.y), ImGui::GetColorU32(c::child::outline), c::child::rounding);

        ImGui::GetBackgroundd1awList()->AddRectFilled(pos + ImVec2(0, 62 + spacing.y) + spacing, pos + ImVec2(180, region.y - spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
        ImGui::GetBackgroundd1awList()->AddRect(pos + ImVec2(0, 62 + spacing.y) + spacing, pos + ImVec2(180, region.y - spacing.y), ImGui::GetColorU32(c::child::outline), c::child::rounding);

        ImGui::GetBackgroundd1awList()->AddRectFilled(pos + ImVec2(180, 0) + spacing, pos + ImVec2(region.x - (spacing.x * 2) - 62, 62 + spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
        ImGui::GetBackgroundd1awList()->AddRect(pos + ImVec2(180, 0) + spacing, pos + ImVec2(region.x - (spacing.x * 2) - 62, 62 + spacing.y), ImGui::GetColorU32(c::child::outline), c::child::rounding);

        ImGui::GetBackgroundd1awList()->AddRectFilled(pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing, pos + ImVec2(region.x - spacing.x, 62 + spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
        ImGui::GetBackgroundd1awList()->AddRect(pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing, pos + ImVec2(region.x - spacing.x, 62 + spacing.y), ImGui::GetColorU32(c::child::outline), c::child::rounding);

        ImGui::GetWindowDrawList()->AddImage(pic::menu_settings_icon, pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing + ImVec2(16, 16), pos + ImVec2(region.x - spacing.x, 62 + spacing.y) - ImVec2(16, 16), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent));

        ImGui::GlowText(font::load_anim_inter_bold, 18.f, "Input.gg", pos + (ImVec2(165, 62 + spacing.y) + spacing - ImGui::CalcTextSize("Input.gg")) / 2, ImGui::GetColorU32(c::accent));

        static int tabs = 0, sub_tabs = 0;

        ImGui::SetCursorPos(ImVec2(spacing.x * 2, 62 + spacing.y * 3));
        ImGui::BeginGroup();
        {
            if (ImGui::Tabs(0 == tabs, pic::aimbot, "Combat", ImVec2(180 - spacing.x * 3, 45), NULL)) tabs = 0;

            if (ImGui::Tabs(1 == tabs, pic::visuals, "Visuals", ImVec2(180 - spacing.x * 3, 45), NULL)) tabs = 1;

            if (ImGui::Tabs(2 == tabs, pic::skins, "Utils", ImVec2(180 - spacing.x * 3, 45), NULL)) tabs = 2;

            if (ImGui::Tabs(3 == tabs, pic::settings, "Menage", ImVec2(180 - spacing.x * 3, 45), NULL)) tabs = 3;
        }
        ImGui::EndGroup();

        if (tabs == 0) {

            ImGui::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
            ImGui::BeginGroup();
            {
                if (ImGui::SubTab(0 == sub_tabs, "General", ImVec2(80, 62))) sub_tabs = 0;
            }
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);

            ImGui::BeginGroup();
            {
                ImGui::CustomBeginChild("Aimbot", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));
                {
                    static int key = 0, m = 0;
                    ImGui::Checkbox(("Silent Aim"), &VarSs::combat::psilent);
                    if (VarSs::combat::psilent) {
                        ImGui::Combo(("Closest Bone"), &VarSs::combat::hitbox, hitbox_type, IM_ARRAYSIZE(hitbox_type), 6);
                        ImGui::Checkbox(("Autoshoot"), &VarSs::combat::autoshoot);
                    }
                    ImGui::Keybindbox(("Desync Shoot"), &VarSs::combat::manipulator, &VarSs::keys::manipulated_key, &m);
                    ImGui::Keybindbox(("test tp"), &VarSs::combat::ZOZO, &VarSs::keys::ZOZO, &m);
                    if (VarSs::combat::manipulator) {
                        ImGui::Checkbox(("Desync Power - Shoot"), &VarSs::combat::instakill);
                        if (VarSs::combat::instakill) {
                            ImGui::SliderFloat(("Power Delay"), &VarSs::combat::insta_kill_value, 0.5f, 1.f);
                        }
                        ImGui::Checkbox(("Desync Auto - Stop"), &VarSs::combat::manipulator_stop);
                        ImGui::Pickerbox(("Desync Radius"), &VarSs::combat::ManipulatorRadios, (float*)&VarSs::colors::MPRadius, NO_ALPHA2);
                        ImGui::Pickerbox(("Circle Visualize"), &VarSs::combat::Player_Circles, (float*)&VarSs::colors::player_circles_color, NO_ALPHA2);
                    }
                    ImGui::Checkbox(("Rotor Hit Heli"), &VarSs::combat::always_heli_rotor);
                    ImGui::Checkbox(("Silent Melee"), &VarSs::combat::silent_melee);
                }
                ImGui::CustomEndChild();

                ImGui::CustomBeginChild("Filters", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f - 40));
                {
                    ImGui::Checkbox(("Draw Fov"), &VarSs::combat::visualize_fov);
                    ImGui::ColorEdit4(("Fov - Color"), (float*)&VarSs::colors::FovCol, NO_ALPHA2);
                    ImGui::ColorEdit4(("Fov Targeted - Color"), (float*)&VarSs::colors::FovCol2, NO_ALPHA2);
                    ImGui::SliderFloat(("Fov Value"), &VarSs::combat::fov, 20.f, 2000.f);
                    ImGui::Checkbox(("Ignore Team"), &VarSs::combat::ignore_team);
                    ImGui::Checkbox(("Ignore Sleep"), &VarSs::combat::ignore_sleepers);
                }
                ImGui::CustomEndChild();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ImGui::CustomBeginChild("Weapon", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 255));
                {
                    static int key = 0, m = 0;

                    ImGui::Checkbox(("Hit Effect - Spawn"), &VarSs::misc::HitEffect);
                    if (VarSs::misc::HitEffect) {
                        ImGui::Combo(("Effect Type"), &VarSs::misc::hitmaterial, hitmaterial_c, IM_ARRAYSIZE(hitmaterial_c), 13);
                    }
                    ImGui::Checkbox(("Hit Random - Spawn"), &VarSs::misc::HitSpawnNearest);

                    ImGui::Checkbox(("No Recoil"), &VarSs::weapons::no_recoil);
                    if (VarSs::weapons::no_recoil) {
                        ImGui::SliderFloat(("Recoil Value"), &VarSs::weapons::recoil_control, 0.f, 100.f);
                    }
                    ImGui::Checkbox(("No Spread"), &VarSs::weapons::no_spread);
                    ImGui::Checkbox(("No Sway"), &VarSs::weapons::no_sway);
                    ImGui::Checkbox(("No Lower"), &VarSs::visuals::No_bobing);
                    ImGui::Checkbox(("No Animation"), &VarSs::weapons::remove_attack_anim);
                    ImGui::Checkbox(("Automatic"), &VarSs::weapons::automatic);
                    ImGui::Keybindbox(("Weapon Spam"), &VarSs::weapons::weapon_spam, &VarSs::keys::weaponspam, &m);
                    ImGui::Checkbox(("Can Hold Items"), &VarSs::weapons::minicopter_aim);
                    ImGui::Checkbox(("Rapid Fire"), &VarSs::weapons::rapidfire);
                    if (VarSs::weapons::rapidfire) {
                        ImGui::SliderFloat(("Rapid Value"), &VarSs::weapons::rapid_value, 0.01f, 0.15f);
                    }
                    ImGui::Checkbox(("Bullet Size"), &VarSs::weapons::size_bullet);
                    if (VarSs::weapons::size_bullet) {
                        ImGui::SliderFloat(("Size Value"), &VarSs::weapons::bullet_size, 5.f, 100.f);
                    }
                    //ImGui::Checkbox(("Hammer Draw"), &VarSs::weapons::hammer_spam);
                    ImGui::Checkbox(("Fast Eoka"), &VarSs::weapons::eokatap);
                    ImGui::Checkbox(("Fast Compound Bow"), &VarSs::weapons::compound);
                    ImGui::Checkbox(("Silent Reload"), &VarSs::combat::autoreload);
                    ImGui::Checkbox(("Jump Attack"), &VarSs::misc::can_attack);
                    ImGui::Checkbox(("Bullet - Teleport"), &VarSs::combat::bullet_tp);
                    if (VarSs::combat::bullet_tp) {
                        ImGui::Pickerbox(("Teleport - Radius"), &VarSs::combat::bullet_tp_radius, (float*)&VarSs::colors::BTspheraCol, NO_ALPHA2);
                        ImGui::Checkbox(("Bullet - Behind Wall"), &VarSs::combat::Manipulator_shot_atBt);
                    }
                    ImGui::Checkbox(("LOS - Pierce"), &VarSs::weapons::penetrate);
                    ImGui::Checkbox(("Fast Bullet"), &VarSs::weapons::fast_bullets);
                    ImGui::Checkbox(("Instant Hit"), &VarSs::misc::circle_hat);
                }
                ImGui::CustomEndChild();
            }
            ImGui::EndGroup();
        }
        if (tabs == 1) {

            ImGui::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
            ImGui::BeginGroup();
            {
                if (ImGui::SubTab(0 == sub_tabs, "General", ImVec2(80, 62))) sub_tabs = 0;
                /* ImGui::SameLine(0, 5);
                 if (ImGui::SubTab(1 == sub_tabs, "World", ImVec2(80, 62))) sub_tabs = 1;*/

            }
            ImGui::EndGroup();

            if (sub_tabs == 0) {

                ImGui::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);

                ImGui::BeginGroup();
                {
                    ImGui::CustomBeginChild("Esp", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));
                    {
                        ImGui::Checkbox(("Name"), &VarSs::players::name);
                        ImGui::Checkbox(("Box"), &VarSs::players::box_rotate);
                        ImGui::Checkbox(("Skeleton"), &VarSs::players::skeleton);
                        ImGui::Checkbox(("Distance"), &VarSs::players::distance);
                        ImGui::Checkbox(("Health bar"), &VarSs::players::healthbar);
                        if (VarSs::players::healthbar) {
                            ImGui::Combo(("Bar Style"), &VarSs::players::healthbarstyle, hp_type, IM_ARRAYSIZE(hp_type), 2);
                        }
                        ImGui::Checkbox(("Weapon"), &VarSs::players::weapon);
                        ImGui::Checkbox(("Weapon Icon"), &VarSs::players::WeaponIcon);
                        ImGui::Checkbox(("Target Lines"), &VarSs::players::targetline);
                        ImGui::Checkbox(("User Belt Bar"), &VarSs::players::TargetBelt);
                        ImGui::Checkbox(("User Wear Bar"), &VarSs::players::TargetWear);
                        ImGui::Checkbox(("OFF Indicator"), &VarSs::players::oof_arrows);
                        if (VarSs::players::oof_arrows) {
                            ImGui::Combo(("OOF Style"), &VarSs::players::out_type, out_type, IM_ARRAYSIZE(out_type), 3);
                        }
                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("Color/Filters", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f - 40));
                    {
                        ImGui::ColorEdit4(("Player - Color"), (float*)&VarSs::visible::box_color, NO_ALPHA2);

                        ImGui::ColorEdit4(("Tracers - Color"), (float*)&VarSs::colors::target_tracers_color, NO_ALPHA2);
                        ImGui::ColorEdit4(("OOF -  Color"), (float*)&VarSs::colors::off_color, NO_ALPHA2);
                        ImGui::ColorEdit4(("ESP - World Color"), (float*)&VarSs::colors::WorldEspColor, NO_ALPHA2);
                        ImGui::Checkbox(("Ignore Sleepers"), &VarSs::players::sleeperignore);
                    }
                    ImGui::CustomEndChild();
                }
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    ImGui::CustomBeginChild("Chams/Filters", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f - 40));
                    {
                        ImGui::Checkbox(("Hand Chams"), &VarSs::visuals::hand_chams);
                        if (VarSs::visuals::hand_chams) {
                            ImGui::Combo(("Hand Type"), &VarSs::visuals::hand_chams_type, hand_chams_type, IM_ARRAYSIZE(hand_chams_type), 13);
                            ImGui::Combo(("Render Mode"), &VarSs::visuals::chams_render_mode, chams_render_mode, IM_ARRAYSIZE(chams_render_mode), 3);
                        }
                        ImGui::Checkbox(("Player Chams"), &VarSs::players::player_chams);
                        if (VarSs::players::player_chams) {
                            ImGui::Combo(("Player Type"), &VarSs::players::player_chams_type, player_chams_type, IM_ARRAYSIZE(player_chams_type), 3);

                            if (VarSs::players::player_chams_type == 0) {
                                ImGui::ColorEdit4(("Visible Color"), (float*)&VarSs::colors::vis_fl_pl, NO_ALPHA2);
                                ImGui::ColorEdit4(("InVisible Color"), (float*)&VarSs::colors::beh_fl_pl, NO_ALPHA2);
                            }
                            if (VarSs::players::player_chams_type == 1) {
                                ImGui::Checkbox(("Frame RGB?"), &VarSs::players::wire_rainbow2);
                                ImGui::ColorEdit4(("Frame Color"), (float*)&VarSs::colors::wire_pl, NO_ALPHA2);
                            }

                            if (VarSs::players::player_chams_type == 2) {
                                ImGui::ColorEdit4(("Visible Color"), (float*)&VarSs::colors::vis_pl2, NO_ALPHA2);
                                ImGui::ColorEdit4(("InVisible Color"), (float*)&VarSs::colors::beh_pl2, NO_ALPHA2);
                            }
                        }
                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("World Visuals", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));
                    {
                        ImGui::Pickerbox(("Bullet Line Colored"), &VarSs::debug::bulletline, (float*)&VarSs::colors::BulletLine, NO_ALPHA2);
                        ImGui::Pickerbox(("Movement Line Colored"), &VarSs::players::playerline, (float*)&VarSs::colors::PlayerLine, NO_ALPHA2);
                        ImGui::Checkbox(("Bullet Line Rainbow"), &VarSs::debug::bulletline2);
                        ImGui::Checkbox(("Movement Line Rainbow"), &VarSs::players::playerline3);
                        ImGui::Pickerbox(("Raycast Sphere"), &VarSs::misc::Atom, (float*)&VarSs::colors::Boxviscol, NO_ALPHA2);
                        ImGui::Pickerbox(("Hit Marker"), &VarSs::misc::hitmarker, (float*)&VarSs::colors::hitmarker, NO_ALPHA2);
                        ImGui::Pickerbox(("Damage Marker"), &VarSs::misc::damagemarker, (float*)&VarSs::colors::damagemarker, NO_ALPHA2);
                        ImGui::Pickerbox(("Colored Ambient"), &VarSs::misc::bright_ambient, (float*)&VarSs::colors::ambient_color, NO_ALPHA2);
                        ImGui::Pickerbox(("Colored Sky"), &VarSs::visuals::skycolor, (float*)&VarSs::colors::sky_color, NO_ALPHA2);
                        ImGui::Checkbox(("Custom Brightness"), &VarSs::misc::brightness_changer);
                        if (VarSs::misc::brightness_changer) {
                            ImGui::SliderFloat(("Brightness Amount"), &VarSs::misc::brightness, 0.f, 100.f, "%.0f");
                        }
                        ImGui::Checkbox(("Custom Rayleigh"), &VarSs::misc::rayleigh_changer);
                        if (VarSs::misc::rayleigh_changer) {
                            ImGui::SliderFloat(("Rayleigh Amount"), &VarSs::misc::rayleigh, 0.f, 100.f, "%.0f");
                        }
                        ImGui::Checkbox(("Custom Mie"), &VarSs::misc::mie_changer);
                        if (VarSs::misc::mie_changer) {
                            ImGui::SliderFloat(("Mie Amount"), &VarSs::misc::mie, 0.f, 100.f, "%.0f");
                        }
                        ImGui::Checkbox(("Custom Stars"), &VarSs::misc::night_stars);
                        if (VarSs::misc::night_stars) {
                            ImGui::SliderFloat(("Stars Amount"), &VarSs::misc::stars, 1.f, 1000.f, "%.0f");
                        }
                        ImGui::Checkbox(("Custom Time"), &VarSs::misc::custom_time);
                        if (VarSs::misc::custom_time) {
                            ImGui::SliderFloat(("Time Value"), &VarSs::misc::time, 0.f, 24.f);
                        }
                    }
                    ImGui::CustomEndChild();
                }
                ImGui::EndGroup();
            }
            if (sub_tabs == 1) {

                ImGui::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);

                ImGui::BeginGroup();
                {
                    ImGui::CustomBeginChild("Crates and Tools", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));
                    {
                        ImGui::Checkbox(("ChinookCrates"), &VarSs::visuals::crates::chinook);
                        ImGui::Checkbox(("Airdrops"), &VarSs::visuals::crates::supply);
                        ImGui::Checkbox(("Heli Crates"), &VarSs::visuals::crates::heli);
                        ImGui::Checkbox(("Bradley Crates"), &VarSs::visuals::crates::bradley);
                        ImGui::Checkbox(("Military Crates"), &VarSs::visuals::crates::military);
                        ImGui::Checkbox(("Elite Crates"), &VarSs::visuals::crates::elite);
                        if (VarSs::visuals::crates::supply || VarSs::visuals::crates::bradley || VarSs::visuals::crates::heli || VarSs::visuals::crates::military || VarSs::visuals::crates::elite) {
                            ImGui::SliderFloat(("Crate ESP Distance"), &VarSs::visuals::crates::draw_distance, 5.f, 3000.f);
                            ImGui::Checkbox(("Show Distance "), &VarSs::visuals::crates::show_distance);
                        }
                        ImGui::Checkbox(("Tool Cupboards"), &VarSs::visuals::base::tc);
                        ImGui::Checkbox(("Sleeping Bags"), &VarSs::visuals::base::sleeping_bag);
                        ImGui::Checkbox(("Beds"), &VarSs::visuals::base::bed);
                        ImGui::Checkbox(("Boxes"), &VarSs::visuals::base::boxes);
                        if (VarSs::visuals::base::tc || VarSs::visuals::base::sleeping_bag || VarSs::visuals::base::bed || VarSs::visuals::base::boxes) {
                            ImGui::SliderFloat(("Base ESP Distance"), &VarSs::visuals::base::draw_distance, 5.f, 400.f);
                            ImGui::Checkbox(("Show Distance     "), &VarSs::visuals::base::show_distance);
                        }
                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("Vehicles and Animals", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f - 40));
                    {
                        ImGui::Checkbox(("Patrol Heli"), &VarSs::visuals::patrol_heli);
                        ImGui::Checkbox(("Minicopters"), &VarSs::visuals::vehicles::minicopter);
                        ImGui::Checkbox(("Scrap Helis"), &VarSs::visuals::vehicles::scrapheli);
                        ImGui::Checkbox(("Boats"), &VarSs::visuals::vehicles::boat);
                        ImGui::Checkbox(("RHIB's"), &VarSs::visuals::vehicles::rhib);
                        if (VarSs::visuals::vehicles::minicopter || VarSs::visuals::vehicles::scrapheli || VarSs::visuals::vehicles::boat || VarSs::visuals::vehicles::rhib) {
                            ImGui::SliderFloat(("Vehicle ESP Distance"), &VarSs::visuals::vehicles::draw_distance, 5.f, 3000.f);
                            ImGui::Checkbox(("Show Distance  "), &VarSs::visuals::vehicles::show_distance);
                        }
                        ImGui::Checkbox(("Bears"), &VarSs::visuals::animals::bear);
                        ImGui::Checkbox(("Pigs"), &VarSs::visuals::animals::pig);
                        ImGui::Checkbox(("Wolfs"), &VarSs::visuals::animals::wolf);
                        ImGui::Checkbox(("Horses"), &VarSs::visuals::animals::deer);
                        ImGui::Checkbox(("Chickens"), &VarSs::visuals::animals::chicken);
                        if (VarSs::visuals::animals::bear || VarSs::visuals::animals::pig || VarSs::visuals::animals::wolf || VarSs::visuals::animals::deer || VarSs::visuals::animals::chicken) {
                            ImGui::SliderFloat(("Animal ESP Distance"), &VarSs::visuals::animals::draw_distance, 5.f, 400.f);
                            ImGui::Checkbox(("Show Distance"), &VarSs::visuals::animals::show_distance);
                        }
                    }
                    ImGui::CustomEndChild();
                }
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    ImGui::CustomBeginChild("Ores and World ", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f - 40));
                    {
                        ImGui::Checkbox(("Stone"), &VarSs::ores::stone);
                        ImGui::Checkbox(("Sulfur"), &VarSs::ores::sulfur);
                        ImGui::Checkbox(("Metal"), &VarSs::ores::metal);
                        if (VarSs::ores::sulfur || VarSs::ores::stone || VarSs::ores::metal) {
                            ImGui::SliderFloat(("Ore ESP Distance"), &VarSs::ores::draw_distance, 5.f, 400.f);
                            ImGui::Checkbox(("Show Collectables"), &VarSs::ores::show_collectables);
                            ImGui::Checkbox(("Show Distance"), &VarSs::ores::show_distance);
                        }
                        ImGui::Checkbox(("Hemp"), &VarSs::visuals::other::hemp);
                        ImGui::Checkbox(("Corpses"), &VarSs::visuals::other::corpse);
                        ImGui::Checkbox(("World Items"), &VarSs::visuals::other::dropped_items);
                        ImGui::Checkbox(("Stashes"), &VarSs::visuals::other::stash);
                        if (VarSs::visuals::other::hemp || VarSs::visuals::other::corpse || VarSs::visuals::other::dropped_items || VarSs::visuals::other::stash) {
                            ImGui::SliderFloat(("World ESP Distance"), &VarSs::visuals::other::draw_distance, 5.f, 400.f);
                            ImGui::Checkbox(("Show Distance"), &VarSs::visuals::other::show_distance);
                        }

                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("Turets", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));
                    {
                        ImGui::Checkbox(("Auto Turrets"), &VarSs::visuals::turrets::auto_turret);
                        ImGui::Checkbox(("Flame Turrets"), &VarSs::visuals::turrets::flame_turret);
                        ImGui::Checkbox(("Shotgun Traps"), &VarSs::visuals::turrets::shotgun_turret);
                        ImGui::Checkbox(("SAM Sites"), &VarSs::visuals::turrets::sam_site);
                        ImGui::Checkbox(("Landmines"), &VarSs::visuals::turrets::landmine);
                        ImGui::Checkbox(("Beartraps"), &VarSs::visuals::turrets::bear_trap);
                        if (VarSs::visuals::turrets::auto_turret || VarSs::visuals::turrets::flame_turret || VarSs::visuals::turrets::shotgun_turret || VarSs::visuals::turrets::landmine || VarSs::visuals::turrets::bear_trap) {
                            ImGui::SliderFloat(("Trap ESP Distance"), &VarSs::visuals::turrets::draw_distance, 5.f, 400.f);
                            ImGui::Checkbox(("Show Distance"), &VarSs::visuals::turrets::show_distance);
                        }
                    }
                    ImGui::CustomEndChild();
                }
                ImGui::EndGroup();
            }
        }
        if (tabs == 2) {

            ImGui::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
            ImGui::BeginGroup();
            {
                if (ImGui::SubTab(0 == sub_tabs, "Player", ImVec2(80, 62))) sub_tabs = 0;
            }

            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);

            ImGui::BeginGroup();
            {
                ImGui::CustomBeginChild("Modify", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 255));
                {
                    static int key = 0, m = 0;

                    ImGui::Checkbox(("Fov View"), &VarSs::misc::fldview);
                    if (VarSs::misc::fldview) {
                        ImGui::SliderFloat(("Fov Value"), &VarSs::misc::fov, 30.f, 120.f);
                    }
                    ImGui::Keybindbox(("Zoom"), &VarSs::misc::zoom, &VarSs::keys::zoom, &m);
                    if (VarSs::misc::zoom) {
                        ImGui::SliderFloat(("Zoom Value"), &VarSs::keys::zoom_value, 15.f, 50.f);
                        
                    }
                    ImGui::Checkbox(("Aspect Ratio"), &VarSs::misc::AspectRatio);
                    if (VarSs::misc::AspectRatio) {
                        ImGui::SliderFloat(("Ratio Value"), &VarSs::misc::ratio, 1.f, 1.8f);
                    }
                    ImGui::Checkbox(("Emotion Spam"), &VarSs::misc::emotion_spamm);
                    if (VarSs::misc::emotion_spamm) {
                        ImGui::Combo(("Emotion"), &VarSs::misc::gesture, geasture_s, IM_ARRAYSIZE(geasture_s), 10);
                    }
                   
                    ImGui::Checkbox(("Auto Med"), &VarSs::misc::faster_healing);
                    ImGui::Checkbox(("Infinity Jump"), &VarSs::misc::better_jump);
                    ImGui::Checkbox(("Spiderman"), &VarSs::misc::spiderman);
                    ImGui::Checkbox(("No Fall Damage"), &VarSs::misc::no_fall);
                 
                    ImGui::Checkbox(("No Sprint Colision"), &VarSs::misc::omnidirectional_sprinting);
                    ImGui::Keybindbox(("SpeedHack"), &VarSs::misc::speedhack, &VarSs::keys::speedkey, &m);
                    if (VarSs::misc::speedhack) {
                        ImGui::SliderFloat(("Speed Value"), &VarSs::misc::speedfloat, 5.f, 25.f);
                    }
                    ImGui::Checkbox(("Hit Sound"), &VarSs::misc::custom_hitsound);
                    if (VarSs::misc::custom_hitsound) {
                        ImGui::Combo(("Sound Type"), &VarSs::misc::custom_hitsound_type, custom_hitsound_type, IM_ARRAYSIZE(custom_hitsound_type), 4);
                    }
                    ImGui::Checkbox(("Fast Loot"), &VarSs::misc::fast_loot);
             
                    /* ImGui::Checkbox(("Spin Bot"), &VarSs::misc::anti_aim);
                     if (VarSs::misc::anti_aim) {
                         ImGui::SliderInt(("Spin Speed"), &VarSs::misc::anti_aim_move, 100.f, 2000.f);
                     }*/
                    ImGui::Checkbox(("Auto Pick Up"), &VarSs::misc::auto_pickup);
                    ImGui::Checkbox(("Admin Flag"), &VarSs::misc::fakeadmin);
                    //ImGui::Checkbox(("Recycler Destroyer"), &VarSs::misc::recycler);
                    ImGui::Keybindbox(("Suicide"), &VarSs::misc::suicide, &VarSs::keys::suicide, &m);
                    ImGui::Keybindbox(("Silent Stash"), &VarSs::misc::silentstash, &VarSs::keys::stasher, &m);
                    ImGui::Keybindbox(("Long Neck"), &VarSs::misc::longneck, &VarSs::keys::longnecking, &m);
                    ImGui::Keybindbox(("Cam Debug"), &VarSs::misc::interactive_debug, &VarSs::keys::debugging, &m);
                    ImGui::Keybindbox(("Teleport Head"), &VarSs::misc::player_tp_head, &VarSs::keys::tp_head_key, &m);
                }
                ImGui::CustomEndChild();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ImGui::CustomBeginChild("Rendering", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 255));
                {
                    static int key = 0, m = 0;

                    ImGui::Checkbox(("Flyhack Indicator"), &VarSs::misc::flyhack_indicator);
                    ImGui::Checkbox(("Reload Indicator"), &VarSs::misc::reload_indicator);
                    ImGui::Checkbox(("Desync Indicator"), &VarSs::misc::manipulator_indicator);
                    ImGui::Checkbox(("Radar On Player"), &VarSs::players::radar);
                    if (VarSs::players::radar) {
                        ImGui::SliderFloat(("Radar Range"), &VarSs::visuals::radar::range, 150.f, 300.f);
                        ImGui::SliderFloat(("Radar Size"), &VarSs::visuals::radar::size, 200.f, 300.f);
                    }
                    ImGui::Checkbox(("Damage Hit Logs"), &VarSs::misc::hit_notification);
                    ImGui::Checkbox(("Bind List"), &VarSs::visuals::keybinds);
                    ImGui::Checkbox(("Crosshair"), &VarSs::crosshair::crosshair);
                    if (VarSs::crosshair::crosshair) {
                        ImGui::Combo(("Crosshair Style"), &VarSs::crosshair::cross_type, cros_type, IM_ARRAYSIZE(cros_type), 3);
                        ImGui::Combo(("Crosshair Color"), &VarSs::crosshair::cross_type_color, cros_type_color, IM_ARRAYSIZE(cros_type_color), 2);
                        if (VarSs::crosshair::cross_type_color == 0) {
                            if (VarSs::crosshair::cross_type == 0) {
                                ImGui::ColorEdit4(("Color Line 1"), (float*)&VarSs::colors::sapphire_color, NO_ALPHA2);
                                ImGui::ColorEdit4(("Color Line 2"), (float*)&VarSs::colors::sapphire_color2, NO_ALPHA2);
                                ImGui::SliderFloat(("Crosshair X"), &VarSs::crosshair::gap, 4.5f, 10.f);
                                ImGui::SliderFloat(("Crosshair Y"), &VarSs::crosshair::length, 4.5f, 10.f);
                            }
                            if (VarSs::crosshair::cross_type == 1) {
                                ImGui::ColorEdit4(("Swaston Color"), (float*)&VarSs::colors::swaston_color, NO_ALPHA2);
                                ImGui::SliderFloat(("Swaston Height"), &VarSs::crosshair::razmer, 5.0f, 10.f);
                                ImGui::SliderFloat(("Swaston Speed"), &VarSs::crosshair::speed, 1.0f, 5.f);
                            }
                            if (VarSs::crosshair::cross_type == 2) {
                                ImGui::ColorEdit4(("Circle Color"), (float*)&VarSs::colors::CrossCol, NO_ALPHA2);
                            }
                        }
                        if (VarSs::crosshair::cross_type_color == 1) {
                            if (VarSs::crosshair::cross_type == 0) {
                                ImGui::SliderFloat(("Crosshair X"), &VarSs::crosshair::gap, 4.5f, 10.f);
                                ImGui::SliderFloat(("Crosshair Y"), &VarSs::crosshair::length, 4.5f, 10.f);
                            }
                            if (VarSs::crosshair::cross_type == 1) {
                                ImGui::SliderFloat(("Swaston Height"), &VarSs::crosshair::razmer, 5.0f, 10.f);
                                ImGui::SliderFloat(("Swaston Speed"), &VarSs::crosshair::speed, 1.0f, 5.f);
                            }
                        }
                    }
                }
                ImGui::CustomEndChild();
            }
            ImGui::EndGroup();
        }
        if (tabs == 3) {

            ImGui::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
            ImGui::BeginGroup();
            {
                if (ImGui::SubTab(0 == sub_tabs, "General", ImVec2(80, 62))) sub_tabs = 0;
            }
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);

            ImGui::BeginGroup();
            {
                ImGui::CustomBeginChild("Config List", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 255));
                {
                    if (ImGui::BeginListBox("List Box", ImGui::GetContentRegionMax() - style->WindowPadding))
                    {

                        for (auto cfg : config.GetConfigList())
                            if (ImGui::Selectable(cfg.c_str(), cfg == VarSs::stuff::selected_cfg))
                                VarSs::stuff::selected_cfg = cfg;

                        ImGui::EndListBox();
                    }
                }
                ImGui::CustomEndChild();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ImGui::CustomBeginChild("Options", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 255));
                {
                    static char config_name[64] = u8"\0";
                    static int key = 0, m = 0;

                    if (ImGui::InputTextEx("TextField", "Enter name ur config", config_name, sizeof(config_name), ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40), NULL));
                    if (ImGui::Button("Load Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                        ImGui::Notification({ ImGuiToastType_Success, 4000, "Config Successful Loaded!" });
                        config.Load(VarSs::stuff::selected_cfg.c_str());
                    }
                    if (ImGui::Button("Open Folder", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                        system(("start Input.gg"));
                    }
                    if (ImGui::Button("Save Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                        ImGui::Notification({ ImGuiToastType_Success, 4000, "Config Successful Saved!" });
                        config.Save(VarSs::stuff::selected_cfg.c_str());
                    }
                    if (ImGui::Button("Create Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                        ImGui::Notification({ ImGuiToastType_Config, 4000, "Config Successful Created!" });
                        VarSs::stuff::selected_cfg = config_name;
                        config.Create();
                    }
                    if (ImGui::Button("Remove Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                        ImGui::Notification({ ImGuiToastType_Warning, 4000, "Config Successful Remove!" });
                        config.Remove(VarSs::stuff::selected_cfg.c_str());
                    }
                    if (ImGui::Button("Unload Cheat", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                        VarSs::stuff::Panic = true;
                    }
                    ImGui::ColorEdit4("UI Color", color_ui, NO_ALPHA2);
                }
                ImGui::CustomEndChild();
            }
            ImGui::EndGroup();
        }

    }
    ImGui::End();
}
void Watermark()
{

    ImGuiStyle* style = &ImGui::GetStyle();

    char name[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(name, &username_len);

    bool info_bar = true;

    const char* cheat_name = "sigma";
    const char* game_name = "pasta";
    const char* game_status = "davit_sosal";
    const char* user = name;

    static float ibar_size = 370;
    static float position = (GetSystemMetrics(SM_CXSCREEN) - ibar_size) / 2;
    position = ImLerp(position, info_bar ? position : GetSystemMetrics(SM_CXSCREEN), ImGui::GetIO().DeltaTime * 10.f);

    if (position <= (GetSystemMetrics(SM_CXSCREEN) - 2)) {

        ImGui::SetNextWindowPos(ImVec2(position, 5));
        ImGui::SetNextWindowSize(ImVec2(ibar_size, 45));

        ImGui::Begin("info-bar", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        {
            const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

            ImGui::GetBackgroundd1awList()->AddRectFilled(pos, pos + ImVec2(ibar_size, 45), ImGui::GetColorU32(c::bg::background_watermark), c::child::rounding);
            ImGui::GetBackgroundd1awList()->AddRectFilled(pos + ImVec2(0, 10), pos + ImVec2(4, 35), ImGui::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_Right);
            ImGui::GetBackgroundd1awList()->AddRectFilled(pos + ImVec2(region.x - 4, 10), pos + ImVec2(region.x, 35), ImGui::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_Left);
            ImGui::GetBackgroundd1awList()->AddRect(pos, pos + ImVec2(ibar_size, 45), ImGui::GetColorU32(c::child::outline), c::child::rounding);

            const char* info_set[4] = { cheat_name, game_name, game_status, user };
            static int info_bar_size = 45;

            ImGui::SetCursorPos(ImVec2(spacing.x, (45 - ImGui::CalcTextSize(user).y) / 2));
            ImGui::BeginGroup();
            {

                for (int i = 0; i < sizeof(info_set) / sizeof(info_set[0]); i++) {
                    ImGui::TextColored(i < 1 ? ImColor(ImGui::GetColorU32(c::accent)) : ImColor(ImGui::GetColorU32(c::text_info_Bar::text_infobar)), info_set[i]);
                    ImGui::SameLine();

                    if (i < 3) {
                        ImGui::TextColored(ImColor(ImGui::GetColorU32(c::text_info_Bar::text_infobar)), "|");
                        ImGui::SameLine();
                    }
                }
            }
            ImGui::EndGroup();
        }
        ImGui::End();
    }
}