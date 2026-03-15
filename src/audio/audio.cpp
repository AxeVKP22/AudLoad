#include "audio.hpp"

WAVEFORMATEX wf = {};

char videoUrl[256];
audioParams audio;                                                                  

std::string ytExeDir = ".\\bin\\yt_dlp\\yt-dlp_x86.exe";
std::string ffExeDir = ".\\bin\\ffmpeg\\ffmpeg.exe";

audioParams downloadAndConvertAudio(char *url) {
    std::string command = "cmd /C \"" + ytExeDir + " -f bestaudio -o - " + std::string(url) + " | " + ffExeDir + " -loglevel quiet -i pipe:0 -f wav pipe:1\"";
    audioParams downloadedAudio;

    FILE* pipe = _popen(command.c_str(), "r");
    unsigned char buffer[4096];

    while (true) {
        size_t bytes = fread(buffer, 1 , sizeof(buffer), pipe);
        if (bytes == 0) {
            break;
        }
        downloadedAudio.audiofile.insert(downloadedAudio.audiofile.end(), buffer, buffer + bytes);
    }

    _pclose(pipe);

    return downloadedAudio;
}

HWAVEOUT initAudio() {
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.nSamplesPerSec = 44100;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL);

    return hWaveOut;
}

WAVEHDR setUpAudioBuffer(HWAVEOUT ad, audioParams audio) {
    WAVEHDR header = {};
    header.lpData = (LPSTR)audio.audiofile.data();
    header.dwBufferLength = audio.audiofile.size();

    waveOutPrepareHeader(ad, &header, sizeof(header));
    return header;
}

void playAudio(HWAVEOUT ad, WAVEHDR header) {
    waveOutWrite(ad, &header, sizeof(header));
}

void pauseAudio(HWAVEOUT ad) {
    waveOutPause(ad);
}

void startAudio(HWAVEOUT ad) {
    waveOutRestart(ad);
}

void setAudioVolume(HWAVEOUT ad, float volume) {
    waveOutSetVolume(ad, volume);
}
