int jitter = 1;
int jitter_speed = 10;
int spin_speed = 70;
int spin = 0;

static float VFlyhack = 0.0f;
static float HFlyhack = 0.0f;
static float VMaxFlyhack = 0.0f;
static float HMaxFlyhack = 0.0f;
namespace entities {
	bool TargetVisible = false;
}
static float flyhackDistanceVertical = 0.f;
static float flyhackDistanceHorizontal = 0.f;
static float flyhackPauseTime;
bool just_shot = false;
static float fixed_time_last_shot = 0.0f;
bool did_reload = false;
static float time_since_last_shot = 0.0f;
bool init, show = false;
#define null NULL
bool should_interactive = false;
Vector2 screen_center = { 0, 0 };
namespace variables {
	namespace manipulator {
		bool manipulated = false;
		Vector3 m_manipulate = Vector3::Zero( );
		static float m_last_manipulate_time = 0.0f;
		static float desync = 0.0f;
	}
}

Vector3 fat_target = Vector3::Zero( );
namespace InGame {
	namespace stor {
		Vector3 eye_pos = Vector3(0, 0, 0);
		uintptr_t uBase = NULL;
		uintptr_t gBase = NULL;
		uintptr_t closestPlayer = NULL;

		uintptr_t closestHeli = NULL;
	}
}
bool matrixupdate = false;
namespace VarSs {
	namespace debug {
		bool debug = false;
		int bulletline_type = 0;
		bool bulletline = false;
		bool bulletline2 = false;
		bool bulletline3 = false;
		bool bulletline4 = false;
		bool bulletline5 = false;
		bool bulletline6 = false;
		bool bulletline7 = false;
		bool bulletline8 = false;
		bool espdebug = false;
		bool targetcolor = false;



	}
	namespace keys {
		int stasher = 0;
		bool PressedKeys = false;
		int stashopen = 0;
		int low_velocity = 0;
		int TeleportTopPlayer = 0;

		int debugging = 0;
		int fast_key = 0;
		int ZOZO = 0;
		int zoom = 0;
		static float zoom_value = 15.f;
		int suicide = 0;
		int tp_head_key = 0;
		int manipulated_key = 0;
		int ignore_stopper = 0;
		int speedkey = 0;
		int instakill = 0;
		int psilentkey = 0;
		int weaponspam = 0;
		int hammer_spamm = 0;
		int desync = 0;
		int longnecking = 0;

		int W { 0x57 };
		int S { 0x53 };
		int D { 0x44 };
		int A { 0x41 };
		int space { 0x20 };
	}
	namespace stuff {
		int font = 0;
		int unload_key = 0;
		bool outline_font = true;
		bool rainbow_ui = true;
		bool load_bundles = false;
		bool hide_me = false;
		bool unload_on_key = false;
		bool unhide_me = false;
		bool clear_cache = true;
		bool esp_rotation = true;
		static float size_font = 12.f;
		static float size_font2 = 10.f;

		static float scan;

		Vector3 eyemani = Vector3(0, 0, 0);
		char ConfigName[0x100] = "";
		std::string selected_cfg = "";
		Vector3 best_target = Vector3(0, 0, 0);
		static float anim_speed = 1.f;
		bool gongetflyhack = false;
		static float flyhack = 0.f;
		static float hor_flyhack = 0.f;
		static float testFloat = 0.f;
		static float max_flyhack = 0.f;
		static float max_hor_flyhack = 0.f;
		bool Panic = false;
		bool InjectAnim = false;
		/*bool Watermark = false;*/
		bool OverFunc = false;
		int ScreenHeight = 0;
		int ScreenWidth = 0;
		int TestInt = 0;
		static float ScreenHeight1 = 0;
		static float ScreenWidth1 = 0;
	}

	namespace Dopes {
		float ImGui_Color[] = { 162.f, 32.f, 240.f };
	}

	namespace theme {
		int theme_type = 0;
		bool theme = false;
		bool Pink = false;
		bool Jazz = false;
		bool Green = false;
		bool Asqo = false;
		bool Cream = false;
		bool Lofy = false;
		bool Reach = false;
		bool Ocean = false;
		bool Galaxy = false;
	}

