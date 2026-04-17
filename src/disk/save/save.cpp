#include "save.hpp"

namespace fs = std::filesystem;

std::string getSavePath(std::string appDir) {
    char szFile[MAX_PATH] = {0};

    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "WAV Files\0*.wav\0""MP3 Files\0*.mp3\0""All Files\0*.*\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrTitle = "Save...";
    ofn.Flags = OFN_OVERWRITEPROMPT || OFN_NOCHANGEDIR;
    if (GetSaveFileName(&ofn)) {    
        std::string fl = ofn.lpstrFile;
        switch (ofn.nFilterIndex)
        {
        case 1:
            fl += ".wav";
            break;
        case 2:
            fl += ".mp3";
            break;
        }
        fs::current_path(appDir);
        return fl;
    }
    fs::current_path(appDir);
    return "";
}

void saveFileWav(std::string path, audioParams audio) {
    std::ofstream out(path, std::ios::binary);
    out.write((char*)audio.audiofile.data(), audio.audiofile.size());
    out.close();
}

void saveFileMP3(std::string path, audioParams audio) {
    saveFileWav("temp/temp.wav", audio);

    std::string ffExeDir = ".\\bin\\ffmpeg\\ffmpeg.exe";

    std::string command = "cmd /C \"" + ffExeDir + " -y -i temp/temp.wav " + path + " -hide_banner\"";

    std::system(command.c_str());

    std::remove("temp/temp.wav");
} 

void saveSettings(std::string path, settingsParams settings) {
    nlohmann::json j;
    j["theme"] = settings.theme;
    j["welcomeWindow"] = settings.welcomeWindow;

    std::ofstream f(path);
    f << j.dump(4);
}