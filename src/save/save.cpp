#include "save.hpp"

std::string getSavePath() {
    char szFile[MAX_PATH] = {0};

    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "WAV Files\0*.wav\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = "Save...";
    ofn.Flags = OFN_OVERWRITEPROMPT;
    if (GetSaveFileName(&ofn)) {    
        std::string fl = ofn.lpstrFile;
        if (fl.find('.') == std::string::npos) {
            fl += ".wav";
        }
        return fl;
    }
    return "";
}

void saveFile(std::string path, audioParams audio) {
    std::ofstream out(path, std::ios::binary);
    out.write((char*)audio.audiofile.data(), audio.audiofile.size());
    out.close();
}