	//namespace radar {
	//	bool ShowRadarNpc = false;
	//	bool Radar_Range = false;
	//	bool ShowRadarSleeper = false;
	//	bool Radar_Size = false;
	//	bool EnableDinamicRadar = false;
	//	bool ShowRadarPlayer = false;
	//	bool ShowRadarBackground = false;
	//	bool Enable = false;

	//}

	namespace chams {
		bool Damascus_Animated = false;
		bool Instansy_Perple = false;
		bool Desolve = false;
		bool Fade_Gold = false;	
		bool FrameRainbow = false;
		bool Galaxy = false;
	}

	namespace combat {
		bool Manipulator_shot_atBt = false;
		bool Player_Circles = false;
		bool low_hiting = false;
		bool hammernapastil = false;
		bool ZOZO = false;
		static float instanthitvalue = 0.f;
		static float DesyncSpawnDelay = 0.15f;
		static float insta_kill_value = 10.f;
		static float silentvalue = 0.f;
		static float silentrange = 0.f;
		static float theta_value = 2.0f;
		static float point_x_value = 1.0f;
		static float point_y_value = 2.5f;
		static float auto_m_dist = 0.1f;
		static float points_rota_generate = 100.f;
		static float points_dync_generate = 10.f;
		bool silentaim = false;
		bool manipualtor_circle = false;
		bool manipulator_stop = false;
		//static float m_stop = { 0.f, 0.f, 0.f, 0.f };
		bool instanthit = false;
		bool silentfovdraw = false;
		bool desync = false;
		bool bullet_tp_arrows = false;
		bool manipulate_angle_vectors = false;
		bool manipulate_angle_arrow = false;
		int  manipulator_vectors_type = 0;
		//bool instakill = false;
		bool ExtentForMoutend = false;
		bool ManipulatorRadios = false;
		bool BulletTeleportGradient = false;
		int hitbox = 0;
		int pvs = 0;
		bool pierce = false;
		bool bullet_tp = false;
		int  bullet_tp_type = 0;
		int  hitscan_tp_type = 0;
		int  hitscan_tp_mode = 0;
		static float bullet_tp_range = 1.f;
		bool bullet_tp_radius = false;
		bool autoreload = false;
		bool instakill = false;
		bool burst_fire = false;
		bool autoshoot = false;
		bool autoshoot_power_shoot = true;
		int desync_autoshoot_type = 0;
		bool always_heli_rotor = false;
		bool HitScan = false;
		bool doubletap = false;
		int doubletaptype = 0;
		bool silent_melee = false;
		bool hit_override = false;
		bool psilent = false;
		bool radius = false;
		bool radius2 = false;
		bool radius3 = false;
		bool psilentheli = false;

		static float fov = 100.f;
		static float DesynctTime = 0.75f;

		bool visualize_fov = true;
		bool manipulator = false;
		bool auto_manipulator = false;
		static float manipulator_scan_points = 2.00f;
		static float manipulator_y = 1.50f;
		static float manipulator_x = 1.50f;
		static float manipulator_safe = 1.00f;
		bool ignore_sleepers = false;
		bool ignore_team = true;
		bool ignore_npc = true;
	}
	namespace crosshair {

		bool crosshair = false;
		bool quad = false;
		int cross_type = 0;
		int cross_type_color = 0;
		bool swastika = false;
		bool sapphire_cross = false;

		static float speed = 2.0f;
		static float razmer = 10.0f;
		static float gap = 4.5f;
		static float length = 4.5f;


	}
	namespace weapons {
		bool spoof_hitdistance = false;
		bool fastbow = false;
		bool remove_attack_anim = false;
		bool weapon_spam = false;
		bool hammer_spam = false;
		int hammer_spam_type = 0;
		bool rapidfire = false;
		static float rapid_value = 0.10;
		bool roundfire = false;
		bool penetrate = false;

