#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>

#include "miniaudio.h"


struct audioParams
{
    std::vector<unsigned char> audiofile;
    std::string audiofileName, audioDuration, audioAuthor;
    bool loaded;
};

extern char videoUrl[256];
extern audioParams audio;
extern bool isPlaying;

extern std::string ytExeDir;
extern std::string ffExeDir;

extern audioParams downloadAudio(char *url);
extern bool convertAudio(audioParams *audio);

extern bool initAudio(audioParams &audio, ma_decoder &decoder);
extern void unInitAudio(ma_decoder &decoder ,ma_device &device);
extern bool setUpAudioDevice(ma_decoder &decoder ,ma_device &device);
extern void playAudio(ma_device &device);
extern void pauseAudio();
extern void startAudio();
extern void setAudioVolume(float volume);