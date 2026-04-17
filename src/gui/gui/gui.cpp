#include "gui.hpp"

namespace fs = std::filesystem;

settingsParams settings;
std::string workingDir;

bool settingsWindowV = false;

const char* colors[] = {"Light", "Blue", "Red", "Green", "Yellow", "Orange"};

void downloadWindow(int sizeX, int sizeY, audioParams &audio,  ma_device &device, ma_decoder &decoder) {

    std::string windowName = "Download - " + std::string(videoUrl) + "###downloadWindow";

    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(sizeX/2.5, sizeY), ImGuiCond_Always);

    if (ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove) || ImGui::SmallButton("Download")) {

        if (ImGui::InputText("Url", videoUrl, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
            std::thread t(downloadFromUrl, videoUrl, std::ref(audio), std::ref(decoder), std::ref(device));
            t.detach();
        }  

        if (ImGui::SmallButton("Download")) {
            std::thread t(downloadFromUrl, videoUrl, std::ref(audio), std::ref(decoder), std::ref(device));
            t.detach();
        }

        if (audio.loaded) {
            int duration = std::stoi(audio.audioDuration);
            int durationM = ((int)duration % 3600) / 60;
            int durationS = (int)duration % 60;
            ImGui::Text(std::string("Author: " + audio.audioAuthor).c_str());
            ImGui::Text(std::string("Name: " + audio.audiofileName).c_str());
            ImGui::Text("Duration: %d:%02d", durationM, durationS);
        }
        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Comma)) {
            settingsWindowV = !settingsWindowV;
        }

        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S)) {
            std::string savePath = getSavePath(fs::current_path().string());
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
            if (ImGui::SmallButton(audio.isPlaying ? "Stop" : "Play") || ImGui::IsKeyPressed(ImGuiKey_Space)) {
                audio.isPlaying = !audio.isPlaying;

                if (audio.isPlaying) {
                    ma_device_start(&device);
                } else {
                    ma_device_stop(&device);
                }
            }

            ImGui::NewLine();

            if (ImGui::SmallButton("Save") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S)) {
                std::string savePath = getSavePath(fs::current_path().string());
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
        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Comma)) {
            settingsWindowV = !settingsWindowV;
        }
        ImGui::End();
    }
}

void welcomeWindow() {
    if (ImGui::Begin("Welcome to AudLoad")) {
        ImGui::Text("Controls:");
        ImGui::NewLine();
        if (ImGui::BeginTable("Controls", 2)) {
            ImGui::TableSetupColumn("Key");
            ImGui::TableSetupColumn("Action");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Ctrl + ,");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Open settings");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Ctrl + s");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Save");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Space");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Play/Pause");
            ImGui::EndTable();
        }
        ImGui::Text("Repo: https://github.com/AxeVKP22/AudLoad");
        ImGui::Checkbox("Show on startup", &settings.welcomeWindow);

        if (ImGui::SmallButton("Close")) {
            std::string savePath = workingDir + "settings.json";
            std::cout << "Saving to " << workingDir;
            saveSettings(savePath, settings);
            settings.showWelcomeWindow = !settings.showWelcomeWindow;
        }
        ImGui::SameLine();
        ImGui::Text("Or press X");
        if (ImGui::IsKeyPressed(ImGuiKey_X)) {
            std::string savePath = workingDir + "settings.json";
            std::cout << "Saving to " << workingDir;
            saveSettings(savePath, settings);
            settings.showWelcomeWindow = !settings.showWelcomeWindow;
        }

        ImGui::End();
    }
}

void settingsWindow() {

    workingDir = fs::current_path().string() + "\\";

    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
    if (ImGui::Begin("Settings")) {

        if (ImGui::SmallButton("Save")) {
            std::string savePath = workingDir + "settings.json";
            std::cout << "Saving to " << workingDir;
            saveSettings(savePath, settings);
        }
        
        ImGui::SameLine();

        if (ImGui::SmallButton("Close")) {
            settingsWindowV = !settingsWindowV;
        }

        if (ImGui::BeginCombo("Theme", colors[settings.theme])) {
            for (int i = 0; i < IM_ARRAYSIZE(colors); i++) {
                bool isSelected = (settings.theme == i);
                if (ImGui::Selectable(colors[i], isSelected)) {
                    settings.theme = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        std::string tWorkingDir = "App directory: " + workingDir;
        ImGui::Text(tWorkingDir.c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Open")) {
            openAndSelect(workingDir);
        }

        ImGui::Checkbox("Show welcome window", &settings.welcomeWindow);

        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Comma)) {
            settingsWindowV = !settingsWindowV;
        }
        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S)) {
            std::string savePath = workingDir + "settings.json";
            std::cout << "Saving to " << workingDir;
            saveSettings(savePath, settings);
        }
    }
    ImGui::End();
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

