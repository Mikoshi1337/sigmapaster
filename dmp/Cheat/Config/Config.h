#include <string>
#include <vector>
#include <Windows.h>
#include <filesystem>
#include "Base63.h"
//static float color_ui[4] = { 255.f, 255.f, 255.f, 255.f };

template <typename T>
class VarType
{
public:
	VarType(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
public:
	CConfig()
	{
		ints = std::vector<VarType<int>*>();
		bools = std::vector<VarType<bool>*>();
		floats = std::vector<VarType<float>*>();
	}

	void Initialize()
	{
		szPath = "Sapphire.wtf\\";
		Setup();
	}
	void Create()
	{
		std::string file;
		file = szPath + VarSs::stuff::selected_cfg + ".cfg";

		CreateDirectoryA(szPath.c_str(), NULL);

		for (VarType<int>* pValue : ints)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<float>* pValue : floats)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<bool>* pValue : bools)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), *pValue->value ? "1" : "0", file.c_str());
	}
	std::vector < std::string > GetConfigList()
	{
		std::filesystem::create_directory("Sapphire.wtf");
		/*std::filesystem::create_directory("Sapphire.wtf\\");*/

		std::vector < std::string > m_ConfigList;
		for (const auto& p : std::filesystem::recursive_directory_iterator(("Sapphire.wtf\\")))
		{
			if (!std::filesystem::is_directory(p) && p.path().extension().string() == (".cfg"))
			{
				auto file = p.path().filename().string();
				m_ConfigList.push_back(file.substr(0, file.size() - 4));
			}
		}

		return m_ConfigList;
	}
	void Save(const char* szConfigName)
	{
		std::string file;
		file = szPath + szConfigName + ".cfg";

		CreateDirectoryA(szPath.c_str(), NULL);

		for (VarType<int>* pValue : ints)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<float>* pValue : floats)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<bool>* pValue : bools)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), *pValue->value ? "1" : "0", file.c_str());
	}

	void Remove(const char* szConfigName)
	{
		std::string file;
		file = szPath + szConfigName + ".cfg";
		std::remove(file.c_str());
	}

	void Load(const char* szConfigName)
	{
		std::string file;
		file = szPath + szConfigName + ".cfg";
		CreateDirectoryA(szPath.c_str(), NULL);

		char value_l[32] = { '\0' };

		for (VarType<int>* pValue : ints)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = atoi(value_l);
		}

		for (VarType<float>* pValue : floats)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = (float)atof(value_l);
		}

		for (VarType<bool>* pValue : bools)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = !strcmp(value_l, "1");
		}
	}

