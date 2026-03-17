#include <windows.h>
#include <commdlg.h>
#include <string>
#include <fstream>
#include "../audio/audio.hpp"

extern std::string getSavePath();
extern void saveFileWav(std::string path, audioParams audio);
extern void saveFileMP3(std::string path, audioParams audio);