void openAndSelect(const std::string& filePath) {
    std::string arg = "/select,\"" + filePath + "\"";
    ShellExecuteA(NULL, "open", "explorer.exe", arg.c_str(), NULL, SW_SHOWDEFAULT);
}

void setColorsRed() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImVec4 accent      = ImVec4(0.757f, 0.145f, 0.078f, 1.00f);
    ImVec4 accentA40   = ImVec4(0.757f, 0.145f, 0.078f, 0.40f);
    ImVec4 accentA31   = ImVec4(0.757f, 0.145f, 0.078f, 0.31f);
    ImVec4 accentA20   = ImVec4(0.757f, 0.145f, 0.078f, 0.20f);
    ImVec4 accentA80   = ImVec4(0.757f, 0.145f, 0.078f, 0.80f);
    ImVec4 accentA90   = ImVec4(0.757f, 0.145f, 0.078f, 0.90f);

    colors[ImGuiCol_Text]            = ImVec4(1,1,1,1);
    colors[ImGuiCol_TextDisabled]    = ImVec4(0.5f,0.5f,0.5f,1);

    colors[ImGuiCol_WindowBg]        = ImVec4(0.06f,0.06f,0.06f,0.94f);
    colors[ImGuiCol_PopupBg]         = ImVec4(0.08f,0.08f,0.08f,0.94f);

    colors[ImGuiCol_Border]          = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_FrameBg]         = accentA40;
    colors[ImGuiCol_FrameBgHovered]  = accentA80;
    colors[ImGuiCol_FrameBgActive]   = accent;

    colors[ImGuiCol_TitleBg]         = ImVec4(0.04f,0.04f,0.04f,1);
    colors[ImGuiCol_TitleBgActive]   = accentA40;

    colors[ImGuiCol_ScrollbarGrab]   = accentA40;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentA80;
    colors[ImGuiCol_ScrollbarGrabActive]  = accent;

    colors[ImGuiCol_CheckMark]       = accent;

    colors[ImGuiCol_SliderGrab]      = accentA40;
    colors[ImGuiCol_SliderGrabActive]= accent;

    colors[ImGuiCol_Button]          = accentA40;
    colors[ImGuiCol_ButtonHovered]   = accentA80;
    colors[ImGuiCol_ButtonActive]    = accent;

    colors[ImGuiCol_Header]          = accentA31;
    colors[ImGuiCol_HeaderHovered]   = accentA80;
    colors[ImGuiCol_HeaderActive]    = accent;

    colors[ImGuiCol_Separator]       = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_ResizeGrip]      = accentA20;
    colors[ImGuiCol_ResizeGripHovered]= accentA80;
    colors[ImGuiCol_ResizeGripActive]= accent;

    colors[ImGuiCol_Tab]             = accentA31;
    colors[ImGuiCol_TabHovered]      = accentA80;
    colors[ImGuiCol_TabSelected]     = accentA40;

    colors[ImGuiCol_TextSelectedBg]  = ImVec4(0.98f,0.59f,0.26f,0.35f);

    colors[ImGuiCol_DragDropTarget]  = ImVec4(1,1,0,0.9f);
}

