ImVec2 ImVecAdd(ImVec2 vec1, ImVec2 vec2)
{
	return { vec1.x + vec2.x, vec1.y + vec2.y };
}

ImVec2 ImVecSubtract(ImVec2 vec1, ImVec2 vec2)
{
	return { vec1.x - vec2.x, vec1.y - vec2.y };
}


static const char* hitbox_type[]{
"Head",
"Neck",
"Stomach",
"Head/Stomach",
};
static const char* Language_type[]{
"Russian",
"English",
};

static const char* anti_aim_yaw[]{
	"Up",
	"Down",
	"Ophthalmic",
};

static const char* healthbar_color_style[]{
"Defualt",
"Gradient",
};
static const char* healthbar_color[]{
"Defualt",
"Custom",
};
static const char* npc_healthbar_color_style[]{
"Defualt",
"Gradient",
};
static const char* chams_type[]{
"Glow",
"Wire Frame",
"Set Trought",
"Lit Chams",
"Standart",
"Hologram",
"Rainbow",
"Plasma",
"Force Field",
"distortionrim",
"baiscrim",
"blur",
"dissolveemission",
"ocean",
};
static const char* npc_healthbar_color[]{
"Defualt",
"Custom",
};

static const char* building_grade[]{
	"Twig",
	"Wood",
	"Stone",
	"Metal",
	"HQM",
};
static const char* npc_box_style[]{
	"None",
	"Default",
	"Cornered",
};
static const char* box_style[]{
	"None",
	"Default",
	"Cornered",
	"3D Box",
};

static const char* target_tracer_pos[]{
	"Default",
	"Middle",
};
static const char* tracer_pos[]{
	"Default",
	"Middle",
};
static const char* modelstate_pos[]{
	"Default",
	"On Ladder",
	"On Ground",
	"Ducked",
	"Sleeping",
};
static const char* autoshoot_type[]{
	"Silent",
	"Default",
};

static const char* desync_autoshoot_type[]{
	"Always",
	"Desync",
};
static const char* timeshoot_type[]{
	"Silent",
	"Default",
};

static const char* healthbar_style[]{
	"None",
	"Left Side",
	"Bottom",
};

static const char* npc_healthbar_style[]{
	"None",
	"Left Side",
	"Bottom",
};

static const char* fake_lag_type[]{
	"Basic",
	"Double Tap",
};

static const char* Out_style[]{
"Triangle Filled",
"Triangle NoFilled",
"Circle",
"Circle Small",
};
static const char* ignore[]{
	"Ignore NPC",
	"Ignore Teammate",
	"Ignore Sleeping",
};
static const char* psilent_bone[]{
"Head",
"Body",
"Random (All)",
"Random (Main)",
};
static const char* emotions[]{
				("None"),
				("Clap"),
				("Friendly"),
				("Thumbsdown"),
				("Thumbsup"),
				("Ok"),
				("Point"),
				("Shrug"),
				("Victory"),
				("Wave"),
};

static const char* fov_style[]{
"Square",
"circular",
};
