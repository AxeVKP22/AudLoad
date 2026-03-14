#include "gui.hpp"

char videoUrl[256];
std::string audiofileName = "-";

std::vector<char*> audiofile;

void downloadWindow(int sizeX, int sizeY) {

    std::string windowName = "Download - " + std::string(videoUrl);

    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(sizeX/2.5, sizeY), ImGuiCond_Always);

    if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        //ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove

        if (ImGui::InputText("Url", videoUrl, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {

        }

        ImGui::End();
    }
}

void audioWindow(int sizeX, int sizeY) {
    std::string windowName = "Audio - " + audiofileName;

    ImGui::SetNextWindowPos(ImVec2(sizeX/2.5,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(sizeX - (sizeX/2.5), sizeY), ImGuiCond_Always);

    if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        if (audiofile.size() == 0) {
            const char* text = "No file selected.";
            ImVec2 textSize = ImGui::CalcTextSize(text);

            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
            ImGui::SetCursorPosY((windowSize.y - textSize.y) * 0.5f);

            ImGui::Text("%s", text);
        }

        ImGui::End();
    }
}