		bool size_bullet = false;
		static float bullet_sizex = 8;
		static float bullet_sizey = 8;
		static float bullet_sizez = 8;
		static float bullet_size = 8;
		bool no_recoil = false;
		static float recoil_control = 100.f;
		static float hitdistance = 100.f;
		bool fast_bullets = false;
		bool fast_pvs = false;
		bool wall_shoot = false;
		bool pierce_materials = false;
	    int pierce_material = 0;
		bool minicopter_aim = false;
		bool no_spread = false;
		bool wall_ricohcet = false;
		static float wall_ricohcet_chance = 100.f;
		static float spread_control = 100.f;
		bool automatic = false;
		bool thick_bullet = false;
		bool no_sway = false;
		static float sway_control = 100.f;
		bool rapid_fire = false;
		bool eokatap = false;
		bool compound = false;
	}
	namespace players {
		float uptime = 2.f;
		int healthbarstyle = 0;
		/*int chamstype = 0;
		int weapon_chams_type = 0;
		int hand_chams_type = 0;
		bool chams = false;*/
		bool playerline = false;
		bool radar = false;
		bool reload_offer = false;
		bool TargetMain = false;
		bool wire_rainbow = false;
		bool wire_rainbow2 = false;
		int playerline_type = 0;
		bool player_chams = false;
		int player_chams_type = 0;
		int player_chams_type2 = 0;
		int chams_type = 0;
		int out_type = 0;
		bool playerline2 = false;
		bool playerline3 = false;
		bool playerline4 = false;
		bool fillbox = false;		
		//float fill = { 0.f, 0.f, 0.f, 150.f };
		bool rainbowfillbox = false;
		static float rgbfillboxspeed = 0.3f;
		static float rgbfilltransperent = 1.f;
		bool Grfillbox = false;
		bool box_rotate = false;
		bool GradientBox = false;
		/*bool handchams = false;*/
		int hand_chams_type = 0;
		bool handchams = false;
		bool TargetBelt = false;
		bool TargetWear = false;
		bool DisableWeaponName = false;
		namespace healthbar_ {
			int font = 1;

		}
		namespace box_ {
			bool outline = false;
			bool colorcheck = true;

		}
		namespace skeleton_ {
			int font = 1;
			int skeleton_colored_type = 0;
			bool outline = false;
			bool colorcheck = true;
			ImColor color = ImColor(255, 255, 255);
		}
		namespace name_ {
			int font = 1;

			bool outline = true;
			bool colorcheck = true;
			ImColor color = ImColor(255, 255, 255);
		}
		namespace distance_ {
			int font = 1;

			bool outline = true;
			bool colorcheck = true;
			ImColor color = ImColor(255, 255, 255);
		}
		namespace weapon_ {
			int font = 1;

			bool outline = true;
			bool colorcheck = true;
			ImColor color = ImColor(255, 255, 255);
		}

		bool target_tracers = false;
		bool oof_arrows = false;
		bool sphere = false;
		bool distance = false;
		bool box = false;
		int boxstyle = 0;
		bool skeleton = false;
		bool name = false;
		bool healthbar = false;
		bool healthsend0 = true;
		bool healthsend1 = false;
		bool healthsend2 = false;
		bool healthalive = false;
		bool weapon = false;
		bool WeaponIcon = false;

