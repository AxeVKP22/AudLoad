#pragma once
#include <string>
#include <fstream>

#include "single_include/nlohmann/json.hpp"

#include "../save/save.hpp"
#include "../../gui/gui/gui.hpp"

extern void loadSettings(std::string path, settingsParams* settings);