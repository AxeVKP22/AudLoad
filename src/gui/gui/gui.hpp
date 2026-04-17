#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <thread>

#include "windows.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "../../audio/audio.hpp"
#include "../../disk/save/save.hpp"

extern settingsParams settings;
extern std::string workingDir;

extern bool settingsWindowV;

extern const char* colors[];

extern void downloadWindow(int sizeX, int sizeY, audioParams &audio,  ma_device &device, ma_decoder &decoder);
extern void audioWindow(int sizeX, int sizeY, audioParams &audio, ma_device &device);
extern void welcomeWindow();
extern void settingsWindow();

extern bool downloadFromUrl(const char* url, audioParams &audio, ma_decoder &decoder, ma_device &device);

extern void openAndSelect(const std::string& filePath);

extern void setColorsRed();
extern void setColorsGreen();
extern void setColorsYellow();
extern void setColorsOrange();