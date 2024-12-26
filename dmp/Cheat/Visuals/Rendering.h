#include "menu/imgui/imgui_tricks.hpp"

		static bool toMouse = false;

		void Draw_Perfection_Circle(float r, ImColor Fov_col) {
			auto& io = ImGui::GetIO();

			ImVec2 center = toMouse ? ImVec2(io.MousePos.x, io.MousePos.y) : ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
			auto drawList = ImGui::GetBackgroundd1awList();

			for (int i = 0; i < VarSs::combat::fov; ++i) {
				auto pos = center;
				float angle = (i / static_cast<float>(VarSs::combat::fov)) * 2 * M_PI;
				auto lastPos = ImVec2(pos.x + cos(angle) * r, pos.y + sin(angle) * r);
				auto nextPos = ImVec2(pos.x + cos(angle + 2 * M_PI / VarSs::combat::fov) * r, pos.y + sin(angle + 2 * M_PI / VarSs::combat::fov) * r);					

				float fov_anim = ImTricks::Animations::FastFloatLerp(("fov animation"), VarSs::combat::visualize_fov, 0.f, 1.f, 0.05f);				
				drawList->AddLine(lastPos, nextPos, ImColor(VarSs::colors::FovCol.x, VarSs::colors::FovCol.y, VarSs::colors::FovCol.z, fov_anim), 2.f); // main | главный
			}
		}