void setColorsGreen() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImVec4 accent      = ImVec4(0.2196, 0.7882, 0.6902, 1.00f);
    ImVec4 accentA40   = ImVec4(0.2196, 0.7882, 0.6902, 0.40f);
    ImVec4 accentA31   = ImVec4(0.2196, 0.7882, 0.6902, 0.31f);
    ImVec4 accentA20   = ImVec4(0.2196, 0.7882, 0.6902, 0.20f);
    ImVec4 accentA80   = ImVec4(0.2196, 0.7882, 0.6902, 0.80f);
    ImVec4 accentA90   = ImVec4(0.2196, 0.7882, 0.6902, 0.90f);

    colors[ImGuiCol_Text]            = ImVec4(1,1,1,1);
    colors[ImGuiCol_TextDisabled]    = ImVec4(0.5f,0.5f,0.5f,1);

    colors[ImGuiCol_WindowBg]        = ImVec4(0.06f,0.06f,0.06f,0.94f);
    colors[ImGuiCol_PopupBg]         = ImVec4(0.08f,0.08f,0.08f,0.94f);

    colors[ImGuiCol_Border]          = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_FrameBg]         = accentA40;
    colors[ImGuiCol_FrameBgHovered]  = accentA80;
    colors[ImGuiCol_FrameBgActive]   = accent;

    colors[ImGuiCol_TitleBg]         = ImVec4(0.04f,0.04f,0.04f,1);
    colors[ImGuiCol_TitleBgActive]   = accentA40;

    colors[ImGuiCol_ScrollbarGrab]   = accentA40;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentA80;
    colors[ImGuiCol_ScrollbarGrabActive]  = accent;

    colors[ImGuiCol_CheckMark]       = accent;

    colors[ImGuiCol_SliderGrab]      = accentA40;
    colors[ImGuiCol_SliderGrabActive]= accent;

    colors[ImGuiCol_Button]          = accentA40;
    colors[ImGuiCol_ButtonHovered]   = accentA80;
    colors[ImGuiCol_ButtonActive]    = accent;

    colors[ImGuiCol_Header]          = accentA31;
    colors[ImGuiCol_HeaderHovered]   = accentA80;
    colors[ImGuiCol_HeaderActive]    = accent;

    colors[ImGuiCol_Separator]       = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_ResizeGrip]      = accentA20;
    colors[ImGuiCol_ResizeGripHovered]= accentA80;
    colors[ImGuiCol_ResizeGripActive]= accent;

    colors[ImGuiCol_Tab]             = accentA31;
    colors[ImGuiCol_TabHovered]      = accentA80;
    colors[ImGuiCol_TabSelected]     = accentA40;

    colors[ImGuiCol_TextSelectedBg]  = ImVec4(0.98f,0.59f,0.26f,0.35f);

    colors[ImGuiCol_DragDropTarget]  = ImVec4(1,1,0,0.9f);
}

void setColorsYellow() {
     ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImVec4 accent      = ImVec4(1.0f, 0.8431f, 0.0667f, 1.00f);
    ImVec4 accentA40   = ImVec4(1.0f, 0.8431f, 0.0667f, 0.40f);
    ImVec4 accentA31   = ImVec4(1.0f, 0.8431f, 0.0667f, 0.31f);
    ImVec4 accentA20   = ImVec4(1.0f, 0.8431f, 0.0667f, 0.20f);
    ImVec4 accentA80   = ImVec4(1.0f, 0.8431f, 0.0667f, 0.80f);
    ImVec4 accentA90   = ImVec4(1.0f, 0.8431f, 0.0667f, 0.90f);

    colors[ImGuiCol_Text]            = ImVec4(1,1,1,1);
    colors[ImGuiCol_TextDisabled]    = ImVec4(0.5f,0.5f,0.5f,1);

    colors[ImGuiCol_WindowBg]        = ImVec4(0.06f,0.06f,0.06f,0.94f);
    colors[ImGuiCol_PopupBg]         = ImVec4(0.08f,0.08f,0.08f,0.94f);

    colors[ImGuiCol_Border]          = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_FrameBg]         = accentA40;
    colors[ImGuiCol_FrameBgHovered]  = accentA80;
    colors[ImGuiCol_FrameBgActive]   = accent;

    colors[ImGuiCol_TitleBg]         = ImVec4(0.04f,0.04f,0.04f,1);
    colors[ImGuiCol_TitleBgActive]   = accentA40;

    colors[ImGuiCol_ScrollbarGrab]   = accentA40;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentA80;
    colors[ImGuiCol_ScrollbarGrabActive]  = accent;

    colors[ImGuiCol_CheckMark]       = accent;

    colors[ImGuiCol_SliderGrab]      = accentA40;
    colors[ImGuiCol_SliderGrabActive]= accent;

    colors[ImGuiCol_Button]          = accentA40;
    colors[ImGuiCol_ButtonHovered]   = accentA80;
    colors[ImGuiCol_ButtonActive]    = accent;

    colors[ImGuiCol_Header]          = accentA31;
    colors[ImGuiCol_HeaderHovered]   = accentA80;
    colors[ImGuiCol_HeaderActive]    = accent;

    colors[ImGuiCol_Separator]       = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_ResizeGrip]      = accentA20;
    colors[ImGuiCol_ResizeGripHovered]= accentA80;
    colors[ImGuiCol_ResizeGripActive]= accent;

    colors[ImGuiCol_Tab]             = accentA31;
    colors[ImGuiCol_TabHovered]      = accentA80;
    colors[ImGuiCol_TabSelected]     = accentA40;

    colors[ImGuiCol_TextSelectedBg]  = ImVec4(0.98f,0.59f,0.26f,0.35f);

    colors[ImGuiCol_DragDropTarget]  = ImVec4(1,1,0,0.9f);
}

