#include "imgui.h"

namespace c {

	inline ImVec4 accent = ImColor(81, 105, 198, 255);

	namespace bg
	{
		inline ImVec4 background = ImColor(9, 9, 11, 230);
		inline ImVec4 background_watermark = ImColor(7, 7, 9, 230);
		inline ImVec2 size = ImVec2(750, 610);
		inline float rounding = 5.f;
	}

	namespace child
	{

		inline ImVec4 background = ImColor(14, 14, 16, 200);
		inline ImVec4 outline = ImColor(18, 20, 18, 255);
		inline float rounding = 5.f;

	}

	namespace checkbox
	{
		inline ImVec4 background = ImColor(24, 24, 26, 255);
		inline ImVec4 circle_inactive = ImColor(34, 34, 36, 255);
		inline float rounding = 10.f;
	}

	namespace slider
	{
		inline ImVec4 background = ImColor(24, 24, 26, 255);
		inline ImVec4 circle_inactive = ImColor(34, 34, 36, 255);
		inline float rounding = 10.f;
	}

	namespace tabs
	{
		inline ImVec4 background = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline float rounding = 5.f;
	}

	namespace combo
	{
		inline ImVec4 background = ImColor(16, 16, 19, 255);
		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 2.f;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(16, 17, 19, 255);
		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 5.f;
	}

	namespace button
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 5.f;
	}

	namespace keybind
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 5.f;
	}

	namespace input
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 5.f;
	}

	namespace scroll
	{
		inline float rounding = 30.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(106, 108, 115, 255);
		inline ImVec4 text = ImColor(66, 68, 75, 255);

		inline float glow_size = 8.f;
	}
	namespace text_info_Bar
	{
		inline ImVec4 text_infobar = ImColor(200, 200, 200, 255);

	}
}