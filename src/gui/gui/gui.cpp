#include "gui.hpp"

namespace fs = std::filesystem;

void downloadWindow(int sizeX, int sizeY, audioParams &audio,  ma_device &device, ma_decoder &decoder) {

    std::string windowName = "Download - " + std::string(videoUrl) + "###downloadWindow";

    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(sizeX/2.5, sizeY), ImGuiCond_Always);

    if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

        if (ImGui::InputText("Url", videoUrl, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
            audio.audiofile.clear();
            unInitAudio(decoder, device);
            audio = downloadAudio(videoUrl);
            if (convertAudio(&audio) && !audio.audiofile.empty()) {
                if (initAudio(audio, decoder)) {
                    if (!setUpAudioDevice(decoder, device)) {
                        std::cerr << "Failed to set up audio device";
                    }
                }
                else {
                    unInitAudio(decoder, device);
                    std::cerr << "Failed to initialize audio device";
                }
            }
            else {
                ImGui::Text("Failed to download or convert audio.");
            }
        }  

        if (ImGui::SmallButton("Download")) {
            audio.audiofile.clear();
            unInitAudio(decoder, device);
            audio = downloadAudio(videoUrl);
            if (convertAudio(&audio) && !audio.audiofile.empty()) {
                if (initAudio(audio, decoder)) {
                    if (!setUpAudioDevice(decoder, device)) {
                        std::cerr << "Failed to set up audio device";
                    }
                }
                else {
                    unInitAudio(decoder, device);
                    std::cerr << "Failed to initialize audio device";
                }
            }
            else {
                ImGui::Text("Failed to download or convert audio.");
            }
        }

        if (audio.loaded) {
            int duration = std::stoi(audio.audioDuration);
            int durationM = ((int)duration % 3600) / 60;
            int durationS = (int)duration % 60;
            ImGui::Text(std::string("Author: " + audio.audioAuthor).c_str());
            ImGui::Text(std::string("Name: " + audio.audiofileName).c_str());
            ImGui::Text("Duration: %d:%02d", durationM, durationS);
        }
        ImGui::End();
    }
}

void audioWindow(int sizeX, int sizeY, audioParams &audio, ma_device &device) {
    std::string windowName = "Audio - " + audio.audiofileName + "###audioWindow";

    ImGui::SetNextWindowPos(ImVec2(sizeX/2.5,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(sizeX - (sizeX/2.5), sizeY), ImGuiCond_Always);

    if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        if (audio.audiofile.size() == 0) {
            const char* text = "No file selected.";
            ImVec2 textSize = ImGui::CalcTextSize(text);

            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
            ImGui::SetCursorPosY((windowSize.y - textSize.y) * 0.5f);

            ImGui::Text("%s", text);
        }
        else {
            playAudio(device);
        }

        ImGui::End();
    }
}