void setColorsOrange() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImVec4 accent      = ImVec4(0.941176f, 0.450980f, 0.121569f, 1.00f);
    ImVec4 accentA40   = ImVec4(0.941176f, 0.450980f, 0.121569f, 0.40f);
    ImVec4 accentA31   = ImVec4(0.941176f, 0.450980f, 0.121569f, 0.31f);
    ImVec4 accentA20   = ImVec4(0.941176f, 0.450980f, 0.121569f, 0.20f);
    ImVec4 accentA80   = ImVec4(0.941176f, 0.450980f, 0.121569f, 0.80f);
    ImVec4 accentA90   = ImVec4(0.941176f, 0.450980f, 0.121569f, 0.90f);

    colors[ImGuiCol_Text]            = ImVec4(1,1,1,1);
    colors[ImGuiCol_TextDisabled]    = ImVec4(0.5f,0.5f,0.5f,1);

    colors[ImGuiCol_WindowBg]        = ImVec4(0.06f,0.06f,0.06f,0.94f);
    colors[ImGuiCol_PopupBg]         = ImVec4(0.08f,0.08f,0.08f,0.94f);

    colors[ImGuiCol_Border]          = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_FrameBg]         = accentA40;
    colors[ImGuiCol_FrameBgHovered]  = accentA80;
    colors[ImGuiCol_FrameBgActive]   = accent;

    colors[ImGuiCol_TitleBg]         = ImVec4(0.04f,0.04f,0.04f,1);
    colors[ImGuiCol_TitleBgActive]   = accentA40;

    colors[ImGuiCol_ScrollbarGrab]   = accentA40;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentA80;
    colors[ImGuiCol_ScrollbarGrabActive]  = accent;

    colors[ImGuiCol_CheckMark]       = accent;

    colors[ImGuiCol_SliderGrab]      = accentA40;
    colors[ImGuiCol_SliderGrabActive]= accent;

    colors[ImGuiCol_Button]          = accentA40;
    colors[ImGuiCol_ButtonHovered]   = accentA80;
    colors[ImGuiCol_ButtonActive]    = accent;

    colors[ImGuiCol_Header]          = accentA31;
    colors[ImGuiCol_HeaderHovered]   = accentA80;
    colors[ImGuiCol_HeaderActive]    = accent;

    colors[ImGuiCol_Separator]       = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);;

    colors[ImGuiCol_ResizeGrip]      = accentA20;
    colors[ImGuiCol_ResizeGripHovered]= accentA80;
    colors[ImGuiCol_ResizeGripActive]= accent;

    colors[ImGuiCol_Tab]             = accentA31;
    colors[ImGuiCol_TabHovered]      = accentA80;
    colors[ImGuiCol_TabSelected]     = accentA40;

    colors[ImGuiCol_TextSelectedBg]  = ImVec4(0.98f,0.59f,0.26f,0.35f);

    colors[ImGuiCol_DragDropTarget]  = ImVec4(1,1,0,0.9f);
}