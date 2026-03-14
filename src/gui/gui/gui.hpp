#include <string>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

extern char videoUrl[256];
extern std::vector<char*> audiofile;
extern std::string audiofileName;

extern void downloadWindow(int sizeX, int sizeY);
extern void audioWindow(int sizeX, int sizeY);