private:
	void SetupValue(int& value, int def, std::string category, std::string name)
	{
		value = def;
		ints.push_back(new VarType<int>(category, name, &value));
	}

	void SetupValue(bool& value, bool def, std::string category, std::string name)
	{
		value = def;
		bools.push_back(new VarType<bool>(category, name, &value));
	}

	void SetupValue(float& value, float def, std::string category, std::string name)
	{
		value = def;
		floats.push_back(new VarType<float>(category, name, &value));
	}


	void Setup()
	{
		SetupValue(VarSs::stuff::outline_font, false, ("Stuff"), ("outline_font"));
		SetupValue(VarSs::stuff::unload_on_key, false, ("Stuff"), ("unload_on_key"));
		SetupValue(VarSs::stuff::unload_key, 0, ("Stuff"), ("unload_key"));
		SetupValue(VarSs::stuff::font, 0, ("Stuff"), ("Font ESP"));
		SetupValue(VarSs::stuff::size_font, 12.f, ("Stuff"), ("size_font"));
		SetupValue(VarSs::stuff::size_font2, 10.f, ("Stuff"), ("size_font2"));

		// --Keys
		SetupValue(VarSs::keys::longnecking , 0, ("keys"), ("longnecking"));
		SetupValue(VarSs::keys::low_velocity, 0, ("keys"), ("low_velocity"));
		SetupValue(VarSs::keys::fast_key, 0, ("keys"), ("fast_key"));
		SetupValue(VarSs::keys::tp_head_key, 0, ("keys"), ("tp_head_key"));
		SetupValue(VarSs::keys::debugging, 0, ("keys"), ("debugging"));
		SetupValue(VarSs::keys::zoom, 0, ("keys"), ("zoom"));
		SetupValue(VarSs::keys::zoom_value, 15.f, ("keys"), ("zoom_value"));
		SetupValue(VarSs::keys::suicide, 0, ("keys"), ("suicide"));
		SetupValue(VarSs::keys::manipulated_key, 0, ("keys"), ("manipulated_key"));
		SetupValue(VarSs::keys::ignore_stopper, 0, ("keys"), ("ignore_stopper"));
		SetupValue(VarSs::keys::stashopen, 0, ("keys"), ("stashopen"));
		SetupValue(VarSs::keys::TeleportTopPlayer, 0, ("keys"), ("TeleportTopPlayer key"));

		SetupValue(VarSs::keys::desync, 0, ("keys"), ("desync"));
		SetupValue(VarSs::keys::speedkey, 0, ("keys"), ("speedkey"));
		SetupValue(VarSs::keys::instakill, 0, ("keys"), ("instakill"));
		SetupValue(VarSs::keys::psilentkey, 0, ("keys"), ("psilentkey"));

		// --Combat
		SetupValue(VarSs::combat::ExtentForMoutend, false, ("Combat"), ("ExtentForMoutend"));
		SetupValue(VarSs::weapons::wall_shoot, false, ("Combat"), ("wall_shoot"));
		SetupValue(VarSs::combat::ManipulatorRadios, false, ("Combat"), ("ManipulatorRadios"));
		SetupValue(VarSs::combat::bullet_tp_arrows, false, ("Combat"), ("bullet_tp_arrows"));
		SetupValue(VarSs::combat::BulletTeleportGradient, false, ("Combat"), ("BulletTeleportGradient"));
		SetupValue(VarSs::combat::pierce, false, ("Combat"), ("pierce"));
		SetupValue(VarSs::combat::low_hiting, false, ("Combat"), ("low_hiting"));
		SetupValue(VarSs::weapons::penetrate, false, ("weapons"), ("penetrate"));
		SetupValue(VarSs::combat::doubletap, false, ("Combat"), ("doubletap"));
		SetupValue(VarSs::combat::doubletaptype, 0, ("Combat"), ("doubletaptype"));
		SetupValue(VarSs::combat::pvs, 0, ("Combat"), ("pvs"));

		SetupValue(VarSs::weapons::roundfire, false, ("Combat"), ("roundfire"));
		SetupValue(VarSs::combat::bullet_tp_radius, false, ("Combat"), ("bullet_tp_radius"));
		SetupValue(VarSs::combat::bullet_tp, false, ("Combat"), ("bullet_tp"));
		SetupValue(VarSs::combat::bullet_tp_type, 0, ("Combat"), ("bullet_tp_type"));
		SetupValue(VarSs::combat::hitscan_tp_type, 0, ("Combat"), ("hitscan_tp_type"));
		SetupValue(VarSs::combat::hitscan_tp_mode, 0, ("Combat"), ("hitscan_tp_mode"));
		SetupValue(VarSs::combat::manipulator_vectors_type, 0, ("Combat"), ("manipulator_vectors_type"));
		SetupValue(VarSs::combat::bullet_tp_range, 1.f, ("Combat"), ("bullet_tp_range"));
		SetupValue(VarSs::combat::autoreload, false, ("Combat"), ("autoreload"));
		SetupValue(VarSs::combat::instakill, false, ("Combat"), ("instakill"));
		SetupValue(VarSs::combat::burst_fire, false, ("Combat"), ("burst_fire"));
		SetupValue(VarSs::combat::DesyncSpawnDelay, 0.15f, ("Combat"), ("DesyncSpawnDelay"));
		SetupValue(VarSs::combat::insta_kill_value, 10.f, ("Combat"), ("insta_kill_value"));
		SetupValue(VarSs::combat::theta_value, 2.0f, ("Combat"), ("theta_value"));
		SetupValue(VarSs::combat::point_x_value, 1.0f, ("Combat"), ("point_x_value"));
		SetupValue(VarSs::combat::point_y_value, 2.5f, ("Combat"), ("point_y_value"));
		SetupValue(VarSs::combat::points_dync_generate, 10.f, ("Combat"), ("points_dync_generate"));
		SetupValue(VarSs::combat::points_rota_generate, 100.f, ("Combat"), ("points_rota_generate"));
		SetupValue(VarSs::combat::instanthit, false, ("Combat"), ("instanthit"));
		SetupValue(VarSs::combat::desync, false, ("Combat"), ("desync"));
		SetupValue(VarSs::combat::autoshoot, false, ("Combat"), ("autoshoot"));
		SetupValue(VarSs::combat::always_heli_rotor, false, ("Combat"), ("always_heli_rotor"));
		SetupValue(VarSs::combat::HitScan, false, ("Combat"), ("HitScan"));
		SetupValue(VarSs::combat::silent_melee, false, ("Combat"), ("silent_melee"));
		SetupValue(VarSs::combat::psilent, false, ("Combat"), ("psilent"));
		SetupValue(VarSs::combat::silentaim, false, ("Combat"), ("silentaim"));		
		SetupValue(VarSs::combat::psilentheli, false, ("Combat"), ("psilentheli"));
		SetupValue(VarSs::combat::visualize_fov, false, ("Combat"), ("visualize_fov"));
		SetupValue(VarSs::combat::manipulator, false, ("Combat"), ("manipulator"));
		SetupValue(VarSs::combat::auto_manipulator, false, ("combat"), ("auto_manipulator"));
		SetupValue(VarSs::combat::auto_m_dist, 0.1f, ("Combat"), ("auto_m_dist"));
		SetupValue(VarSs::combat::manipulator_stop, false, ("Combat"), ("manipulator_stop"));
		SetupValue(VarSs::combat::ignore_sleepers, false, ("Combat"), ("ignore_sleepers"));
		SetupValue(VarSs::combat::ignore_team, false, ("Combat"), ("ignore_team"));
		SetupValue(VarSs::combat::ignore_npc, true, ("Combat"), ("ignore_npc"));
		SetupValue(VarSs::combat::hitbox, 0, ("Combat"), ("HitBox_ov"));
		SetupValue(VarSs::misc::insta_indicator, 0, ("Combat"), ("insta_indicator"));
		SetupValue(VarSs::misc::speed_indicator, 0, ("Combat"), ("speed_indicator"));
		SetupValue(VarSs::misc::indiacators_type, 0, ("Combat"), ("indiacators_type"));
		SetupValue(VarSs::visuals::chams_render_mode, 0, ("Combat"), ("chams_render_mode"));
		SetupValue(VarSs::combat::desync_autoshoot_type, 0, ("Combat"), ("Desync Type"));
		SetupValue(VarSs::combat::fov, 100.f, ("Combat"), ("Aimbot Fove"));
		//SetupValue(VarSs::combat::silentvalue, 1.0f, ("Combat"), ("silentvalue"));
		SetupValue(VarSs::combat::silentfovdraw, false, ("Combat"), ("silentfovdraw"));
		SetupValue(VarSs::combat::manipulate_angle_arrow, false, ("Combat"), ("manipulate_angle_arrow"));
		SetupValue(VarSs::combat::manipulate_angle_vectors, false, ("Combat"), ("manipulate_angle_vectors"));
		SetupValue(VarSs::misc::speedhack, false, ("misc"), ("speedhack"));
		SetupValue(VarSs::misc::Atom, false, ("misc"), ("Atom"));
		SetupValue(VarSs::misc::recycler, false, ("misc"), ("recycler"));
		SetupValue(VarSs::misc::AtomOutline, false, ("misc"), ("AtomOutline"));
		SetupValue(VarSs::misc::silentstash, false, ("misc"), ("silentstash"));
		SetupValue(VarSs::misc::speedfloat, 5.f, ("misc"), ("speedfloat"));
		SetupValue(VarSs::misc::gravity, false, ("misc"), ("gravity"));
		SetupValue(VarSs::misc::gravity_modifier, 2.5f, ("misc"), ("gravity_modifier"));
		SetupValue(VarSs::misc::GradientPos, 0.f, ("misc"), ("GradientPos"));
		SetupValue(VarSs::misc::GradientSpread, 0.5f, ("misc"), ("GradientSpread"));
		SetupValue(VarSs::combat::DesynctTime, 0.75f, ("Combat"), ("DesyncTime"));
		// -- Crosshair by faczad
		SetupValue(VarSs::crosshair::sapphire_cross, false, ("crosshair"), ("sapphire_cross"));
		SetupValue(VarSs::crosshair::quad, false, ("crosshair"), ("quad"));
		SetupValue(VarSs::crosshair::gap, 4.5f, ("crosshair"), ("gap"));
		SetupValue(VarSs::crosshair::length, 4.5f, ("crosshair"), ("length"));
		// -- Weapon
		SetupValue(VarSs::weapons::rapidfire, false, ("Weapon"), ("rapidfire"));
		SetupValue(VarSs::weapons::rapid_value, 0.10f, ("Weapon"), ("rapid_value"));
		SetupValue(VarSs::weapons::no_recoil, false, ("Weapon"), ("no_recoil"));
		SetupValue(VarSs::weapons::fastbow, false, ("Weapon"), ("fastbow"));
		SetupValue(VarSs::weapons::size_bullet, false, ("Weapon"), ("size_bullet"));
		SetupValue(VarSs::weapons::bullet_sizex, 100.f, ("Weapon"), ("bullet_sizex"));
		SetupValue(VarSs::weapons::bullet_sizey, 100.f, ("Weapon"), ("bullet_sizey"));
		SetupValue(VarSs::weapons::bullet_sizez, 100.f, ("Weapon"), ("bullet_sizez"));
		SetupValue(VarSs::weapons::recoil_control, 100, ("Weapon"), ("recoil_control2"));
		SetupValue(VarSs::weapons::spread_control, 100, ("Weapon"), ("spread_control"));
		SetupValue(VarSs::weapons::sway_control, 100, ("Weapon"), ("sway_control"));
		SetupValue(VarSs::weapons::fast_bullets, false, ("Weapon"), ("fast_bullets"));
		SetupValue(VarSs::weapons::fast_pvs, false, ("Weapon"), ("fast_pvs"));
		SetupValue(VarSs::weapons::spoof_hitdistance, false, ("Weapon"), ("spoof_hitdistance"));
		SetupValue(VarSs::weapons::hitdistance, 0.f, ("Weapon"), ("hitdistance"));
		SetupValue(VarSs::weapons::pierce_materials, false, ("Weapon"), ("pierce_materials"));
		SetupValue(VarSs::weapons::minicopter_aim, false, ("Weapon"), ("minicopter_aim"));
		SetupValue(VarSs::weapons::remove_attack_anim, false, ("Weapon"), ("remove_attack_anim"));
		SetupValue(VarSs::weapons::no_spread, false, ("Weapon"), ("no_spread"));
		SetupValue(VarSs::weapons::automatic, false, ("Weapon"), ("automatic"));
		SetupValue(VarSs::weapons::thick_bullet, false, ("Weapon"), ("thick_bullet"));
		SetupValue(VarSs::weapons::no_sway, false, ("Weapon"), ("no_sway"));
		SetupValue(VarSs::weapons::wall_ricohcet, false, ("Weapon"), ("wall_ricohcet"));
		SetupValue(VarSs::weapons::eokatap, false, ("Weapon"), ("eokatap"));
		SetupValue(VarSs::weapons::compound, false, ("Weapon"), ("compound"));

		// -- players
		SetupValue(VarSs::players::box_::outline, true, ("Visuals"), ("outline box"));
		SetupValue(VarSs::players::reload_offer, true, ("Visuals"), ("reload_offer"));
		SetupValue(VarSs::visuals::radar_, false, ("Visuals"), ("radar"));
		SetupValue(VarSs::players::box_::colorcheck, true, ("Visuals"), ("colorcheck"));
		SetupValue(VarSs::players::skeleton_::outline, true, ("Visuals"), ("Skeleton Outline"));
		SetupValue(VarSs::players::skeleton_::colorcheck, true, ("Visuals"), ("Skeletom Color Check"));
		SetupValue(VarSs::players::name_::outline, true, ("Visuals"), ("Name Outline"));
		SetupValue(VarSs::players::name_::font, 1, ("Visuals"), ("Name Font"));

		SetupValue(VarSs::players::name_::colorcheck, true, ("Visuals"), ("Name Color check"));
		SetupValue(VarSs::players::distance_::outline, true, ("Visuals"), ("Distance Outline"));
		SetupValue(VarSs::players::distance_::colorcheck, true, ("Visuals"), ("Distance Color Check"));
		SetupValue(VarSs::players::weapon_::outline, true, ("Visuals"), ("Weapon Outline"));
		SetupValue(VarSs::players::weapon_::font, 1, ("Visuals"), ("Weapon Font"));
		SetupValue(VarSs::players::weapon_::colorcheck, true, ("Visuals"), ("Weapon Color Check"));
		SetupValue(VarSs::players::healthbar_::font, 1, ("Visuals"), ("HealthBar Font"));
		SetupValue(VarSs::players::healthbarstyle, 0, ("Visuals"), ("HealthBar Style"));

		SetupValue(VarSs::players::TargetBelt, false, ("Visuals"), ("TargetBelt"));
		SetupValue(VarSs::players::TargetWear, false, ("Visuals"), ("TargetWear"));
		SetupValue(VarSs::players::TargetMain, false, ("Visuals"), ("TargetMain"));
		SetupValue(VarSs::visuals::keybinds, false, ("Weapon"), ("keybinds"));

		//SetupValue(VarSs::stuff::Watermark, false, ("Visuals"), ("Watermark"));
		SetupValue(VarSs::visuals::hand_chams_type, 0, ("Visuals"), ("hand_chams_type"));
		SetupValue(VarSs::visuals::hand_chams_type2, 0, ("Visuals"), ("hand_chams_type2"));
		SetupValue(VarSs::visuals::weapon_chams_type, 0, ("Visuals"), ("weapon_chams_type"));
		SetupValue(VarSs::players::player_chams_type, 0, ("Visuals"), ("player_chams_type"));
		SetupValue(VarSs::players::player_chams_type2, 0, ("Visuals"), ("player_chams_type2"));
		SetupValue(VarSs::players::player_chams, false, ("Visuals"), ("player_chams"));
		SetupValue(VarSs::visuals::hand_chams, false, ("Visuals"), ("hand_chams"));
		SetupValue(VarSs::players::wire_rainbow, false, ("Visuals"), ("pwire_rainbow"));
		SetupValue(VarSs::players::wire_rainbow2, false, ("Visuals"), ("pwire_rainbow2"));
		SetupValue(VarSs::visuals ::wire_u_color, false, ("Visuals"), ("wire_u_color"));
		SetupValue(VarSs::visuals::weapon_chams, false, ("Visuals"), ("weapon_chams"));
		SetupValue(VarSs::stuff::OverFunc, false, ("Visuals"), ("OverFunc"));
		SetupValue(VarSs::misc::local_bones, false, ("Visuals"), ("local_bones"));
		SetupValue(VarSs::misc::anti_aim, false, ("Visuals"), ("anti_aim"));
		SetupValue(VarSs::misc::player_tp_head, false, ("Combat"), ("player_tp_head"));
		SetupValue(VarSs::combat::radius, false, ("Visuals"), ("radius"));
		SetupValue(VarSs::players::target_tracers, false, ("Visuals"), ("target_tracers"));
		SetupValue(VarSs::players::oof_arrows, false, ("Visuals"), ("oof_arrows"));
		SetupValue(VarSs::players::out_type, 0, ("Visuals"), ("out_type"));
		SetupValue(VarSs::players::distance, false, ("Visuals"), ("distance"));
		SetupValue(VarSs::players::box, false, ("Visuals"), ("box"));
		SetupValue(VarSs::combat::autoshoot_power_shoot, false, ("combat"), ("autoshoot_power_shoot"));
		SetupValue(VarSs::players::box_rotate, false, ("Visuals"), ("box_rotate"));
		SetupValue(VarSs::players::fillbox, false, ("Visuals"), ("fillbox"));
		//SetupValue(VarSs::players::fill, 150, ("Visuals"), ("fillAlpha"));
		SetupValue(VarSs::players::Grfillbox, false, ("Visuals"), ("Grfillbox"));
		SetupValue(VarSs::players::rainbowfillbox, false, ("Visuals"), ("RGBfillbox"));
		SetupValue(VarSs::players::rgbfilltransperent, 1.f, ("Visuals"), ("RGBfillbox Alpha"));
		SetupValue(VarSs::players::boxstyle, 0, ("Visuals"), ("boxstyle"));
		SetupValue(VarSs::esp::esp_type, 0, ("Esp"), ("esp_type"));
		SetupValue(VarSs::players::skeleton, false, ("Visuals"), ("skeleton"));
		SetupValue(VarSs::players::name, false, ("Visuals"), ("name"));
		SetupValue(VarSs::players::healthbar, false, ("Visuals"), ("healthbar"));
		SetupValue(VarSs::players::weapon, false, ("Visuals"), ("weapon"));
		SetupValue(VarSs::players::WeaponIcon, false, ("Visuals"), ("WeaponIcon"));
		SetupValue(VarSs::players::DisableWeaponName, false, ("Visuals"), ("DisableWeaponName"));
		SetupValue(VarSs::players::sleeperignore, false, ("Visuals"), ("sleeperignore"));
		SetupValue(VarSs::players::targetline, false, ("Visuals"), ("targetline"));
		// -- Npc
		SetupValue(VarSs::npc::oof_arrows, false, ("Visuals npc"), ("oof_arrows"));
		SetupValue(VarSs::npc::distance, false, ("Visuals npc"), ("distance"));
		SetupValue(VarSs::npc::box, false, ("Visuals npc"), ("box"));
		SetupValue(VarSs::npc::fillbox, false, ("Visuals npc"), ("fillbox"));
		SetupValue(VarSs::npc::Grfillbox, false, ("Visuals npc"), ("GRfillbox"));
		SetupValue(VarSs::npc::rainbowfillbox, false, ("Visuals npc"), ("rainbowfillbox"));
		SetupValue(VarSs::npc::skeleton, false, ("Visuals npc"), ("skeleton"));
		SetupValue(VarSs::npc::name, false, ("Visuals npc"), ("name"));
		SetupValue(VarSs::npc::healthbar, false, ("Visuals npc"), ("healthbar"));
		SetupValue(VarSs::npc::weapon, false, ("Visuals npc"), ("weapon"));
		SetupValue(VarSs::npc::WeaponIcon, false, ("Visuals npc"), ("WeaponIcon"));
		SetupValue(VarSs::npc::DisableWeaponName, false, ("Visuals npc"), ("DisableWeaponName"));
		// -- Other
		SetupValue(VarSs::visuals::xueta, 5.f, ("Other"), ("Radial Radius"));
		SetupValue(VarSs::visuals::skycolor, false, ("Other"), ("skycolor"));
		SetupValue(VarSs::misc::night_stars, false, ("Other"), ("Stars"));
		SetupValue(VarSs::misc::stars, 1000.f, ("Other"), ("Stars Amount"));
		SetupValue(VarSs::visuals::show_distance_bulletTp, false, ("Other"), ("show_distance_bulletTp"));
		SetupValue(VarSs::visuals::No_bobing, false, ("Other"), ("No_bobing"));
		SetupValue(VarSs::visuals::bradley_apc, false, ("Other"), ("bradley_apc"));
		SetupValue(VarSs::visuals::patrol_heli, false, ("Other"), ("patrol_heli"));
		SetupValue(VarSs::visuals::base::tc, false, ("Other"), ("Tc"));
		SetupValue(VarSs::visuals::base::sleeping_bag, false, ("Other"), ("sleeping_bag"));
		SetupValue(VarSs::visuals::base::bed, false, ("Other"), ("bed"));
		SetupValue(VarSs::visuals::base::boxes, false, ("Other"), ("boxes"));
		SetupValue(VarSs::visuals::base::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::base::draw_distance, 300.f, ("Other"), ("draw_distance base"));
		SetupValue(VarSs::visuals::vehicles::minicopter, false, ("Other"), ("minicopter"));
		SetupValue(VarSs::visuals::vehicles::scrapheli, false, ("Other"), ("scrapheli"));
		SetupValue(VarSs::visuals::vehicles::boat, false, ("Other"), ("boat"));
		SetupValue(VarSs::visuals::vehicles::rhib, false, ("Other"), ("rhib"));
		SetupValue(VarSs::visuals::vehicles::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::vehicles::draw_distance, 300.f, ("Other"), ("draw_distance vehicle"));
		SetupValue(VarSs::visuals::turrets::auto_turret, false, ("Other"), ("auto_turret"));
		SetupValue(VarSs::visuals::turrets::flame_turret, false, ("Other"), ("flame_turret"));
		SetupValue(VarSs::visuals::turrets::shotgun_turret, false, ("Other"), ("shotgun_turret"));
		SetupValue(VarSs::visuals::turrets::landmine, false, ("Other"), ("landmine"));
		SetupValue(VarSs::visuals::turrets::sam_site, false, ("Other"), ("sam_site"));
		SetupValue(VarSs::visuals::turrets::bear_trap, false, ("Other"), ("bear_trap"));
		SetupValue(VarSs::visuals::turrets::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::turrets::draw_distance, 300.f, ("Other"), ("draw_distance turret"));
		SetupValue(VarSs::visuals::other::dropped_items, false, ("Other"), ("dropped_items"));
		SetupValue(VarSs::visuals::other::bodybag, false, ("Other"), ("bodybag"));
		SetupValue(VarSs::visuals::other::corpse, false, ("Other"), ("corpse"));
		SetupValue(VarSs::visuals::other::stash, false, ("Other"), ("stash"));
		SetupValue(VarSs::visuals::other::hemp, false, ("Other"), ("hemp"));
		SetupValue(VarSs::visuals::other::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::other::draw_distance, 300.f, ("Other"), ("draw_distance other"));
		SetupValue(VarSs::visuals::crates::elite, false, ("Other"), ("elite"));
		SetupValue(VarSs::visuals::crates::military, false, ("Other"), ("military"));
		SetupValue(VarSs::visuals::crates::supply, false, ("Other"), ("supply"));
		SetupValue(VarSs::visuals::crates::chinook, false, ("Other"), ("chinook"));
		SetupValue(VarSs::visuals::crates::heli, false, ("Other"), ("heli"));
		SetupValue(VarSs::visuals::crates::bradley, false, ("Other"), ("bradley"));
		SetupValue(VarSs::visuals::crates::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::crates::draw_distance, 300.f, ("Other"), ("draw_distance crates"));
		SetupValue(VarSs::visuals::animals::bear, false, ("Other"), ("bear"));
		SetupValue(VarSs::visuals::animals::pig, false, ("Other"), ("pig"));
		SetupValue(VarSs::visuals::animals::chicken, false, ("Other"), ("chicken"));
		SetupValue(VarSs::visuals::animals::wolf, false, ("Other"), ("wolf"));
		SetupValue(VarSs::visuals::animals::deer, false, ("Other"), ("deer"));
		SetupValue(VarSs::visuals::animals::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::animals::draw_distance, 300.f, ("Other"), ("draw_distance animals"));
		//SetupValue(VarSs::visuals::vehicles::show_distance, false, ("Other"), ("show_distance"));
		/*SetupValue(VarSs::visuals::vehicles::draw_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::crates::show_distance, false, ("Other"), ("show_distance"));
		SetupValue(VarSs::visuals::crates::draw_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::ores::show_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::ores::draw_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::animals::show_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::animals::draw_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::turrets::show_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::turrets::draw_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::base::show_distance, 300.f, ("Other"), ("draw_distance animals"));
		SetupValue(VarSs::visuals::base::draw_distance, 300.f, ("Other"), ("draw_distance animals"));*/
		SetupValue(VarSs::visuals::other::show_distance, 300.f, ("Other"), ("other::show_distance"));
		SetupValue(VarSs::visuals::other::draw_distance, 300.f, ("Other"), ("other::draw_distance"));




		// -- MiscTargetInfo
		SetupValue(VarSs::misc::TargetInfo, false, ("Misc"), ("TargetInfo"));
		//VarSs::debug::bullettracer
		SetupValue(VarSs::debug::bulletline, false, ("Misc"), ("Bullet Tracer"));
		SetupValue(VarSs::debug::bulletline2, false, ("Misc"), ("Bullet Tracer2"));
		SetupValue(VarSs::debug::bulletline3, false, ("Misc"), ("Bullet Tracer3"));
		SetupValue(VarSs::debug::bulletline4, false, ("Misc"), ("Bullet Tracer4"));
		SetupValue(VarSs::debug::bulletline5, false, ("Misc"), ("Bullet Tracer5"));
		SetupValue(VarSs::debug::bulletline6, false, ("Misc"), ("Bullet Tracer6"));
		SetupValue(VarSs::players::playerline, false, ("Misc"), ("Movement Line "));
		SetupValue(VarSs::players::playerline2, false, ("Misc"), ("Movement Line2"));
		SetupValue(VarSs::players::playerline3, false, ("Misc"), ("Movement Line3"));
		SetupValue(VarSs::players::playerline4, false, ("Misc"), ("Movement Line4"));

		SetupValue(VarSs::misc::interactive_debug, false, ("Misc"), ("Ineractive Debug"));
		
		SetupValue(VarSs::misc::fakelag, 0, ("Misc"), ("fakelag"));
		SetupValue(VarSs::misc::TeloportTopPlayer, false, ("Misc"), ("TeloportTopPlayer"));

		SetupValue(VarSs::misc::modelstate, 0, ("Misc"), ("modelstate"));

		SetupValue(VarSs::crosshair::cross_type, 0, ("crosshair"), ("cross_type"));
		SetupValue(VarSs::crosshair::cross_type_color, 0, ("crosshair"), ("cross_type_color"));
		SetupValue(VarSs::crosshair::crosshair, false, ("crosshair"), ("crosshair"));
		SetupValue(VarSs::misc::longneck, false, ("Misc"), ("longneck"));
		SetupValue(VarSs::misc::zoom, false, ("Misc"), ("zoom"));
		SetupValue(VarSs::misc::jesus, false, ("Misc"), ("jesus"));
		SetupValue(VarSs::misc::fotsize, 12.f, ("Misc"), ("fotsize"));
		SetupValue(VarSs::misc::fly_auto_stopper, false, ("Misc"), ("fly_auto_stopper"));
		SetupValue(VarSs::misc::weapon_spam, false, ("Misc"), ("weapon_spam"));		
		SetupValue(VarSs::misc::fast_loot, false, ("Misc"), ("fast_loot"));
		SetupValue(VarSs::weapons::hammer_spam, false, ("weapons"), ("hammer_spam"));
		SetupValue(VarSs::weapons::hammer_spam_type, 0, ("weapons"), ("hammer_spam_type"));
		SetupValue(VarSs::misc::AspectRatio, false, ("Misc"), ("AspectRatio"));
		SetupValue(VarSs::misc::flyhack_indicator, false, ("Misc"), ("flyhack_indicator"));
		SetupValue(VarSs::misc::reload_indicator, false, ("Misc"), ("reload_indicator"));
		SetupValue(VarSs::misc::manipulator_indicator, false, ("Misc"), ("manipulator_indicator"));
		SetupValue(VarSs::combat::Player_Circles, false, ("Misc"), ("Player_Circles"));
		SetupValue(VarSs::misc::spiderman, false, ("Misc"), ("spiderman"));
		SetupValue(VarSs::misc::spider_colider, false, ("Misc"), ("spider_colider"));
		SetupValue(VarSs::misc::better_jump, false, ("Misc"), ("better_jump"));
		SetupValue(VarSs::misc::no_fall, false, ("Misc"), ("no_fall"));
		SetupValue(VarSs::misc::fldview, false, ("Misc"), ("fldview"));
		SetupValue(VarSs::misc::hitmaterial, 0, ("Misc"), ("Hit Material"));
		SetupValue(VarSs::misc::HitEffect, false, ("Misc"), ("HitEffect"));
		SetupValue(VarSs::misc::HitSpawnNearest, false, ("Misc"), ("HitSpawnNearest"));
		SetupValue(VarSs::misc::anti_aim_type, 0, ("Misc"), ("anti_aim_type"));
		SetupValue(VarSs::misc::emotion_spamm, false , ("Misc"), ("emotion_spamm"));
		SetupValue(VarSs::misc::gesture, 0, ("Misc"), ("Gesture Spam"));

		SetupValue(VarSs::misc::hit_notification, false, ("Misc"), ("hit_notification"));
		SetupValue(VarSs::misc::auto_pickup, false, ("Misc"), ("Auto Pickp Up"));
		SetupValue(VarSs::misc::auto_farm, false, ("Misc"), ("auto_farm"));
		SetupValue(VarSs::misc::auto_farm_ore, false, ("Misc"), ("auto_farm_ore"));
		SetupValue(VarSs::misc::auto_farm_tree, false, ("Misc"), ("auto_farm_tree"));
		SetupValue(VarSs::misc::can_attack, false, ("Misc"), ("can_attack"));
		SetupValue(VarSs::weapons::weapon_spam, false, ("Misc"), ("weapon_spam"));
		SetupValue(VarSs::misc::faster_healing, false, ("Misc"), ("faster_healing"));
		SetupValue(VarSs::misc::suicide, false, ("Misc"), ("suicide"));
		SetupValue(VarSs::misc::custom_hitsound, false, ("Misc"), ("HitSound"));
		SetupValue(VarSs::misc::custom_hitsound_type, 0, ("Misc"), ("custom_hitsound_type"));
		SetupValue(VarSs::misc::annoyer, false, ("Misc"), ("annoyer"));
		SetupValue(VarSs::misc::rayleigh_changer, false, ("Misc"), ("rayleigh_changer"));
		SetupValue(VarSs::misc::mie_changer, false, ("Misc"), ("mie_changer"));
		SetupValue(VarSs::misc::brightness_changer, false, ("Misc"), ("bright_ambient"));
		SetupValue(VarSs::misc::thunder_changer, false, ("Misc"), ("brightness_changer"));
		SetupValue(VarSs::misc::fog_changer, false, ("Misc"), ("fog_changer"));
		SetupValue(VarSs::misc::rain_changer, false, ("Misc"), ("rain_changer"));
		SetupValue(VarSs::misc::rain, 0.f, ("Misc"), ("rain"));
		SetupValue(VarSs::misc::thunder, 0.f, ("Misc"), ("thunder"));
		SetupValue(VarSs::misc::fog, 0.f, ("Misc"), ("fog"));
		SetupValue(VarSs::misc::brightness, 1.f, ("Misc"), ("brightness"));
		SetupValue(VarSs::misc::rayleigh, 1.f, ("Misc"), ("rayleigh"));
		SetupValue(VarSs::misc::mie, 1.f, ("Misc"), ("mie"));
		SetupValue(VarSs::misc::bright_ambient, false, ("Misc"), ("bright_ambient"));
		SetupValue(VarSs::misc::night_mode, false, ("Misc"), ("night_mode"));
		SetupValue(VarSs::misc::block_commands, false, ("Misc"), ("block_commands"));
		SetupValue(VarSs::misc::sapphire_spammer, false, ("Misc"), ("sapphire_spammer"));
		SetupValue(VarSs::misc::night_intensivity, 5.f, ("Misc"), ("night_intensivity"));
		SetupValue(VarSs::visuals::nightmode, false, ("visuals"), ("nightmode"));
		SetupValue(VarSs::visuals::amb, 5.f, ("visuals"), ("amb"));
		SetupValue(VarSs::misc::Sphera, false, ("Misc"), ("Sphera"));		
		SetupValue(VarSs::misc::circle_hat, false, ("Misc"), ("circle_hat"));
		SetupValue(VarSs::misc::fov, 90.f, ("Misc"), ("Custom Foving Player"));
		SetupValue(VarSs::misc::ratio, 1.4f, ("Misc"), ("ratio"));
		SetupValue(VarSs::misc::omnidirectional_sprinting, false, ("Misc"), ("omnidirectional_sprinting"));
		SetupValue(VarSs::misc::fakeadmin, false, ("Misc"), ("fakeadmin"));
		SetupValue(VarSs::misc::damagemarker, false, ("Misc"), ("Damage Marker"));
		SetupValue(VarSs::misc::hitmarker, false, ("Misc"), ("Hit Marker"));
		SetupValue(VarSs::keys::speedkey, false, ("Visuals"), ("speed_key"));
		SetupValue(VarSs::keys::stasher, false, ("Visuals"), ("stasher"));
		SetupValue(VarSs::keys::weaponspam, false, ("Visuals"), ("weaponspam"));
		SetupValue(VarSs::keys::hammer_spamm, false, ("Visuals"), ("hammer_spam"));
		SetupValue(VarSs::combat::radius2, false, ("Misc"), ("BTbox"));
		SetupValue(VarSs::combat::radius2, false, ("Misc"), ("BTbox"));
		/*SetupValue(VarSs::misc::hitmarker, false, ("Misc"), ("Hit Marker"));
		SetupValue(VarSs::keys::speedkey, false, ("Visuals"), ("speed_key"));*/

		SetupValue(VarSs::misc::custom_time, false, ("Misc"), ("custom_time"));
		SetupValue(VarSs::misc::time, 12.f, ("Misc"), ("time"));
		//VarSs::stuff::font
		SetupValue(VarSs::stuff::font, 0, ("Misc"), ("Global Render Font"));

		SetupValue(VarSs::colors::ambient_color.x, 1.f, ("colors"), ("ambient_color x"));
		SetupValue(VarSs::colors::ambient_color.y, 1.f, ("colors"), ("ambient_color y"));
		SetupValue(VarSs::colors::ambient_color.z, 1.f, ("colors"), ("ambient_color z"));
		SetupValue(VarSs::colors::ambient_color.w, 1.f, ("colors"), ("ambient_color w"));

		SetupValue(VarSs::colors::sky_color.x, 1.f, ("colors"), ("sky_color x"));
		SetupValue(VarSs::colors::sky_color.y, 1.f, ("colors"), ("sky_color y"));
		SetupValue(VarSs::colors::sky_color.z, 1.f, ("colors"), ("sky_color z"));
		SetupValue(VarSs::colors::sky_color.w, 1.f, ("colors"), ("sky_color w"));

		SetupValue(VarSs::colors::one_color.x, 1.f, ("colors"), ("Radial Color x"));
		SetupValue(VarSs::colors::one_color.y, 1.f, ("colors"), ("Radial Color y"));
		SetupValue(VarSs::colors::one_color.z, 1.f, ("colors"), ("Radial Color z"));
		SetupValue(VarSs::colors::one_color.w, 1.f, ("colors"), ("Radial Color w"));

		SetupValue(VarSs::colors::marker.x, 1.f, ("colors"), ("marker x"));
		SetupValue(VarSs::colors::marker.y, 1.f, ("colors"), ("marker y"));
		SetupValue(VarSs::colors::marker.z, 1.f, ("colors"), ("marker z"));
		SetupValue(VarSs::colors::marker.w, 1.f, ("colors"), ("markerr w"));

		SetupValue(VarSs::colors::Boxviscol.x, 255.f, ("colors"), ("Boxviscol x"));
		SetupValue(VarSs::colors::Boxviscol.y, 255.f, ("colors"), ("Boxviscol y"));
		SetupValue(VarSs::colors::Boxviscol.z, 255.f, ("colors"), ("Boxviscol z"));
		SetupValue(VarSs::colors::Boxviscol.w, 1.f, ("colors"), ("Boxviscol w"));

		SetupValue(VarSs::colors::damagemarker.x, 255.f, ("colors"), ("damagemarker Color x"));
		SetupValue(VarSs::colors::damagemarker.y, 255.f, ("colors"), ("damagemarker Color y"));
		SetupValue(VarSs::colors::damagemarker.z, 255.f, ("colors"), ("damagemarker Color z"));
		SetupValue(VarSs::colors::damagemarker.w, 255.f, ("colors"), ("damagemarker Color w"));

		SetupValue(VarSs::colors::hitmarker.x, 255.f, ("colors"), ("hitmarker Color x"));
		SetupValue(VarSs::colors::hitmarker.y, 255.f, ("colors"), ("hitmarker Color y"));
		SetupValue(VarSs::colors::hitmarker.z, 255.f, ("colors"), ("hitmarker Color z"));
		SetupValue(VarSs::colors::hitmarker.w, 255.f, ("colors"), ("hitmarker Color w"));

		SetupValue(VarSs::colors::colsphere.x, 1.f, ("colors"), ("colsphere x"));
		SetupValue(VarSs::colors::colsphere.y, 1.f, ("colors"), ("colsphere y"));
		SetupValue(VarSs::colors::colsphere.z, 1.f, ("colors"), ("colsphere z"));
		SetupValue(VarSs::colors::colsphere.w, 1.f, ("colors"), ("colsphere w"));

		SetupValue(VarSs::colors::vissphere.x, 1.f, ("colors"), ("vissphere x"));
		SetupValue(VarSs::colors::vissphere.y, 1.f, ("colors"), ("vissphere y"));
		SetupValue(VarSs::colors::vissphere.z, 1.f, ("colors"), ("vissphere z"));
		SetupValue(VarSs::colors::vissphere.w, 1.f, ("colors"), ("vissphere w"));

		SetupValue(VarSs::visible::box_color[0], 255.f, ("colors"), ("player_color x"));
		SetupValue(VarSs::visible::box_color[1], 255.f, ("colors"), ("player_color y"));
		SetupValue(VarSs::visible::box_color[2], 255.f, ("colors"), ("player_color z"));
		SetupValue(VarSs::visible::box_color[3], 255.f, ("colors"), ("player_color w"));

		SetupValue(VarSs::visible::GrFillBox_Color1[0], 255.f, ("colors"), ("GrFillBox_Color1 x"));
		SetupValue(VarSs::visible::GrFillBox_Color1[1], 255.f, ("colors"), ("GrFillBox_Color1 y"));
		SetupValue(VarSs::visible::GrFillBox_Color1[2], 255.f, ("colors"), ("GrFillBox_Color1 z"));
		SetupValue(VarSs::visible::GrFillBox_Color1[3], 255.f, ("colors"), ("GrFillBox_Color1 w"));

		SetupValue(VarSs::visible::GrFillBox_Color2[0], 255.f, ("colors"), ("GrFillBox_Color2 x"));
		SetupValue(VarSs::visible::GrFillBox_Color2[1], 255.f, ("colors"), ("GrFillBox_Color2 y"));
		SetupValue(VarSs::visible::GrFillBox_Color2[2], 255.f, ("colors"), ("GrFillBox_Color2 z"));
		SetupValue(VarSs::visible::GrFillBox_Color2[3], 255.f, ("colors"), ("GrFillBox_Color2 w"));

		SetupValue(VarSs::colors::target_tracers_color[0], 255.f, ("colors"), ("target_tracers_color x"));
		SetupValue(VarSs::colors::target_tracers_color[1], 255.f, ("colors"), ("target_tracers_color y"));
		SetupValue(VarSs::colors::target_tracers_color[2], 255.f, ("colors"), ("target_tracers_color z"));
		SetupValue(VarSs::colors::target_tracers_color[3], 255.f, ("colors"), ("target_tracers_color w"));

		SetupValue(VarSs::npc::GrFillBox_Color1[0], 255.f, ("colors"), ("npc1 x"));
		SetupValue(VarSs::npc::GrFillBox_Color1[1], 255.f, ("colors"), ("npc1 y"));
		SetupValue(VarSs::npc::GrFillBox_Color1[2], 255.f, ("colors"), ("npc1 z"));
		SetupValue(VarSs::npc::GrFillBox_Color1[3], 255.f, ("colors"), ("npc1 w"));

		SetupValue(VarSs::npc::GrFillBox_Color2[0], 255.f, ("colors"), ("npc2 x"));
		SetupValue(VarSs::npc::GrFillBox_Color2[1], 255.f, ("colors"), ("npc2 y"));
		SetupValue(VarSs::npc::GrFillBox_Color2[2], 255.f, ("colors"), ("npc2 z"));
		SetupValue(VarSs::npc::GrFillBox_Color2[3], 255.f, ("colors"), ("npc2 w"));

		SetupValue(VarSs::colors::npc_box_color[0], 255.f, ("colors"), ("npc_color x"));
		SetupValue(VarSs::colors::npc_box_color[1], 255.f, ("colors"), ("npc_color y"));
		SetupValue(VarSs::colors::npc_box_color[2], 255.f, ("colors"), ("npc_color z"));
		SetupValue(VarSs::colors::npc_box_color[3], 255.f, ("colors"), ("npc_color w"));

		SetupValue(VarSs::colors::sapphire_color[0], 255.f, ("colors"), ("crosshair x"));
		SetupValue(VarSs::colors::sapphire_color[1], 255.f, ("colors"), ("crosshair y"));
		SetupValue(VarSs::colors::sapphire_color[2], 255.f, ("colors"), ("crosshair z"));
		SetupValue(VarSs::colors::sapphire_color[3], 255.f, ("colors"), ("crosshair w"));

		SetupValue(VarSs::colors::sapphire_color2[0], 255.f, ("colors"), ("crosshair2 x"));
		SetupValue(VarSs::colors::sapphire_color2[1], 255.f, ("colors"), ("crosshair2 y"));
		SetupValue(VarSs::colors::sapphire_color2[2], 255.f, ("colors"), ("crosshair2 z"));
		SetupValue(VarSs::colors::sapphire_color2[3], 255.f, ("colors"), ("crosshair2 w"));

		SetupValue(VarSs::colors::cross_color[0], 255.f, ("colors"), ("cross_color x"));
		SetupValue(VarSs::colors::cross_color[1], 255.f, ("colors"), ("cross_color y"));
		SetupValue(VarSs::colors::cross_color[2], 255.f, ("colors"), ("cross_color z"));
		SetupValue(VarSs::colors::cross_color[3], 255.f, ("colors"), ("cross_color w"));

		SetupValue(VarSs::colors::cross2_color[0], 255.f, ("colors"), ("cross2_color x"));
		SetupValue(VarSs::colors::cross2_color[1], 255.f, ("colors"), ("cross2_color y"));
		SetupValue(VarSs::colors::cross2_color[2], 255.f, ("colors"), ("cross2_color z"));
		SetupValue(VarSs::colors::cross2_color[3], 255.f, ("colors"), ("cross2_color w"));

		SetupValue(VarSs::colors::name_distance_color_player[0], 255.f, ("colors"), ("name_distance_color_player x"));
		SetupValue(VarSs::colors::name_distance_color_player[1], 255.f, ("colors"), ("name_distance_color_player y"));
		SetupValue(VarSs::colors::name_distance_color_player[2], 255.f, ("colors"), ("name_distance_color_player z"));
		SetupValue(VarSs::colors::name_distance_color_player[3], 255.f, ("colors"), ("name_distance_color_player w"));

		SetupValue(VarSs::colors::weapon_color_player[0], 255.f, ("colors"), ("npc_color x"));
		SetupValue(VarSs::colors::weapon_color_player[1], 255.f, ("colors"), ("npc_color y"));
		SetupValue(VarSs::colors::weapon_color_player[2], 255.f, ("colors"), ("npc_color z"));
		SetupValue(VarSs::colors::weapon_color_player[3], 255.f, ("colors"), ("npc_color w"));

		SetupValue(VarSs::colors::FillColor[0], 255.f, ("colors"), ("FillColor x"));
		SetupValue(VarSs::colors::FillColor[1], 255.f, ("colors"), ("FillColor y"));
		SetupValue(VarSs::colors::FillColor[2], 255.f, ("colors"), ("FillColor z"));
		SetupValue(VarSs::colors::FillColor[3], 255.f, ("colors"), ("FillColor w"));

		SetupValue(VarSs::colors::FillColor2[0], 255.f, ("colors"), ("FillColor2x"));
		SetupValue(VarSs::colors::FillColor2[1], 255.f, ("colors"), ("FillColor2 y"));
		SetupValue(VarSs::colors::FillColor2[2], 255.f, ("colors"), ("FillColor2 z"));
		SetupValue(VarSs::colors::FillColor2[3], 255.f, ("colors"), ("FillColor2 w"));

		SetupValue(VarSs::colors::off_color[0], 255.f, ("colors"), ("off_color x"));
		SetupValue(VarSs::colors::off_color[1], 255.f, ("colors"), ("off_color y"));
		SetupValue(VarSs::colors::off_color[2], 255.f, ("colors"), ("off_color z"));
		SetupValue(VarSs::colors::off_color[3], 255.f, ("colors"), ("off_color w"));

		SetupValue(VarSs::colors::PlayerLine.x, 1.f, ("colors"), ("PlayerLine x"));
		SetupValue(VarSs::colors::PlayerLine.y, 1.f, ("colors"), ("PlayerLine y"));
		SetupValue(VarSs::colors::PlayerLine.z, 1.f, ("colors"), ("PlayerLine z"));
		SetupValue(VarSs::colors::PlayerLine.w, 1.f, ("colors"), ("PlayerLine w"));

		SetupValue(VarSs::colors::BulletLine.x, 1.f, ("colors"), ("BulletLine x"));
		SetupValue(VarSs::colors::BulletLine.y, 1.f, ("colors"), ("BulletLine y"));
		SetupValue(VarSs::colors::BulletLine.z, 1.f, ("colors"), ("BulletLine z"));
		SetupValue(VarSs::colors::BulletLine.w, 1.f, ("colors"), ("BulletLine w"));

		SetupValue(VarSs::colors::CrossCol.x, 255.f, ("colors"), ("CrossCol x"));
		SetupValue(VarSs::colors::CrossCol.y, 255.f, ("colors"), ("CrossCol y"));
		SetupValue(VarSs::colors::CrossCol.z, 255.f, ("colors"), ("CrossCol z"));
		SetupValue(VarSs::colors::CrossCol.w, 255.f, ("colors"), ("CrossCol w"));

		SetupValue(VarSs::colors::FovCol.x, 255.f, ("colors"), ("FovCol x"));
		SetupValue(VarSs::colors::FovCol.y, 255.f, ("colors"), ("FovCol y"));
		SetupValue(VarSs::colors::FovCol.z, 255.f, ("colors"), ("FovCol z"));
		SetupValue(VarSs::colors::FovCol.w, 255.f, ("colors"), ("FovCol w"));

		SetupValue(VarSs::colors::FovCol2.x, 255.f, ("colors"), ("FovCol2 x"));
		SetupValue(VarSs::colors::FovCol2.y, 255.f, ("colors"), ("FovCol2 y"));
		SetupValue(VarSs::colors::FovCol2.z, 255.f, ("colors"), ("FovCol2 z"));
		SetupValue(VarSs::colors::FovCol2.w, 255.f, ("colors"), ("FovCol2 w"));

		SetupValue(VarSs::colors::fill_col.x, 255.f, ("colors"), ("fill_col x"));
		SetupValue(VarSs::colors::fill_col.y, 255.f, ("colors"), ("fill_col y"));
		SetupValue(VarSs::colors::fill_col.z, 255.f, ("colors"), ("fill_col z"));
		SetupValue(VarSs::colors::fill_col.w, 255.f, ("colors"), ("fill_col w"));

		SetupValue(VarSs::colors::MarkColor.x, 255.f, ("colors"), ("MarkColor x"));
		SetupValue(VarSs::colors::MarkColor.y, 255.f, ("colors"), ("MarkColor y"));
		SetupValue(VarSs::colors::MarkColor.z, 255.f, ("colors"), ("MarkColor z"));
		//SetupValue(VarSs::colors::MarkColor.w, 255.f, ("colors"), ("MarkCol w"));

		SetupValue(VarSs::colors::MenuColor.x, 255.f, ("colors"), ("MenuColor x"));
		SetupValue(VarSs::colors::MenuColor.y, 0.f, ("colors"), ("MenuColor y"));
		SetupValue(VarSs::colors::MenuColor.z, 255.f, ("colors"), ("MenuColor z"));
		//SetupValue(VarSs::colors::MenuColor.w, 255.f, ("colors"), ("MenuColor w"));

		SetupValue(VarSs::colors::MPRadius.x, 255.f, ("colors"), ("MPRadius x"));
		SetupValue(VarSs::colors::MPRadius.y, 255.f, ("colors"), ("MPRadius y"));
		SetupValue(VarSs::colors::MPRadius.z, 255.f, ("colors"), ("MPRadius z"));
		SetupValue(VarSs::colors::MenuColor.w, 255.f, ("colors"), ("MPRadius w"));

		SetupValue(VarSs::colors::BTspheraCol.x, 1.f, ("colors"), ("BTspheraCol x"));
		SetupValue(VarSs::colors::BTspheraCol.y, 1.f, ("colors"), ("BTspheraCol y"));
		SetupValue(VarSs::colors::BTspheraCol.z, 1.f, ("colors"), ("BTspheraCol z"));
		SetupValue(VarSs::colors::BTspheraCol.w, 1.f, ("colors"), ("BTspheraCol w"));

		SetupValue(VarSs::colors::angle_vector.x, 1.f, ("colors"), ("angle_vector x"));
		SetupValue(VarSs::colors::angle_vector.y, 1.f, ("colors"), ("angle_vector y"));
		SetupValue(VarSs::colors::angle_vector.z, 1.f, ("colors"), ("angle_vector z"));
		//SetupValue(VarSs::colors::angle_vector.w, 1.f, ("colors"), ("angle_vector w"));

		SetupValue(VarSs::colors::angle_arrow.x, 1.f, ("colors"), ("angle_arrow x"));
		SetupValue(VarSs::colors::angle_arrow.y, 1.f, ("colors"), ("angle_arrow y"));
		SetupValue(VarSs::colors::angle_arrow.z, 1.f, ("colors"), ("angle_arrow z"));
		//SetupValue(VarSs::colors::angle_arrow.w, 1.f, ("colors"), ("angle_arrow w"));

		SetupValue(VarSs::colors::BTboxCol.x, 1.f, ("colors"), ("BTboxCol x"));
		SetupValue(VarSs::colors::BTboxCol.y, 1.f, ("colors"), ("BTboxCol y"));
		SetupValue(VarSs::colors::BTboxCol.z, 1.f, ("colors"), ("BTboxCol z"));
		SetupValue(VarSs::colors::BTboxCol.w, 1.f, ("colors"), ("BTboxCol w"));

		SetupValue(VarSs::colors::local_bones.x, 1.f, ("colors"), ("local_bones x"));
		SetupValue(VarSs::colors::local_bones.y, 1.f, ("colors"), ("local_bones y"));
		SetupValue(VarSs::colors::local_bones.z, 1.f, ("colors"), ("local_bones z"));
		SetupValue(VarSs::colors::local_bones.w, 1.f, ("colors"), ("local_bones w"));

		SetupValue(VarSs::colors::hand_chams.x, 1.f, ("colors"), ("hand_chams x"));
		SetupValue(VarSs::colors::hand_chams.y, 1.f, ("colors"), ("hand_chams y"));
		SetupValue(VarSs::colors::hand_chams.z, 1.f, ("colors"), ("hand_chams z"));
		SetupValue(VarSs::colors::hand_chams.w, 1.f, ("colors"), ("hand_chams w"));

		SetupValue(VarSs::colors::player_circles_color.x, 255.f, ("colors"), ("player_circles_color x"));
		SetupValue(VarSs::colors::player_circles_color.y, 255.f, ("colors"), ("player_circles_color y"));
		SetupValue(VarSs::colors::player_circles_color.z, 255.f, ("colors"), ("player_circles_color z"));
		//SetupValue(VarSs::colors::player_circles_color.w, 255.f, ("colors"), ("player_circles_color w"));

		SetupValue(VarSs::colors::hand_chams2.x, 1.f, ("colors"), ("hand_chams2 x"));
		SetupValue(VarSs::colors::hand_chams2.y, 1.f, ("colors"), ("hand_chams2 y"));
		SetupValue(VarSs::colors::hand_chams2.z, 1.f, ("colors"), ("hand_chams2 z"));
		SetupValue(VarSs::colors::hand_chams2.w, 1.f, ("colors"), ("hand_chams2 w"));

		SetupValue(VarSs::colors::ghost.x, 1.f, ("colors"), ("ghost x"));
		SetupValue(VarSs::colors::ghost.y, 1.f, ("colors"), ("ghost y"));
		SetupValue(VarSs::colors::ghost.z, 1.f, ("colors"), ("ghost z"));
		SetupValue(VarSs::colors::ghost.w, 1.f, ("colors"), ("ghost w"));

		SetupValue(VarSs::colors::holo.x, 1.f, ("colors"), ("holo x"));
		SetupValue(VarSs::colors::holo.y, 1.f, ("colors"), ("holo y"));
		SetupValue(VarSs::colors::holo.z, 1.f, ("colors"), ("holo z"));
		SetupValue(VarSs::colors::holo.w, 1.f, ("colors"), ("holo w"));

		SetupValue(VarSs::colors::glow.x, 1.f, ("colors"), ("glow x"));
		SetupValue(VarSs::colors::glow.y, 1.f, ("colors"), ("glow y"));
		SetupValue(VarSs::colors::glow.z, 1.f, ("colors"), ("glow z"));
		SetupValue(VarSs::colors::glow.w, 1.f, ("colors"), ("glow w"));

		SetupValue(VarSs::colors::beh_pl.x, 1.f, ("colors"), ("beh_pl x"));
		SetupValue(VarSs::colors::beh_pl.y, 1.f, ("colors"), ("beh_pl y"));
		SetupValue(VarSs::colors::beh_pl.z, 1.f, ("colors"), ("beh_pl z"));
		SetupValue(VarSs::colors::beh_pl.w, 1.f, ("colors"), ("beh_pl w"));

		SetupValue(VarSs::colors::vis_pl.x, 1.f, ("colors"), ("vis_pl x"));
		SetupValue(VarSs::colors::vis_pl.y, 1.f, ("colors"), ("vis_pl y"));
		SetupValue(VarSs::colors::vis_pl.z, 1.f, ("colors"), ("vis_pl z"));
		SetupValue(VarSs::colors::vis_pl.w, 1.f, ("colors"), ("vis_pl w"));

		SetupValue(VarSs::colors::beh_pl2.x, 1.f, ("colors"), ("beh_pl2 x"));
		SetupValue(VarSs::colors::beh_pl2.y, 1.f, ("colors"), ("beh_pl2 y"));
		SetupValue(VarSs::colors::beh_pl2.z, 1.f, ("colors"), ("beh_pl2 z"));
		SetupValue(VarSs::colors::beh_pl2.w, 1.f, ("colors"), ("beh_pl2 w"));

		SetupValue(VarSs::colors::vis_pl2.x, 1.f, ("colors"), ("vis_pl x"));
		SetupValue(VarSs::colors::vis_pl2.y, 1.f, ("colors"), ("vis_pl y"));
		SetupValue(VarSs::colors::vis_pl2.z, 1.f, ("colors"), ("vis_pl z"));
		SetupValue(VarSs::colors::vis_pl2.w, 1.f, ("colors"), ("vis_pl w"));

		SetupValue(VarSs::colors::wire_pl.x, 1.f, ("colors"), ("wire_pl x"));
		SetupValue(VarSs::colors::wire_pl.y, 1.f, ("colors"), ("wire_pl y"));
		SetupValue(VarSs::colors::wire_pl.z, 1.f, ("colors"), ("wire_pl z"));
		SetupValue(VarSs::colors::wire_pl.w, 1.f, ("colors"), ("wire_pl w"));

		SetupValue(VarSs::colors::glow_pl.x, 1.f, ("colors"), ("glow_pl x"));
		SetupValue(VarSs::colors::glow_pl.y, 1.f, ("colors"), ("glow_pl y"));
		SetupValue(VarSs::colors::glow_pl.z, 1.f, ("colors"), ("glow_pl z"));
		SetupValue(VarSs::colors::glow_pl.w, 1.f, ("colors"), ("glow_pl w"));

		SetupValue(VarSs::colors::beh_fl_pl.x, 1.f, ("colors"), ("beh_fl_pl x"));
		SetupValue(VarSs::colors::beh_fl_pl.y, 1.f, ("colors"), ("beh_fl_pl y"));
		SetupValue(VarSs::colors::beh_fl_pl.z, 1.f, ("colors"), ("beh_fl_pl z"));
		SetupValue(VarSs::colors::beh_fl_pl.w, 1.f, ("colors"), ("beh_fl_pl w"));

		SetupValue(VarSs::colors::vis_fl_pl.x, 1.f, ("colors"), ("vis_fl_pl x"));
		SetupValue(VarSs::colors::vis_fl_pl.y, 1.f, ("colors"), ("vis_fl_pl y"));
		SetupValue(VarSs::colors::vis_fl_pl.z, 1.f, ("colors"), ("vis_fl_pl z"));
		SetupValue(VarSs::colors::vis_fl_pl.w, 1.f, ("colors"), ("vis_fl_pl w"));

		SetupValue(VarSs::colors::DamascusColor1_pl.x, 1.f, ("colors"), ("DamascusColor1_pl x"));
		SetupValue(VarSs::colors::DamascusColor1_pl.y, 1.f, ("colors"), ("DamascusColor1_pl y"));
		SetupValue(VarSs::colors::DamascusColor1_pl.z, 1.f, ("colors"), ("DamascusColor1_pl z"));
		SetupValue(VarSs::colors::DamascusColor1_pl.w, 1.f, ("colors"), ("DamascusColor1_pl w"));

		SetupValue(VarSs::colors::DamascusColor2_pl.x, 1.f, ("colors"), ("DamascusColor2_pl x"));
		SetupValue(VarSs::colors::DamascusColor2_pl.y, 1.f, ("colors"), ("DamascusColor2_pl y"));
		SetupValue(VarSs::colors::DamascusColor2_pl.z, 1.f, ("colors"), ("DamascusColor2_pl z"));
		SetupValue(VarSs::colors::DamascusColor2_pl.w, 1.f, ("colors"), ("DamascusColor2_pl w"));

		SetupValue(VarSs::colors::DamascusGradient1_pl.x, 1.f, ("colors"), ("DamascusGradient1_pl x"));
		SetupValue(VarSs::colors::DamascusGradient1_pl.y, 1.f, ("colors"), ("DamascusGradient1_pl y"));
		SetupValue(VarSs::colors::DamascusGradient1_pl.z, 1.f, ("colors"), ("DamascusGradient1_pl z"));
		SetupValue(VarSs::colors::DamascusGradient1_pl.w, 1.f, ("colors"), ("DamascusGradient1_pl w"));


		SetupValue(VarSs::colors::DamascusPattern1_pl.x, 1.f, ("colors"), ("DamascusPattern1_pl x"));
		SetupValue(VarSs::colors::DamascusPattern1_pl.y, 1.f, ("colors"), ("DamascusPattern1_pl y"));
		SetupValue(VarSs::colors::DamascusPattern1_pl.z, 1.f, ("colors"), ("DamascusPattern1_pl z"));
		SetupValue(VarSs::colors::DamascusPattern1_pl.w, 1.f, ("colors"), ("DamascusPattern1_pl w"));

		SetupValue(VarSs::colors::LightingColor_pl.x, 1.f, ("colors"), ("LightingColor_pl x"));
		SetupValue(VarSs::colors::LightingColor_pl.y, 1.f, ("colors"), ("LightingColor_pl y"));
		SetupValue(VarSs::colors::LightingColor_pl.z, 1.f, ("colors"), ("LightingColor_pl z"));
		SetupValue(VarSs::colors::LightingColor_pl.w, 1.f, ("colors"), ("LightingColor_pl w"));

		SetupValue(VarSs::colors::light_pl.x, 1.f, ("colors"), ("light_pl x"));
		SetupValue(VarSs::colors::light_pl.y, 1.f, ("colors"), ("light_pl y"));
		SetupValue(VarSs::colors::light_pl.z, 1.f, ("colors"), ("light_pl z"));
		SetupValue(VarSs::colors::light_pl.w, 1.f, ("colors"), ("light_pl w"));

		SetupValue(VarSs::colors::celoutline.x, 1.f, ("colors"), ("celoutline x"));
		SetupValue(VarSs::colors::celoutline.y, 1.f, ("colors"), ("celoutline y"));
		SetupValue(VarSs::colors::celoutline.z, 1.f, ("colors"), ("celoutline z"));
		SetupValue(VarSs::colors::celoutline.w, 1.f, ("colors"), ("celoutline w"));

		SetupValue(VarSs::colors::plasma.x, 1.f, ("colors"), ("plasma x"));
		SetupValue(VarSs::colors::plasma.y, 1.f, ("colors"), ("plasma y"));
		SetupValue(VarSs::colors::plasma.z, 1.f, ("colors"), ("plasma z"));
		SetupValue(VarSs::colors::plasma.w, 1.f, ("colors"), ("plasma w"));

		SetupValue(VarSs::colors::invisible.x, 1.f, ("colors"), ("invisible x"));
		SetupValue(VarSs::colors::invisible.y, 1.f, ("colors"), ("invisible y"));
		SetupValue(VarSs::colors::invisible.z, 1.f, ("colors"), ("invisible z"));
		SetupValue(VarSs::colors::invisible.w, 1.f, ("colors"), ("invisible w"));

		SetupValue(VarSs::colors::visible.x, 1.f, ("colors"), ("visible x"));
		SetupValue(VarSs::colors::visible.y, 1.f, ("colors"), ("visible y"));
		SetupValue(VarSs::colors::visible.z, 1.f, ("colors"), ("visible z"));
		SetupValue(VarSs::colors::visible.w, 1.f, ("colors"), ("visible w"));

		SetupValue(VarSs::colors::player_chams.x, 1.f, ("colors"), ("player_chams x"));
		SetupValue(VarSs::colors::player_chams.y, 1.f, ("colors"), ("player_chams y"));
		SetupValue(VarSs::colors::player_chams.z, 1.f, ("colors"), ("player_chams z"));
		SetupValue(VarSs::colors::player_chams.w, 1.f, ("colors"), ("player_chams w"));

		SetupValue(VarSs::colors::player_chams2.x, 1.f, ("colors"), ("player_chams2 x"));
		SetupValue(VarSs::colors::player_chams2.y, 1.f, ("colors"), ("player_chams2 y"));
		SetupValue(VarSs::colors::player_chams2.z, 1.f, ("colors"), ("player_chams2 z"));
		SetupValue(VarSs::colors::player_chams2.w, 1.f, ("colors"), ("player_chams2 w"));

		SetupValue(VarSs::colors::WorldEspColor.x, 255.f, ("colors"), ("WorldEspColor x"));
		SetupValue(VarSs::colors::WorldEspColor.y, 255.f, ("colors"), ("WorldEspColor y"));
		SetupValue(VarSs::colors::WorldEspColor.z, 255.f, ("colors"), ("WorldEspColor z"));
		SetupValue(VarSs::colors::WorldEspColor.w, 255.f, ("colors"), ("WorldEspColor w"));

		SetupValue(VarSs::colors::one_color.x, 1.f, ("colors"), ("one_color x"));
		SetupValue(VarSs::colors::one_color.y, 1.f, ("colors"), ("one_color y"));
		SetupValue(VarSs::colors::one_color.z, 1.f, ("colors"), ("one_color z"));
		SetupValue(VarSs::colors::one_color.w, 1.f, ("colors"), ("one_color w"));

		SetupValue(VarSs::colors::one_color.x, 1.f, ("colors"), ("one_color x"));
		SetupValue(VarSs::colors::one_color.y, 1.f, ("colors"), ("one_color y"));
		SetupValue(VarSs::colors::one_color.z, 1.f, ("colors"), ("one_color z"));
		SetupValue(VarSs::colors::one_color.w, 1.f, ("colors"), ("one_color w"));

		SetupValue(VarSs::colors::DamascusPattern1.x, 255.f, ("colors"), ("DamascusPattern1 x"));
		SetupValue(VarSs::colors::DamascusPattern1.y, 255.f, ("colors"), ("DamascusPattern1 y"));
		SetupValue(VarSs::colors::DamascusPattern1.z, 255.f, ("colors"), ("DamascusPattern1 z"));
		SetupValue(VarSs::colors::DamascusPattern1.w, 255.f, ("colors"), ("DamascusPattern1 w"));

		SetupValue(VarSs::colors::DamascusGradient1.x, 255.f, ("colors"), ("DamascusGradient1 x"));
		SetupValue(VarSs::colors::DamascusGradient1.y, 255.f, ("colors"), ("DamascusGradient1 y"));
		SetupValue(VarSs::colors::DamascusGradient1.z, 255.f, ("colors"), ("DamascusGradient1 z"));
		SetupValue(VarSs::colors::DamascusGradient1.w, 255.f, ("colors"), ("DamascusGradient1 w"));

		SetupValue(VarSs::colors::DamascusColor2.x, 255.f, ("colors"), ("DamascusColor2 x"));
		SetupValue(VarSs::colors::DamascusColor2.y, 255.f, ("colors"), ("DamascusColor2 y"));
		SetupValue(VarSs::colors::DamascusColor2.z, 255.f, ("colors"), ("DamascusColor2 z"));
		SetupValue(VarSs::colors::DamascusColor2.w, 255.f, ("colors"), ("DamascusColor2 w"));

		SetupValue(VarSs::colors::DamascusColor1.x, 255.f, ("colors"), ("DamascusColor1 x"));
		SetupValue(VarSs::colors::DamascusColor1.y, 255.f, ("colors"), ("DamascusColor1 y"));
		SetupValue(VarSs::colors::DamascusColor1.z, 255.f, ("colors"), ("DamascusColor1 z"));
		SetupValue(VarSs::colors::DamascusColor1.w, 255.f, ("colors"), ("DamascusColor1 w"));

		SetupValue(VarSs::colors::LightingColor.x, 1.f, ("colors"), ("LightingColor x"));
		SetupValue(VarSs::colors::LightingColor.y, 1.f, ("colors"), ("LightingColor y"));
		SetupValue(VarSs::colors::LightingColor.z, 1.f, ("colors"), ("LightingColor z"));
		SetupValue(VarSs::colors::LightingColor.w, 1.f, ("colors"), ("LightingColor w"));

		SetupValue(VarSs::colors::two_color.x, 1.f, ("colors"), ("two_color x"));
		SetupValue(VarSs::colors::two_color.y, 1.f, ("colors"), ("two_color y"));
		SetupValue(VarSs::colors::two_color.z, 1.f, ("colors"), ("two_color z"));
		SetupValue(VarSs::colors::two_color.w, 1.f, ("colors"), ("two_color w"));

		SetupValue(belt::pos.x, 200.f, ("pos"), ("belt::pos x"));
		SetupValue(belt::pos.y, 400.f, ("pos"), ("belt::pos y"));

		SetupValue(wear::pos.x, 400.f, ("pos"), ("wear::pos x"));
		SetupValue(wear::pos.y, 400.f, ("pos"), ("wear::pos y"));

		SetupValue(keybind::pos.x, 200.f, ("pos"), ("keybind::pos x"));
		SetupValue(keybind::pos.y, 200.f, ("pos"), ("keybind::pos y"));

		SetupValue(radar::pos.x, 500.f, ("pos"), ("radar::pos x"));
		SetupValue(radar::pos.y, 500.f, ("pos"), ("radar::pos y"));

		SetupValue(color_ui[0], 0.f, ("colors"), ("UI COLOR 0"));
		SetupValue(color_ui[1], 255.f, ("colors"), ("UI COLOR 1"));
		SetupValue(color_ui[2], 0.f, ("colors"), ("UI COLOR 2"));
		//SetupValue(color_ui[3], 255.f, ("colors"), ("UI COLOR 3"));
	}

	std::string szPath = "";

protected:
	std::vector<VarType<int>*> ints;
	std::vector<VarType<bool>*> bools;
	std::vector<VarType<float>*> floats;
};

CConfig config;