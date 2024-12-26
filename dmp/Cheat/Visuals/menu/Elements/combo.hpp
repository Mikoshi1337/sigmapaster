/////////////////////////////////////////////////////
//                                                   
//    Generated with Retake Design. (ex. ImGui Editor) 
//    Retake Design is designed with love by pers0na2. 
//    All rights to Retake Design belong to pers0na2.  
//                                                   
/////////////////////////////////////////////////////


#ifndef QUARCCOMBO
#define QUARCCOMBO

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_USE using namespace ImGui; 
#include "../imgui/imgui_internal.h"

namespace Retake
{

	static float CalcMaxPopupHeightFromItemCount(int items_count)
	{
		ImGuiContext& g = *GImGui;
		if (items_count <= 0)
			return FLT_MAX;
		return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}

	bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
	bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

	bool begincombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		// Always consume the SetNextWindowSizeConstraint() call in our early return paths
		ImGuiContext& g = *GImGui;
		bool has_window_size_constraint = (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint) != 0;
		g.NextWindowData.Flags &= ~ImGuiNextWindowDataFlags_HasSizeConstraint;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ImGui::GetFrameHeight();
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const float expected_w = ImGui::GetWindowWidth() - style.WindowPadding.x - 10;
		const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : expected_w;
		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 3.0f));
		const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held);
		bool popup_open = ImGui::IsPopupOpen(id, ImGuiPopupFlags_None);


		const ImU32 frame_col = ImGui::GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		const float value_x2 = ImMax(frame_bb.Min.x, frame_bb.Max.x - arrow_size);
		ImGui::RenderNavHighlight(frame_bb, id);
		window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImColor(33, 35, 41), 2);
		ImGui::RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), ImColor(255, 255, 255), ImGuiDir_Down, 1.0f);
		if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
			ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding + ImVec2(0,2), ImVec2(value_x2, frame_bb.Max.y), preview_value, NULL, NULL, ImVec2(0.0f, 0.0f));
		if (label_size.x > 0)
			ImGui::RenderText(ImVec2(frame_bb.Min.x, frame_bb.Min.y - 17), label);

		if ((pressed || g.NavActivateId == id) && !popup_open)
		{
			if (window->DC.NavLayerCurrent == 0)
				window->NavLastIds[0] = id;
			ImGui::OpenPopupEx(id);
			popup_open = true;
		}

		if (!popup_open)
			return false;

		if (has_window_size_constraint)
		{
			g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasSizeConstraint;
			g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
		}
		else
		{
			if ((flags & ImGuiComboFlags_HeightMask_) == 0)
				flags |= ImGuiComboFlags_HeightRegular;
			IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_));    // Only one
			int popup_max_height_in_items = -1;
			if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
			else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
			else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
			ImGui::SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
		}

		char name[16];
		ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

		// Peak into expected window size so we can position it
		if (ImGuiWindow* popup_window = ImGui::FindWindowByName(name))
			if (popup_window->WasActive)
			{
				// Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
				ImVec2 size_expected = ImGui::CalcWindowNextAutoFitSize(popup_window);
				popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
				ImRect r_outer = ImGui::GetPopupAllowedExtentRect(popup_window);
				ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
				ImGui::SetNextWindowPos(pos);
			}
		// We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

		// Horizontally align ourselves with the framed text
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, style.WindowPadding.y));
		bool ret = ImGui::Begin(name, NULL, window_flags);
		ImGui::PopStyleVar();
		if (!ret)
		{
			ImGui::EndPopup();
			IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
			return false;
		}
		return true;
	}

	static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
	{
		const char* const* items = (const char* const*)data;
		if (out_text)
			*out_text = items[idx];
		return true;
	}

	static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
	{
		// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
		const char* items_separated_by_zeros = (const char*)data;
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p)
		{
			if (idx == items_count)
				break;
			p += strlen(p) + 1;
			items_count++;
		}
		if (!*p)
			return false;
		if (out_text)
			*out_text = p;
		return true;
	}

	bool combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
	{
		IM_USE;
		ImGuiContext& g = *GImGui;
		const char* preview_value = NULL;
		if (*current_item >= 0 && *current_item < items_count)
			items_getter(data, *current_item, &preview_value);

		if (popup_max_height_in_items != -1 && !g.NextWindowData.SizeCond)//SizeConstraintCond
			SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

		if (!begincombo(label, preview_value, ImGuiComboFlags_None))
			return false;
		bool value_changed = false;

		for (int i = 0; i < items_count; i++)
		{
			PushID((void*)(intptr_t)i);
			const bool item_selected = (i == *current_item);
			const char* item_text;
			if (!items_getter(data, i, &item_text))
				item_text = "Unknown item";
			if (Selectable(item_text, item_selected))
			{
				value_changed = true;
				*current_item = i;
			}
			if (item_selected)
				SetItemDefaultFocus();
			PopID();
		}

		EndCombo();
		return value_changed;
	}

	bool combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
	{
		const bool value_changed = combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
		return value_changed;
	}

	bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
	{
		int items_count = 0;
		const char* p = items_separated_by_zeros;

		while (*p)
		{
			p += strlen(p) + 1;
			items_count++;
		}

		bool value_changed = combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
		return value_changed;
	}

}

#endif