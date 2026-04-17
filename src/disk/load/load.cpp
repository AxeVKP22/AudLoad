#include "load.hpp"

void loadSettings(std::string path, settingsParams* settings) {
    std::ifstream f(path);
    nlohmann::json data = nlohmann::json::parse(f);
    if (data.contains("theme")) {
        settings->theme = data["theme"];
    }
    if (data.contains("welcomeWindow")) {
        settings->showWelcomeWindow = data["welcomeWindow"];
        settings->welcomeWindow = data["welcomeWindow"];
    }
}