ImColor percent_color(float percent) {
	float red, green, blue;
	if (percent < 0.5) {
		red = percent * 2.f * 255.f;
		green = 255.f;
		blue = 0.f;
	}
	else {
		red = 255.f;
		green = (2.f - 2.f * percent) * 255.f;
		blue = 0.f;
	}

	if (percent > 1.f)
		percent = 1.f;
	return ImColor(red,green,blue,255.f);
}
int loadanim;
void Init()
{

	if (loadanim >= 5) {
		VarSs::stuff::InjectAnim = true;
		return;
	}
	// Setup

	auto front = ImGui::GetForegroundd1awList(); // also you can use GetWindowDrawList() or GetBackgroundd1awList()
	ImVec2 center = ImGui::GetIO().DisplaySize / 2.f;
	static ImColor fore_color(0, 255, 0, 255);
	static ImColor back_color(0, 0, 0, 35);
	static float arc_size = 0.45f; // 0.f < x < 2.f
	static float radius = 20.f;
	static float thickness = 2.0f;
	static float AlphaGround = 0.0f;
	int screenWidthX = GetSystemMetrics(SM_CXSCREEN);
	int screenHeightY = GetSystemMetrics(SM_CYSCREEN);
	auto back = ImGui::GetBackgroundd1awList();

	if (AlphaGround < 0.65f)
		AlphaGround += 0.005f;
	back->AddRectFilled(ImVec2(0, 0), ImVec2(screenWidthX, screenHeightY), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, AlphaGround)));

	// Animation
	static float position = 0.f;
	position = ImLerp(position, IM_PI * 2.f, ImGui::GetIO().DeltaTime * 2.3f);

	// Background
	front->PathClear();
	front->PathArcTo(center, radius, 0.f, 2.f * IM_PI, 35.f);
	front->PathStroke(ImGui::GetColorU32(back_color.Value), 0, thickness);

	 //Foreground
	front->PathClear();
	front->PathArcTo(center, radius, IM_PI * 1.5f + position, IM_PI * (1.5f + arc_size) + position, 35.f);
	front->PathStroke(ImGui::GetColorU32(fore_color.Value), 0, thickness);

	//ImGui::GetForegroundd1awList()->AddImage(pic::sapphire_icon, MiddleX - initializ.x / 890 + 890, MiddleY + 580);

	ImGuiStyle* faczad = &ImGui::GetStyle();

	ImGui::PushFont(originalsapphire);
	ImVec2 initializ = ImGui::CalcTextSize((u8"Initialization Sapphire.wtf..."));
	ImVec2 dev = ImGui::CalcTextSize((u8"Good Playing!"));
	ImVec2 good = ImGui::CalcTextSize((u8"Creator - Faczad!"));
	const ImVec2& pos = ImGui::GetWindowPos();
	const ImVec2& spacing = faczad->ItemSpacing;
	const ImVec2& region = ImGui::GetContentRegionMax();
	auto MiddleX = VarSs::stuff::ScreenHeight1 / 2;
	auto MiddleY = VarSs::stuff::ScreenHeight1 / 2;
	//front->AddImage(pic::sapphire_icon, pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing + ImVec2(16, 16), pos + ImVec2(region.x - spacing.x, 62 + spacing.y) - ImVec2(25, 25), ImVec2(25, 25), ImVec2(1, 1), ImGui::GetColorU32(c::accent));
	//front->AddImage({ MiddleX - initializ.x / 890 + 860, MiddleY + 580 });
	front->AddText({ MiddleX - initializ.x / 890 + 890, MiddleY + 580 }, ImColor(255, 255, 255), (u8"Initialization Sapphire.wtf..."));	
	front->AddText({ MiddleX - dev.x / 925 + 925, MiddleY + 600 }, ImColor(255, 255, 255), (u8"Good Playing!"));
	front->AddText({ MiddleX - good.x / 915 + 915, MiddleY + 620 }, ImColor(255, 255, 255), (u8"Creator - Faczad"));
	ImGui::PopFont();

	// Reset animation
	if (position >= IM_PI * 1.90f) {
		position = 0.f;
		loadanim += 1;
	}
}
struct m_keybind
{
	ImDrawList* drawList;
	ImVec2 position;
	const char* m_text;
	int bind_key;
	//Color m_color;
	m_keybind(ImDrawList* drawList, ImVec2 position, const char* text, int bind) : drawList(drawList), position(position), m_text(text), bind_key(bind) {
	}
};
std::vector<m_keybind> m_keybinds;
void Addbind() {
	auto offset = 2;
	for (auto& indicator : m_keybinds) {
		if (GetAsyncKeyState(indicator.bind_key)) {
			indicator.drawList->AddText(font::inter_bold, 15.f, ImVec2(indicator.position.x + 7, indicator.position.y + 25 + 4 + offset), ImColor(255, 255, 255), indicator.m_text);
			indicator.drawList->AddText(font::inter_bold, 15.f, ImVec2(indicator.position.x + 180, indicator.position.y + 25 + 4 + offset), ImColor(146, 148, 155, 255), ("On"));
			offset += 20.f;
		}
	}
	m_keybinds.clear();
}
void keybinds() {
	ImDrawList* drawList_test;
	auto count = 0;
	if (VarSs::combat::manipulator && GetAsyncKeyState(VarSs::keys::manipulated_key))  count++;
	if (VarSs::misc::player_tp_head && GetAsyncKeyState(VarSs::keys::tp_head_key)) count++;
	if (VarSs::misc::longneck && GetAsyncKeyState(VarSs::keys::longnecking)) count++;
	if (VarSs::misc::suicide && GetAsyncKeyState(VarSs::keys::suicide)) count++;
	if (VarSs::misc::zoom && GetAsyncKeyState(VarSs::keys::zoom)) count++;
	if (VarSs::misc::interactive_debug && GetAsyncKeyState(VarSs::keys::debugging)) count++;
	if (VarSs::misc::speedhack && GetAsyncKeyState(VarSs::keys::speedkey)) count++;
	if (VarSs::misc::weapon_spam && GetAsyncKeyState(VarSs::keys::weaponspam)) count++;
	if (VarSs::combat::instakill && GetAsyncKeyState(VarSs::keys::instakill)) count++;
	if (VarSs::weapons::hammer_spam && GetAsyncKeyState(VarSs::keys::hammer_spamm)) count++;
	//if (VarSs::combat::low_hiting && GetAsyncKeyState(VarSs::keys::low_velocity)) count++;
	ImGui::SetNextWindowSize(ImVec2(200, 25 * count + 25));
	ImGui::Begin(("keybinds"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize); {
		auto offset = 2;
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList_test = drawList;
		const ImVec2 origin(ImGui::GetCursorScreenPos());
		keybind::pos = ImGui::GetWindowPos();
		drawList->AddRectFilled(keybind::pos, ImVec2(keybind::pos.x + 200, keybind::pos.y + 25), ImColor(7, 7, 9, 255), 0);//167
		drawList->AddRectFilled(keybind::pos, ImVec2(keybind::pos.x + 200, keybind::pos.y + 2), ImColor(color_ui[0], color_ui[1], color_ui[2]), 0);//167
		auto size_text = font::inter_bold->CalcTextSizeA(15.f, FLT_MAX, NULL, ("Binds"));
		drawList->AddText(font::inter_bold, 15.f, ImVec2(keybind::pos.x + 200 / 2 - size_text.x / 2, keybind::pos.y + 25 / 2 - size_text.y / 2), ImColor(color_ui[0], color_ui[1], color_ui[2]), ("Binds"));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Desync", VarSs::keys::manipulated_key));
		//m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Power Shoot", VarSs::keys::instakill));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Teleport Head", VarSs::keys::tp_head_key));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Long Neck", VarSs::keys::longnecking));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Suicide", VarSs::keys::suicide));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Zoom", VarSs::keys::zoom));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Interactive", VarSs::keys::debugging));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Speed Hack", VarSs::keys::speedkey));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Fake Shoot", VarSs::keys::weaponspam));
		m_keybinds.push_back(m_keybind(drawList, keybind::pos, "Hammer Spam", VarSs::keys::hammer_spamm));
		//m_keybinds.push_back(m_keybind(drawList, keybind::pos, "PVS Atack", VarSs::keys::low_velocity));
	}
	ImGui::End();
}
void pre_draw() {

	static float Alpha = 2500;
	static bool Tick = false;
	static float Speed = 0.5f;

	if (!Tick || Alpha != 2100)
	{
		Tick = false;
		if (!(Alpha >= 2500))
			Alpha += Speed;
		else if (Alpha >= 2500)
			Tick ^= 1;
		if (Alpha >= 2100)
		{
			Alpha = -300;
		}
	}

	const char* _text;
	static float r = 0.50f, g = 0.00f, b = 0.50f;
	static int cases = 0;
	switch (cases)
	{
	case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
	case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
	case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
	case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
	default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
	}

	float xs = VarSs::stuff::ScreenWidth / 2, ys = VarSs::stuff::ScreenHeight / 2;
	auto desynctime = Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime();
	auto desyncpercentage = (((desynctime / 0.95f) * 100.0f) + 1.f) / 100;
	float desynctime2 = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;
	float ManipulatorCircleAnimation = ImTricks::Animations::FastFloatLerp(("ManipulatorCircle animation"), VarSs::combat::ManipulatorRadios  && GetAsyncKeyState(VarSs::keys::manipulated_key) && desyncpercentage>0.1 , 0.f, 1.f, 0.05f);
	float ManipulatorCircleAnimationFill = ImTricks::Animations::FastFloatLerp(("ManipulatorCircleFill animation"),  VarSs::combat::ManipulatorRadios && GetAsyncKeyState(VarSs::keys::manipulated_key) && desyncpercentage > 0.1 , 0.f, 0.4f, 0.05f);
	if (ManipulatorCircleAnimation > 0.005f) {
		//g_Render->DrawRing3D(LocalPlayer::Entity()->transform()->position(), desyncpercentage * 10, 190, ImColor(VarSs::colors::MPRadius.x, VarSs::colors::MPRadius.y, VarSs::colors::MPRadius.z, ManipulatorCircleAnimationFill), ImColor(VarSs::colors::MPRadius.x, VarSs::colors::MPRadius.y, VarSs::colors::MPRadius.z, ManipulatorCircleAnimationFill), 2.f, 0.f, true);
	}
	auto* TargetPlayerA = reinterpret_cast<BasePlayer*>(InGame::stor::closestPlayer);
	if (VarSs::visuals::keybinds) keybinds();
	Addbind();
	ent_loop();
	Init();
	if (VarSs::players::targetline) {
		if (target_player != NULL && target_player->IsValid()) {
			static Vector2 startPos = Vector2(VarSs::stuff::ScreenWidth / 2.f, VarSs::stuff::ScreenHeight / 2.f);
			Vector2 ScreenPos;
			if (!(target_player->get_bone_pos(spine1).x == 0 && target_player->get_bone_pos(spine1).y == 0 && target_player->get_bone_pos(spine1).z == 0)) {
				if (utils::w2s(target_player->get_bone_pos(head), ScreenPos)) {
					g_Render->DrawLine(startPos.x, startPos.y, ScreenPos.x, ScreenPos.y, ImColor(VarSs::colors::target_tracers_color[0], VarSs::colors::target_tracers_color[1], VarSs::colors::target_tracers_color[2]), 1.0f);
				}
			}
		}
	}
	if (VarSs::crosshair::crosshair) {

		static float r = 1.00f, g = 0.00f, b = 1.00f;
		static int cases = 0;
		switch (cases)
		{
		case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
		case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
		case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
		case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
		default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
		}

		if (VarSs::crosshair::cross_type_color == 0) {
			if (VarSs::crosshair::cross_type == 0) {
				g_Render->DrawLine(xs, ys, xs + VarSs::crosshair::gap, ys + VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color2[0], VarSs::colors::sapphire_color2[1], VarSs::colors::sapphire_color2[2], VarSs::colors::sapphire_color2[3]), 2.5f);
				g_Render->DrawLine(xs, ys, xs + VarSs::crosshair::gap, ys - VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color2[0], VarSs::colors::sapphire_color2[1], VarSs::colors::sapphire_color2[2], VarSs::colors::sapphire_color2[3]), 2.5f);
				g_Render->DrawLine(xs, ys, xs - VarSs::crosshair::gap, ys - VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color2[0], VarSs::colors::sapphire_color2[1], VarSs::colors::sapphire_color2[2], VarSs::colors::sapphire_color2[3]), 2.5f);
				g_Render->DrawLine(xs, ys, xs - VarSs::crosshair::gap, ys + VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color2[0], VarSs::colors::sapphire_color2[1], VarSs::colors::sapphire_color2[2], VarSs::colors::sapphire_color2[3]), 2.5f);

				g_Render->DrawLine(xs + VarSs::crosshair::gap, ys + VarSs::crosshair::length, xs + VarSs::crosshair::gap + VarSs::crosshair::length, ys + VarSs::crosshair::length + VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color[0], VarSs::colors::sapphire_color[1], VarSs::colors::sapphire_color[2], VarSs::colors::sapphire_color[3]), 2.5f);
				g_Render->DrawLine(xs + VarSs::crosshair::gap, ys - VarSs::crosshair::length, xs + VarSs::crosshair::gap + VarSs::crosshair::length, ys - VarSs::crosshair::length - VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color[0], VarSs::colors::sapphire_color[1], VarSs::colors::sapphire_color[2], VarSs::colors::sapphire_color[3]), 2.5f);
				g_Render->DrawLine(xs - VarSs::crosshair::gap, ys - VarSs::crosshair::length, xs - VarSs::crosshair::gap - VarSs::crosshair::length, ys - VarSs::crosshair::length - VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color[0], VarSs::colors::sapphire_color[1], VarSs::colors::sapphire_color[2], VarSs::colors::sapphire_color[3]), 2.5f);
				g_Render->DrawLine(xs - VarSs::crosshair::gap, ys + VarSs::crosshair::length, xs - VarSs::crosshair::gap - VarSs::crosshair::length, ys + VarSs::crosshair::length + VarSs::crosshair::length, ImColor(VarSs::colors::sapphire_color[0], VarSs::colors::sapphire_color[1], VarSs::colors::sapphire_color[2], VarSs::colors::sapphire_color[3]), 2.5f);
			}
			if (VarSs::crosshair::cross_type == 1) {
				float xs = VarSs::stuff::ScreenWidth / 2, ys = VarSs::stuff::ScreenHeight / 2;
				static float rotation_degree = 0.f;
				if (rotation_degree > 89.f)
					rotation_degree = 0.f;
				rotation_degree += VarSs::crosshair::speed;
				int length = (int)(VarSs::stuff::ScreenWidth / VarSs::crosshair::razmer / 15);
				float gamma = atan(length / length);
				int i = 0;
				while (i < 4)
				{
					std::vector <int> p
					{
						int(length * sin(Math::GRD_TO_BOG(rotation_degree + (i * 90)))),
						int(length * cos(Math::GRD_TO_BOG(rotation_degree + (i * 90)))),
						int((length / cos(gamma)) * sin(Math::GRD_TO_BOG(rotation_degree + (i * 90) + RAD2DEG(gamma)))),
						int((length / cos(gamma)) * cos(Math::GRD_TO_BOG(rotation_degree + (i * 90) + RAD2DEG(gamma))))
					};
					g_Render->DrawLine(xs, ys, xs + p[0], ys - p[1], ImColor(VarSs::colors::swaston_color.x, VarSs::colors::swaston_color.y, VarSs::colors::swaston_color.z), 2.5f);
					g_Render->DrawLine(xs + p[0], ys - p[1], xs + p[2], ys - p[3], ImColor(VarSs::colors::swaston_color.x, VarSs::colors::swaston_color.y, VarSs::colors::swaston_color.z), 2.5f);
					i++;
				}
			}
			if (VarSs::crosshair::cross_type == 2) {
				//g_Render->DrawTexture(ImVec2(xs, ys), render2::none, ImVec2(65, 65), ImColor(0, 200, 0, 255), reinterpret_cast<void*>(ammo_rifle));
				g_Render->DrawFilledCircle(xs, ys, 2.5, ImColor(VarSs::colors::CrossCol.x, VarSs::colors::CrossCol.y, VarSs::colors::CrossCol.z));
			}
		}
		if (VarSs::crosshair::cross_type_color == 1) {
			if (VarSs::crosshair::cross_type == 0) {
				g_Render->DrawLine(xs, ys, xs + VarSs::crosshair::gap, ys + VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
				g_Render->DrawLine(xs, ys, xs + VarSs::crosshair::gap, ys - VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
				g_Render->DrawLine(xs, ys, xs - VarSs::crosshair::gap, ys - VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
				g_Render->DrawLine(xs, ys, xs - VarSs::crosshair::gap, ys + VarSs::crosshair::length, ImColor(r, g, b), 2.5f);

				g_Render->DrawLine(xs + VarSs::crosshair::gap, ys + VarSs::crosshair::length, xs + VarSs::crosshair::gap + VarSs::crosshair::length, ys + VarSs::crosshair::length + VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
				g_Render->DrawLine(xs + VarSs::crosshair::gap, ys - VarSs::crosshair::length, xs + VarSs::crosshair::gap + VarSs::crosshair::length, ys - VarSs::crosshair::length - VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
				g_Render->DrawLine(xs - VarSs::crosshair::gap, ys - VarSs::crosshair::length, xs - VarSs::crosshair::gap - VarSs::crosshair::length, ys - VarSs::crosshair::length - VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
				g_Render->DrawLine(xs - VarSs::crosshair::gap, ys + VarSs::crosshair::length, xs - VarSs::crosshair::gap - VarSs::crosshair::length, ys + VarSs::crosshair::length + VarSs::crosshair::length, ImColor(r, g, b), 2.5f);
			}
			if (VarSs::crosshair::cross_type == 1) {
				float xs = VarSs::stuff::ScreenWidth / 2, ys = VarSs::stuff::ScreenHeight / 2;
				static float rotation_degree = 0.f;
				if (rotation_degree > 89.f)
					rotation_degree = 0.f;
				rotation_degree += VarSs::crosshair::speed;
				int length = (int)(VarSs::stuff::ScreenWidth / VarSs::crosshair::razmer / 15);
				float gamma = atan(length / length);
				int i = 0;
				while (i < 4)
				{
					std::vector <int> p
					{
						int(length * sin(Math::GRD_TO_BOG(rotation_degree + (i * 90)))),
						int(length * cos(Math::GRD_TO_BOG(rotation_degree + (i * 90)))),
						int((length / cos(gamma)) * sin(Math::GRD_TO_BOG(rotation_degree + (i * 90) + RAD2DEG(gamma)))),
						int((length / cos(gamma)) * cos(Math::GRD_TO_BOG(rotation_degree + (i * 90) + RAD2DEG(gamma))))
					};
					g_Render->DrawLine(xs, ys, xs + p[0], ys - p[1], ImColor(r, g, b), 2.5f);
					g_Render->DrawLine(xs + p[0], ys - p[1], xs + p[2], ys - p[3], ImColor(r, g, b), 2.5f);
					i++;
				}
			}
			if (VarSs::crosshair::cross_type == 2) {
				g_Render->DrawFilledCircle(xs, ys, 2.5, ImColor(r, g, b));
			}
		}
	}
	if (VarSs::misc::indiacators_type == 0) {
		float ManipulatorAnimation = ImTricks::Animations::FastFloatLerp(("ManipulatorInd animation"), VarSs::misc::manipulator_indicator, 0.f, 1.f, 0.05f);
		if (ManipulatorAnimation > 0.001f) {
			float size = 200;
			g_Render->Rect(xs - (size / 2), ys - 30, size, 6.f, ImColor(0.f, 0.f, 0.f, ManipulatorAnimation), 0, 0);
			if (VarSs::misc::manipulator_indicator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
				float ManipulatorAnimationac = ImTricks::Animations::FastFloatLerp(("ManipulatorInd Active animation"), VarSs::misc::manipulator_indicator && desyncpercentage > 0.001, 0.f, 1.f, 0.05f);

				if (ManipulatorAnimationac > 0.005) {
					float size = 200;
					float x = (desynctime * size);
					if (x > (size) * 2) x = (size) * 2;

					g_Render->FilledRect(xs - (size / 2) + 1, ys - 30 + 1, x - 2, 6.f - 2, g_Render->PercentColor(desyncpercentage, ManipulatorAnimationac), 0, 0);
				}
			}			
		}
		float FlyhackAnimation = ImTricks::Animations::FastFloatLerp(("Flyhack animation"), VarSs::misc::flyhack_indicator, 0.f, 1.f, 0.05f);

		if (FlyhackAnimation > 0.001f) {
			float barv = 1.5f;
			g_Render->Rect(screen_center.x - 180, barv + 85, 350, 10, ImColor(0.f, 0.f, 0.f, FlyhackAnimation), 0, 0);
			g_Render->Rect(screen_center.x - 180, barv + 65, 350, 10, ImColor(0.f, 0.f, 0.f, FlyhackAnimation), 0, 0);
			//g_Render->DrawTexture(ImVec2(screen_center.x + 500, screen_center.y + 500), render2::none, ImVec2(65, 65), ImColor(0, 200, 0, 255), reinterpret_cast<void*>(ammo_rifle));
			float v, h;
			v = VFlyhack / (VMaxFlyhack - 1.0f);
			h = HFlyhack / (HMaxFlyhack - 1.0f);
			g_Render->FilledRect(screen_center.x - 180, barv + 85, 350 * (v >= 1.0f ? 1.0f : v), 10, g_Render->PercentColor(v, FlyhackAnimation), 0);
			g_Render->FilledRect(screen_center.x - 180, barv + 65, 350 * (h >= 1.0f ? 1.0f : h), 10, g_Render->PercentColor(h, FlyhackAnimation), 0);
		}
		int reload_anim = ImTricks::Animations::FastIntLerp(("ReloadInd animation"), VarSs::misc::reload_indicator, 0, 255, 15);
		if (reload_anim > 1) {
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			float size = 150;
			if (held) {
				if (held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {


					if (did_reload == false && time_since_last_shot <= (held->reloadTime() - (held->reloadTime() / 10)) && time_since_last_shot > 0) {
						g_Render->Rect(xs - (size / 2), ys - 37, size, 6.f, ImColor(0, 0, 0, reload_anim), 0, 0);
						float reloadDurationTotal = (held->reloadTime() - (held->reloadTime() / 10));
						float reloadDuration = time_since_last_shot;
						auto percentage = (((reloadDuration / reloadDurationTotal) * 100.0f) + 1.f) / 100;
						int reload_anim_active = ImTricks::Animations::FastIntLerp("ReloadInd1 animation", VarSs::misc::reload_indicator && percentage != 1, 0, 255, 15);

						if (reload_anim_active > 1) {
							ImVec2 center(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2);
							float size = 150;
							float x = (percentage * size);

							if (x > (size) * 2)
								x = (size) * 2; //s
							ImVec2 icon_size = ImVec2(80, 80);
							//g_Render->DrawTexture(ImVec2(xs + 500, ys + 500), render2::centered_x, icon_size, ImColor(VarSs::colors::weapon_color_player[0], VarSs::colors::weapon_color_player[1], VarSs::colors::weapon_color_player[2]), reinterpret_cast<void*>(get_icon_name(BP).texture.texture));
							g_Render->FilledRect(xs - (size / 2) + 1, ys - 37 + 1, x - 2, 6.f - 2, ImColor(0, 200, 0, reload_anim_active), 0, 0);
						}
					}
					else {
						if (held->HasReloadCooldown()) {
							g_Render->Rect(xs - (size / 2), ys - 37, size, 6.f, ImColor(0, 0, 0, reload_anim), 0, 0);
							float SamReloadDuration = held->nextReloadTime() - GLOBAL_TIME;
							float SamReloadDurationTotal = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
							auto SamPercentage = (((SamReloadDuration / SamReloadDurationTotal) * 100.0f) + 1.f) / 100;
							int reload_anim_active = ImTricks::Animations::FastIntLerp(("ReloadInd2 animation"), VarSs::misc::reload_indicator && SamPercentage != 1, 0, 255, 15);

							if (reload_anim_active > 1) {
								ImVec2 center(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2);
								float size = 150;
								float x = (SamPercentage * size);
								if (x > (size) * 2)
									x = (size) * 2; //s
								g_Render->FilledRect(xs - (size / 2) + 1, ys - 37 + 1, x - 2, 6.f - 2, ImColor(0, 200, 0, reload_anim_active), 0, 0);								
							}
						}
					}
				}
			}
		}
	}
	if (VarSs::misc::indiacators_type == 1) {
		float ManipulatorAnimation = ImTricks::Animations::FastFloatLerp(("ManipulatorInd animation"), VarSs::misc::manipulator_indicator, 0.f, 1.f, 0.05f);
		if (ManipulatorAnimation > 0.001f) {
			float size = 200;
			g_Render->FilledRect(xs - (size / 2), ys - 30, size, 6.f, ImColor(0.f, 0.f, 0.f, ManipulatorAnimation), 0, 0);
			if (VarSs::misc::manipulator_indicator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
				float ManipulatorAnimationac = ImTricks::Animations::FastFloatLerp(("ManipulatorInd Active animation"), VarSs::misc::manipulator_indicator && desyncpercentage > 0.001, 0.f, 1.f, 0.05f);

				if (ManipulatorAnimationac > 0.005) {
					float size = 200;
					float x = (desynctime * size);
					if (x > (size) * 2) x = (size) * 2;

					g_Render->FilledRect(xs - (size / 2) + 1, ys - 30 + 1, x - 2, 6.f - 2, g_Render->PercentColor(desyncpercentage, ManipulatorAnimationac), 0, 0);
				}
			}
		}
		float FlyhackAnimation = ImTricks::Animations::FastFloatLerp(("Flyhack animation"), VarSs::misc::flyhack_indicator, 0.f, 1.f, 0.05f);

		if (FlyhackAnimation > 0.001f) {
			float barv = 1.5f;
			g_Render->FilledRect(screen_center.x - 180, barv + 85, 350, 10, ImColor(0.f, 0.f, 0.f, FlyhackAnimation), 0, 0);
			g_Render->FilledRect(screen_center.x - 180, barv + 65, 350, 10, ImColor(0.f, 0.f, 0.f, FlyhackAnimation), 0, 0);
			float v, h;
			v = VFlyhack / (VMaxFlyhack - 1.0f);
			h = HFlyhack / (HMaxFlyhack - 1.0f);
			g_Render->FilledRect(screen_center.x - 180, barv + 85, 350 * (v >= 1.0f ? 1.0f : v), 10, g_Render->PercentColor(v, FlyhackAnimation), 0);
			g_Render->FilledRect(screen_center.x - 180, barv + 65, 350 * (h >= 1.0f ? 1.0f : h), 10, g_Render->PercentColor(h, FlyhackAnimation), 0);
		}
		int reload_anim = ImTricks::Animations::FastIntLerp(("ReloadInd animation"), VarSs::misc::reload_indicator, 0, 255, 15);
		if (reload_anim > 1) {
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			float size = 150;
			if (held) {
				if (held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {


					if (did_reload == false && time_since_last_shot <= (held->reloadTime() - (held->reloadTime() / 10)) && time_since_last_shot > 0) {
						g_Render->FilledRect(xs - (size / 2), ys - 37, size, 6.f, ImColor(0, 0, 0, reload_anim), 0, 0);
						float reloadDurationTotal = (held->reloadTime() - (held->reloadTime() / 10));
						float reloadDuration = time_since_last_shot;
						auto percentage = (((reloadDuration / reloadDurationTotal) * 100.0f) + 1.f) / 100;
						int reload_anim_active = ImTricks::Animations::FastIntLerp("ReloadInd1 animation", VarSs::misc::reload_indicator && percentage != 1, 0, 255, 15);

						if (reload_anim_active > 1) {
							ImVec2 center(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2);
							float size = 150;
							float x = (percentage * size);

							if (x > (size) * 2)
								x = (size) * 2; //s
							g_Render->FilledRect(xs - (size / 2) + 1, ys - 37 + 1, x - 2, 6.f - 2, ImColor(0, 200, 0, reload_anim_active), 0, 0);
						}
					}
					else {
						if (held->HasReloadCooldown()) {
							g_Render->FilledRect(xs - (size / 2), ys - 37, size, 6.f, ImColor(0, 0, 0, reload_anim), 0, 0);
							float SamReloadDuration = held->nextReloadTime() - GLOBAL_TIME;
							float SamReloadDurationTotal = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
							auto SamPercentage = (((SamReloadDuration / SamReloadDurationTotal) * 100.0f) + 1.f) / 100;
							int reload_anim_active = ImTricks::Animations::FastIntLerp(("ReloadInd2 animation"), VarSs::misc::reload_indicator && SamPercentage != 1, 0, 255, 15);

							if (reload_anim_active > 1) {
								ImVec2 center(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2);
								float size = 150;
								float x = (SamPercentage * size);
								if (x > (size) * 2)
									x = (size) * 2; //s
								g_Render->FilledRect(xs - (size / 2) + 1, ys - 37 + 1, x - 2, 6.f - 2, ImColor(0, 200, 0, reload_anim_active), 0, 0);

							}
						}
					}
				}
			}
		}
	}
	if (VarSs::misc::indiacators_type == 2) {
		float ManipulatorAnimation = ImTricks::Animations::FastFloatLerp(("ManipulatorInd animation"), VarSs::misc::manipulator_indicator, 0.f, 1.f, 0.05f);
		if (ManipulatorAnimation > 0.001f) {
			float size = 200;
			g_Render->FilledRect(xs - (size / 2), ys - 30, size, 6.f, ImColor(0.f, 0.f, 0.f, ManipulatorAnimation), 0, 0);
			if (VarSs::misc::manipulator_indicator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
				float ManipulatorAnimationac = ImTricks::Animations::FastFloatLerp(("ManipulatorInd Active animation"), VarSs::misc::manipulator_indicator && desyncpercentage > 0.001, 0.f, 1.f, 0.05f);

				if (ManipulatorAnimationac > 0.005) {
					float size = 200;
					float x = (desynctime * size);
					if (x > (size) * 2) x = (size) * 2;

					g_Render->FilledRectGradient(xs - (size / 2) + 1, ys - 30 + 1, x - 2, 6.f - 2, g_Render->PercentColor(desyncpercentage, ManipulatorAnimationac), ImColor(0, 0, 0, 0), 0, 0);
				}
			}			
		}
		float FlyhackAnimation = ImTricks::Animations::FastFloatLerp(("Flyhack animation"), VarSs::misc::flyhack_indicator, 0.f, 1.f, 0.05f);

		if (FlyhackAnimation > 0.001f) {
			float barv = 1.5f;
			g_Render->FilledRect(screen_center.x - 180, barv + 85, 350, 10, ImColor(0.f, 0.f, 0.f, FlyhackAnimation), 0, 0);
			g_Render->FilledRect(screen_center.x - 180, barv + 65, 350, 10, ImColor(0.f, 0.f, 0.f, FlyhackAnimation), 0, 0);
			float v, h;
			v = VFlyhack / (VMaxFlyhack - 1.0f);
			h = HFlyhack / (HMaxFlyhack - 1.0f);
			g_Render->FilledRectGradient(screen_center.x - 180, barv + 85, 350 * (v >= 1.0f ? 1.0f : v), 10, g_Render->PercentColor(v, FlyhackAnimation), ImColor(0, 0, 0, 0), 0, 0);
			g_Render->FilledRectGradient(screen_center.x - 180, barv + 65, 350 * (h >= 1.0f ? 1.0f : h), 10, g_Render->PercentColor(h, FlyhackAnimation), ImColor(0, 0, 0, 0), 0, 0);
		}
		int reload_anim = ImTricks::Animations::FastIntLerp(("ReloadInd animation"), VarSs::misc::reload_indicator, 0, 255, 15);
		if (reload_anim > 1) {
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			float size = 150;
			if (held) {
				if (held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {


					if (did_reload == false && time_since_last_shot <= (held->reloadTime() - (held->reloadTime() / 10)) && time_since_last_shot > 0) {
						g_Render->FilledRect(xs - (size / 2), ys - 37, size, 6.f, ImColor(0, 0, 0, reload_anim), 0, 0);
						float reloadDurationTotal = (held->reloadTime() - (held->reloadTime() / 10));
						float reloadDuration = time_since_last_shot;
						auto percentage = (((reloadDuration / reloadDurationTotal) * 100.0f) + 1.f) / 100;
						int reload_anim_active = ImTricks::Animations::FastIntLerp("ReloadInd1 animation", VarSs::misc::reload_indicator && percentage != 1, 0, 255, 15);

						if (reload_anim_active > 1) {
							ImVec2 center(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2);
							float size = 150;
							float x = (percentage * size);

							if (x > (size) * 2)
								x = (size) * 2; //s
							g_Render->FilledRect(xs - (size / 2) + 1, ys - 37 + 1, x - 2, 6.f - 2, ImColor(0, 200, 0, reload_anim_active), 0, 0);
						}
					}
					else {
						if (held->HasReloadCooldown()) {
							g_Render->FilledRect(xs - (size / 2), ys - 37, size, 6.f, ImColor(0, 0, 0, reload_anim), 0, 0);
							float SamReloadDuration = held->nextReloadTime() - GLOBAL_TIME;
							float SamReloadDurationTotal = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
							auto SamPercentage = (((SamReloadDuration / SamReloadDurationTotal) * 100.0f) + 1.f) / 100;
							int reload_anim_active = ImTricks::Animations::FastIntLerp(("ReloadInd2 animation"), VarSs::misc::reload_indicator && SamPercentage != 1, 0, 255, 15);

							if (reload_anim_active > 1) {
								ImVec2 center(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2);
								float size = 150;
								float x = (SamPercentage * size);
								if (x > (size) * 2)
									x = (size) * 2; //s
								g_Render->FilledRectGradient(xs - (size / 2) + 1, ys - 37 + 1, x - 2, 6.f - 2, ImColor(0, 200, 0, reload_anim_active), ImColor(0, 0, 0, 0), 0, 0);

							}
						}
					}
				}
			}
		}
	}	
	float crosshair_anim = ImTricks::Animations::FastFloatLerp(("Crosshair animation"), VarSs::misc::crosshair, 0.f, 1.f, 0.05f);
	if (crosshair_anim > 0.01f) {
		//	"~manipulated~"
		g_Render->DrawFilledCircle(xs, ys, 2.5, ImColor(VarSs::colors::CrossCol.x, VarSs::colors::CrossCol.y, VarSs::colors::CrossCol.z, crosshair_anim));
	}
	float fov_anim = ImTricks::Animations::FastFloatLerp(("fov animation"), VarSs::combat::visualize_fov, 0.f, 1.f, 0.05f);
	if (fov_anim > 0.01f) {

		Draw_Perfection_Circle(VarSs::combat::fov, ImColor(VarSs::colors::FovCol.x, VarSs::colors::FovCol.y, VarSs::colors::FovCol.z, fov_anim));		

		if (target_player) {

			Draw_Perfection_Circle(VarSs::combat::fov, ImColor(VarSs::colors::FovCol2.x, VarSs::colors::FovCol2.y, VarSs::colors::FovCol2.z, fov_anim));
		}
	}

	int pos = 0;

	float OverFunc_anim = ImTricks::Animations::FastFloatLerp((" OverFunc animation"), VarSs::stuff::OverFunc, 0.f, 1.f, 0.08f);
if (OverFunc_anim > 0.08f) {
    if (VarSs::stuff::OverFunc) {
    
		if (VarSs::combat::manipulator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, ("~Manipulated~"));
			pos += 15;
		}
		if (VarSs::weapons::penetrate && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 0, 0), render2::outline | render2::centered_x, 2, 10, ("LineOfSight Scanning..."));
			pos += 15;
		}
		if (VarSs::weapons::fast_bullets && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(0, 80, 255), render2::outline | render2::centered_x, 2, 10, ("~Low - Velocity Ready~"));
			pos += 15;
		}
		if (VarSs::combat::bullet_tp && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(0, 80, 255), render2::outline | render2::centered_x, 2, 10, ("~Bullet - TP~"));
			pos += 15;
		}
		if (VarSs::weapons::weapon_spam && GetAsyncKeyState(VarSs::keys::weaponspam)) {
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, ("Weapon Spaming..."));
			pos += 15;
		}
		if (VarSs::misc::TeloportTopPlayer && GetAsyncKeyState(VarSs::keys::TeleportTopPlayer) && Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), target_player->get_bone_pos(head)) < 2.0f)
		{
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, ("Head Teleported..."));
			pos += 15;
		}
		if (VarSs::misc::speedhack && GetAsyncKeyState(VarSs::keys::speedkey) && Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), target_player->get_bone_pos(head)) < 2.0f)
		{
			g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, ("Speed Hacking..."));
			pos += 15;
		}
	/*if (VarSs::weapons::thick_bullet) {
		g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, ("~Thick-Bullet~"));
		pos += 15;
	}*/
	/*if (VarSs::combat::doubletap) {
		g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 0, 255), render2::outline | render2::centered_x, 2, 10, ("~Double-Tap~"));
		pos += 15;
	}*/
	/*if (VarSs::weapons::penetrate) {
		g_Render->DrawString(xs, ys + 20 + pos, ImColor(0, 255, 3), render2::outline | render2::centered_x, 2, 10, ("~Trought-Shoot~"));
		pos += 15;
	}*/
		/*if (VarSs::combat::visualize_prediction && TargetPlayerA->IsValid()) {
			Vector2 t;
			if (!(TargetPlayerA->get_bone_pos(spine1).x == 0 && TargetPlayerA->get_bone_pos(spine1).y == 0 && TargetPlayerA->get_bone_pos(spine1).z == 0)) {
				if (utils::w2s(a::get_aim_point(GetBulletSpeed(), GetGravity(LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo())), t)) {
					g_Render->DrawString(t.x, t.y, ImColor(VarSs::colors::prediction_color), render2::outline | render2::centered_x, pixel_s, 11, ("X"));

				}
			}
		}*/
	/*if (VarSs::weapons::fast_bullets && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
		g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 0), render2::outline | render2::centered_x, 2, 10, ("Fast-Hit"));
		pos += 15;
	}
	if (VarSs::misc::TeloportTopPlayer && GetAsyncKeyState(VarSs::keys::TeleportTopPlayer) && Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), target_player->get_bone_pos(head)) < 2.0f)
	{
		g_Render->DrawString(xs, ys + 20 + pos, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, ("~Teleported~"));
		pos += 15;
	}*/
    }
}
    int w = 200, h = 80;
	int wz = 300, hz = 250;

	if (VarSs::players::TargetBelt && show)
	{
		belt::belt_tab_mov(Vector2(w, h + 20.0f));
		g_Render->FilledRect(belt::pos.x, belt::pos.y + 20.0f, w, h, ImColor(7, 7, 9, 255), 3.f, ImDrawFlags_RoundCornersBottom);
		g_Render->DrawString(belt::pos.x + 100, belt::pos.y + 40, ImColor(255, 255, 255), render2::none | render2::centered_x, 1, 25, ("Belt Position"));
	}
	if (VarSs::players::TargetWear && show)
	{
		wear::wear_tab_mov(Vector2(w, h + 20.0f));
		g_Render->FilledRect(wear::pos.x, wear::pos.y + 20.0f, w, h, ImColor(7, 7, 9, 255), 3.f, ImDrawFlags_RoundCornersBottom);
		g_Render->DrawString(wear::pos.x + 100, wear::pos.y + 40, ImColor(255, 255, 255), render2::none | render2::centered_x, 1, 25, ("Wear Position"));
	}	
}
void ImGuiRendering::BeginScene()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("##Backbuffer", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), 0);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), 0);
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	pre_draw();
	hitmarker::get().draw_hits(window->DrawList);
}
void ImGuiRendering::EndScene()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();
	                                           
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}
