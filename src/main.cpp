#define WINDOW_SIZE_X 500
#define WINDOW_SIZE_Y 350

#include <iostream>
#include <fstream>

#include "gui/init/init.hpp"
#include "gui/gui/gui.hpp"
#include "audio/audio.hpp"
#include "disk/load/load.hpp"

ma_device device;
ma_decoder decoder;

bool updateAudioBuffer = true;

int main() {

    GLFWwindow* window = initWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "AudLoader");
    if (window == NULL) {
        std::cout << "Failed to initialize window."; //ww
    }
    initImGui(window);

    std::string loadPath = workingDir + "settings.json";

    std::ifstream f(loadPath);
    if (f.is_open()) {
        loadSettings(loadPath, &settings);
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0706f, 0.0431f, 0.0706f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        newFrame();


        switch (settings.theme) {
        case 0: ImGui::StyleColorsLight(); break;
        case 1: ImGui::StyleColorsDark(); break;
        case 2: setColorsRed(); break;
        case 3: setColorsGreen(); break;
        case 4: setColorsYellow(); break;
        case 5: setColorsOrange(); break;
        }

        if (settingsWindowV) {
            settingsWindow();
        }

        downloadWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, audio, device, decoder);
        audioWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, audio, device);

        if (settings.showWelcomeWindow) {
            welcomeWindow();
        }

        render();
        glfwSwapBuffers(window);
    }

    unInitAudio(decoder, device);
    imGuiShutdown();
    glfwTerminate();

    return 0;
}