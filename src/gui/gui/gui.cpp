#include "gui.hpp"

namespace fs = std::filesystem;

void downloadWindow(int sizeX, int sizeY, audioParams &audio,  ma_device &device, ma_decoder &decoder) {

    std::string windowName = "Download - " + std::string(videoUrl) + "###downloadWindow";

    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(sizeX/2.5, sizeY), ImGuiCond_Always);

    if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove) || ImGui::SmallButton("Download")) {

        if (ImGui::InputText("Url", videoUrl, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
            if (!downloadFromUrl(videoUrl, audio, decoder, device)) {
                ImGui::Text("Failed to download or convert audio.");
            }
        }  

        if (ImGui::SmallButton("Download")) {
            downloadFromUrl(videoUrl, audio, decoder, device);
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
            if (ImGui::SliderFloat("Volume", &audio.volume, 0.0, 1.0)) {
                 setAudioVolume(device, audio.volume);
            }
            if (ImGui::SmallButton(audio.isPlaying ? "Stop" : "Play")) {
                audio.isPlaying = !audio.isPlaying;

                if (audio.isPlaying) {
                    ma_device_start(&device);
                } else {
                    ma_device_stop(&device);
                }
            }

            ImGui::NewLine();

            if (ImGui::SmallButton("Save")) {
                std::string savePath = getSavePath();
                std::string bDot, aDot;

                std::stringstream ss(savePath);

                std::getline(ss, bDot, '.');
                std::getline(ss, aDot);

                if (aDot == "wav") {
                    saveFileWav(savePath, audio);
                }
                else if (aDot == "mp3") {
                    saveFileMP3(savePath, audio);
                }
            }
        }   
        ImGui::End();
    }
}


bool downloadFromUrl(const char* url, audioParams &audio, ma_decoder &decoder, ma_device &device) {
    audio.audiofile.clear();
    unInitAudio(decoder, device);
    audio = downloadAudio(url);
    if (convertAudio(audio) && !audio.audiofile.empty()) {
        if (initAudio(audio, decoder)) {
            if (!setUpAudioDevice(decoder, device, audio)) {
                std::cerr << "Failed to set up audio device";
                return false;
            }
            return true;
        }
        else {
            unInitAudio(decoder, device);
            std::cerr << "Failed to initialize audio device";
        }
    }
    return false;
}