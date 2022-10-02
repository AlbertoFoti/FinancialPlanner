#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "imgui.h"
#include "../Core/Backend/Backend.hpp"

struct Date_format {
    int Day;
    int Month;
    int Year;
};
using Date_format_p = std::shared_ptr<Date_format>;

/**
 * @brief Calendar Selection Widget
 * 
 * @return Date_format_p 
 */
Date_format_p calendarSelection();

/**
 * @brief ListBox Selection Widget
 *
 * @return Item selection inside list
 */
template<typename T>
int category_list_box(const std::string& list_name, std::vector<T> list, int sel) {
    static char c_str[50];
    sprintf(c_str, "%s", list_name.c_str());
    ImGui::BulletText("%s", c_str);

    char identifier[30];
    std::string id = "##" + list_name + "list_box_id";
    sprintf(identifier, "%s", id.c_str());
    if (ImGui::BeginListBox(identifier, ImVec2(0, ImGui::GetWindowHeight()*0.1)))
    {
        for (int n = 0; n < list.size(); n++)
        {
            const bool is_selected = (sel == n);

            if (ImGui::Selectable(list[n]->name.c_str(), is_selected))
                sel = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    return sel;
}

/**
	 * @brief Utility : shows Helper Marker
	 *
	 * @param desc
	 */
void HelpMarker(const char* desc);

/**
	 * @brief Utility : Big Separator
	 *
	 * @param desc
	 */
void ImGui_big_separator();