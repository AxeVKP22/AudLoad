#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

extern GLFWwindow* initWindow(int xSize, int Ysize, const char* title);

extern void initImGui(GLFWwindow* window);
extern void render();
extern void newFrame();
extern void imGuiShutdown();
