#include <windows.h>
#include <commdlg.h>
#include <string>
#include <fstream>
#include "../audio/audio.hpp"

extern std::string getSavePath();
extern void saveFile(std::string path, audioParams audio);