		bool sleeperignore = false;
		bool targetline = false;
	}
	namespace npc {
		bool DisableWeaponName = false;
		bool distance = false;
		bool oof_arrows = false;
		bool box = false;
		bool fillbox = false;
		bool Grfillbox = false;
		bool rainbowfillbox = false;
		bool skeleton = false;
		bool name = false;
		bool healthbar = false;
		bool weapon = false;
		bool WeaponIcon = false;
		int boxstyle = 0;
		float GrFillBox_Color1[] = { 255.f, 255.f, 255.f };
		float GrFillBox_Color2[] = { 255.f, 255.f, 255.f };
	}
	namespace ores {
		bool closest_ore = false;
		bool show_collectables = false;
		bool stone = false;
		bool sulfur = false;
		bool metal = false;
		bool show_distance = false;
		static float draw_distance = 300.f;
	}
	namespace visuals {
		namespace radar {
			float x = 300.f;
			float y = 300.f;
			float range = 160.f;
			float size = 160.f;
		}
		bool radar_ = false;
		bool wire_u_color = false;
		bool look_eye = false;
		bool npc_look_eye = false;
		bool keybinds = false;
		bool show_radius_bullettp = 2.f;
		bool hand_chams = false;
		bool weapon_chams = false;
		static float xueta = 2.f;
		bool skycolor = false;
		bool nightmode = false;
		static float amb = 5.f;
		bool show_distance_bulletTp = false;
		bool No_bobing = false;
		bool bradley_apc = false;
		bool patrol_heli = false;
		int hand_chams_type = 0;
		int hand_chams_type2 = 0;
		int chams_render_mode = 0;
		int weapon_chams_type = 0;
		namespace base {
			bool tc = false;
			bool sleeping_bag = false;
			bool bed = false;
			bool boxes = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
		namespace vehicles {
			bool minicopter = false;
			bool scrapheli = false;
			bool boat = false;
			bool rhib = false;
			bool show_distance = false;
			static float draw_distance = 300.f;
		}
		namespace turrets {
			bool auto_turret = false;
			bool flame_turret = false;
			bool shotgun_turret = false;
			bool landmine = false;
			bool sam_site = false;
			bool bear_trap = false;
			bool show_distance = false;
			static float draw_distance = 300.f;
		}
		namespace other {
			bool dropped_items = false;
			bool bodybag = false;
			bool corpse = false;
			bool stash = false;
			bool hemp = false;
			bool show_distance = false;
			static float draw_distance = 300.f;
		}
		namespace crates {
			bool elite = false;
			bool military = false;
			bool supply = false;
			bool chinook = false;
			bool heli = false;
			bool bradley = false;
			bool show_distance = false;
			static float draw_distance = 300.f;
		}
		namespace animals {
			bool bear = false;
			bool pig = false;
			bool chicken = false;
			bool wolf = false;
			bool deer = false;
			bool show_distance = false;
			static float draw_distance = 300.f;
		}
	}
	namespace misc {
		bool rayleigh_changer = false;
		bool mie_changer = false;
		bool brightness_changer = false;
		bool rain_changer = false;
		bool rainbow_changer = false;
		bool ocean_changer = false;
		bool thunder_changer = false;
		bool cloud_set_size = false;
		bool cloud_set_color = false;
		bool fog_changer = false;
		bool set_contrast = false;
		bool set_cloud_brightness = false;
		bool cloud_changer = false;
		static float rayleigh = 1.f;
		static float brightness = 1.f;
		static float cloud_brightness = 1.f;
		static float mie = 1.f;
		static float contrast = 1.f;
		static float thunder = 1.f;
		static float rainbow = 1.f;
		static float fog = 1.f;
		static float ocean = 1.f;
		static float rain = 1.f;
		static float cloud = 1.f;
		static float cloud_size = 1.f;
		bool silentstash = false;
		bool speed_indicator = false;
		bool circle_hat = false;
		bool recycler = false;
		int spin_speed = 70;
		int spin = 0;
		static  float Random_Speed = 999.f;
		static  float Fixed_Offset = 999.f;
		bool gravity = false;
		bool night_mode = false;
		static float night_intensivity = 5.f;
		static float gravity_modifier = 2.5f;
		bool hit_notification = false;
		bool fldview = false;
		bool player_tp_head = false;
		bool AAintaver = false;
		bool Anti_aim = false;
		bool AA1 = false;
		bool AA2 = false;
		bool AA3 = false;
		bool AA4 = false;
		bool AA5 = false;
		bool anti_aim = false;
		bool sapphire_spammer = false;
		int anti_aim_type = 0;
		int anti_aim_speed = 10;
		int anti_aim_yaw = false;
		bool SpinBot = false;
		bool SpinBotup = false;
		bool SpinBotdown = false;
		bool TargetInfo = false;
		bool HitLogs = false;
		bool custom_hitsound = false;
		int custom_hitsound_type = 0;
		bool interactive_debug = false;
		int gesture = 0;

		int hitmaterial = 0;
		bool HitEffect = false;
		bool HitSpawnNearest = false;
		bool auto_pickup = false;
		bool auto_farm = false;

		bool TeloportTopPlayer = false;

		bool hitmarker = false;
		bool damagemarker = false;
		int fakelag = 0;

		int modelstate = 0;
		bool crosshair = false;
		bool zoom = false;
		bool jesus = false;
		bool local_bones = false;

