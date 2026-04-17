#pragma once

#include <windows.h>
#include <commdlg.h>
#include <string>
#include <fstream>
#include <filesystem>
#include "../../audio/audio.hpp"
#include "single_include/nlohmann/json.hpp"

struct settingsParams {
    int theme = 1; // 0 = white, 1  = blue, 2 = red, 3 = green, 4 = yellow, 5 = orange
    bool welcomeWindow = true;
    bool showWelcomeWindow = true;
};

extern std::string getSavePath(std::string appDir);
extern void saveFileWav(std::string path, audioParams audio);
extern void saveFileMP3(std::string path, audioParams audio);
extern void saveSettings(std::string path, settingsParams settings);