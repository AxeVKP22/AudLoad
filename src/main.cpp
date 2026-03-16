#define WINDOW_SIZE_X 500
#define WINDOW_SIZE_Y 350

#include <iostream>

#include "gui/init/init.hpp"
#include "gui/gui/gui.hpp"
#include "audio/audio.hpp"

ma_device device;
ma_decoder decoder;

bool updateAudioBuffer = true;

int main() {

    GLFWwindow* window = initWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "AudLoader");
    if (window == NULL) {
        std::cout << "Failed to initialize window."; //ww
    }
    initImGui(window);

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(0.0706f, 0.0431f, 0.0706f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        newFrame();

        downloadWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, audio, device, decoder);
        audioWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, audio, device);

        render();
        glfwSwapBuffers(window);
    }

    imGuiShutdown();
    glfwTerminate();

    return 0;
}