		static float fotsize = 12.f;
		bool emotion_spamm = false;
		bool speedhack = false;
		bool longneck = false;
		static float speedfloat = 5.f;
		bool fly_auto_stopper = false;
		bool weapon_spam = false;
		bool fast_loot = false;
		bool AspectRatio = false;

		bool flyhack_indicator = false;
		bool reload_indicator = false;
		bool manipulator_indicator = false;
		int indiacators_type = 0;
		bool insta_indicator = false;
		bool manipulator_indicator2 = false;

		bool spiderman = false;
		bool spider_colider = false;
		bool better_jump = false;
		bool no_fall = false;
		bool auto_farm_ore = false;
		bool auto_farm_tree = false;
		bool can_attack = false;

		bool faster_healing = false;
		bool suicide = false;

		bool annoyer = false;
		bool bright_ambient = false;
		bool night_stars = false;
		bool block_commands = false;
		static float stars = 1000.f;
		bool Sphera = false;
		bool bulletsphere = false;
		bool Atom = false;
		bool AtomOutline = false;
		bool Sphera2 = false;
		bool Capsule = false;
		static float fov = 90.f;
		static float ratio = 1.4f;

		static float GradientPos = 0.f;
		static float GradientSpread = 0.5f;

		bool omnidirectional_sprinting = false;
		bool fakeadmin = false;
		bool custom_time = false;
		static float time = 10.0f;
	}
	namespace colors {
		float FillColor[] = { 0.f, 0.f, 0.f, 180.f };
		float FillColor2[] = { 0.f, 0.f, 0.f, 180.f };		
		float name_distance_color_player[] = { 255.f, 255.f, 255.f };
		float weapon_color_player[] = { 255.f, 255.f, 255.f };
		float team_color[] = { 255.f, 255.f, 255.f };
		float off_color[] = { 255.f, 255.f, 255.f };
		float npc_name_color[] = { 255.f, 255.f, 255.f };
		float npc_healthdist_color[] = { 255.f, 255.f, 255.f };
		float npc_weapons_color[] = { 255.f, 255.f, 255.f };		
		float target_tracers_color[] = { 255.f, 255.f, 255.f };
		float name_color[] = { 255.f, 255.f, 255.f };
		float box_color[] = { 255.f, 255.f, 255.f };
		float weapon_color[] = { 255.f, 255.f, 255.f };

		float sapphire_color[] = { 255.f, 255.f, 255.f, 255.f };
		float sapphire_color2[] = { 255.f, 255.f, 255.f, 255.f };

		float cross_color[] = { 255.f, 255.f, 255.f };
		float cross2_color[] = { 255.f, 255.f, 255.f };

		float npc_skeleton_color[] = { 255.f, 255.f, 255.f };
		float npc_box_color[] = { 255.f, 255.f, 255.f };

