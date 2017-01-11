#pragma once
#include "imgui.h"

namespace Toolkit
{
	void SetupImGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = true;
		style.CurveTessellationTol = 1.25f;
		style.Alpha = 1.f;
		//style.WindowFillAlphaDefault = .7f;

		style.WindowPadding = ImVec2(8, 8);
		style.WindowRounding = 4;
		style.ChildWindowRounding = 0;
		style.FramePadding = ImVec2(3, 3);
		style.FrameRounding = 0;
		style.ItemSpacing = ImVec2(8, 4);
		style.ItemInnerSpacing = ImVec2(5, 5);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 22;
		style.ScrollbarSize = 16;
		style.ScrollbarRounding = 8;
		style.GrabMinSize = 8;
		style.GrabRounding = 0;

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.98f, 0.98f, 0.98f, 0.50f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.03f, 0.03f, 0.03f, 0.30f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.28f, 0.28f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.31f, 0.31f, 0.31f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.19f, 0.19f, 0.19f, 0.20f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.87f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 0.52f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.60f, 0.60f, 0.34f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.84f, 0.84f, 0.34f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.90f, 0.90f, 0.90f, 0.60f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.36f, 0.59f, 0.61f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

}