		ImVec4 marker = { 255.f, 255.f, 255.f, 255.f };
		ImVec4 ambient_color = { 0.8f, 0.8f, 0.8f, 0.8f };
		ImVec4 sky_color = { 0.8f, 0.8f, 0.8f, 0.8f };
		ImVec4 fill_col = { 255.f, 255.f, 255.f, 200.f };
		ImVec4 one_color = { 1.f, 0.0f, 0.0f, 1.f };
		ImVec4 two_color = { 1.f, 0.0f, 0.0f, 1.f };
		ImVec4 visible = { 1, 1, 1, 1 };
		ImVec4 invisible = { 1, 1, 1, 1 };
		ImVec4 hand_chams = { 1, 1, 1, 1 };
		ImVec4 glow = { 1, 1, 1, 1 };
		ImVec4 beh_pl = { 1, 1, 1, 1 };
		ImVec4 vis_pl = { 1, 1, 1, 1 };
		ImVec4 beh_fl_pl = { 1, 1, 1, 1 };
		ImVec4 vis_fl_pl = { 1, 1, 1, 1 };
		ImVec4 beh_pl2 = { 1, 1, 1, 1 };
		ImVec4 vis_pl2 = { 1, 1, 1, 1 };
		ImVec4 light_pl = { 1, 1, 1, 1 };
		ImVec4 wire_pl = { 1, 1, 1, 1 };
		ImVec4 glow_pl = { 1, 1, 1, 1 };
		ImVec4 LightingColor = { 255, 255, 255, 255 };
		ImVec4 LightingColor_pl = { 255, 255, 255, 255 };
		ImVec4 DamascusColor1 = { 0, 0, 0, 255 };
		ImVec4 DamascusColor2 = { 0, 0, 0, 255 };
		ImVec4 DamascusGradient1 = { 0, 0, 0, 255 };
		ImVec4 DamascusPattern1 = { 0, 255, 255, 255 };
		ImVec4 DamascusColor1_pl = { 0, 0, 0, 255 };
		ImVec4 DamascusColor2_pl = { 0, 0, 0, 255 };
		ImVec4 DamascusGradient1_pl = { 0, 0, 0, 255 };
		ImVec4 DamascusPattern1_pl = { 0, 255, 255, 255 };
		ImVec4 cloud_color = { 0.8f, 0.8f, 0.8f, 0.8f };
		ImVec4 hand_chams2 = { 1, 1, 1, 1 };
		ImVec4 clouds_color = { 1, 1, 1, 1 };
		ImVec4 plasma = { 1, 1, 1, 1 };
		ImVec4 celoutline = { 1, 1, 1, 1 };
		ImVec4 holo = { 1, 1, 1, 1 };
		ImVec4 ghost = { 1, 1, 1, 1 };
		ImVec4 player_chams = { 1, 1, 1, 1 };
		ImVec4 player_chams2 = { 1, 1, 1, 1 };
		ImVec4 player_circles_color = { 255, 255, 255, 255 };
		ImVec4 weapon_chams = { 1, 1, 1, 1 };
		ImVec4 damagemarker = { 1.f, 0.0f, 0.0f, 1.f };
		ImVec4 hitmarker = { 1.f, 0.0f, 0.0f, 1.f };
		ImVec4 BulletLine = { 1, 1, 1, 1 };
		ImVec4 local_bones = { 1, 1, 1, 1 };
		ImVec4 colsphere = { 1, 1, 1, 1 };
		ImVec4 vissphere = { 1, 1, 1, 1 };
		ImVec4 Boxviscol = { 255, 255, 255, 100 };
		ImVec4 CrossCol = { 255, 255, 255, 255 };
		ImVec4 swaston_color = { 255, 255, 255, 255 };
		ImVec4 WorldEspColor = { 255, 255, 255, 255 };
		ImVec4 FovCol = { 255, 255, 255, 255 };
		ImVec4 FovCol2 = { 255, 255, 255, 255 };
		ImVec4 PlayerLine = { 1, 1, 1, 1 };
		ImVec4 MenuColor = { 1, 0, 0, 1 };
		ImVec4 MarkColor = { 255, 255, 255, 255 };			
		ImVec4 BTspheraCol = { 1, 1, 1, 1 };
		ImVec4 BTboxCol = { 1, 1, 1, 1 };
		ImVec4 MPRadius = { 255, 255, 255, 255 };
		ImVec4 angle_vector = { 1, 1, 1, 1 };
		ImVec4 angle_arrow = { 1, 1, 1, 1 };
		ImVec4 tp_head_color = { 1, 1, 1, 1 };

		//ImVec4 two_color = { 1.f, 0.0f, 0.0f, 0.0f };

		//float colsphere[] = { 255, 255, 255, 1 };

		float BulletTp_color[] = { 1.f, 1.f,1.f ,1.f };
	}
	namespace visible {		
		float skeleton_color[] = { 255.f, 255.f, 255.f };
		float box_color[] = { 255.f, 255.f, 255.f };
		float GrFillBox_Color1[] = { 255.f, 255.f, 255.f };
		float GrFillBox_Color2[] = { 255.f, 255.f, 255.f };
		float GradientBox_Color1[] = { 255.f, 255.f, 255.f };
		float GradientBox_Color2[] = { 255.f, 255.f, 255.f };
	}
	namespace invisible {
		float skeleton_color[] = { 255.f, 255.f, 255.f };
		float box_color[] = { 0.f, 255.f, 255.f };
	}
	namespace esp {
		bool esp_type_dd;
		int esp_type = 0;
	}
}