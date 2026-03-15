#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <cstdio>
#include <string>

#include <windows.h>
#include <mmsystem.h>


struct audioParams
{
    std::vector<unsigned char> audiofile;
    std::string audiofileName;
};

extern WAVEFORMATEX wf;

extern char videoUrl[256];
extern audioParams audio;

extern std::string ytExeDir;
extern std::string ffExeDir;

extern audioParams downloadAndConvertAudio(char *url);
extern HWAVEOUT initAudio();
extern WAVEHDR setUpAudioBuffer(HWAVEOUT ad, audioParams audio);
extern void playAudio(HWAVEOUT ad, WAVEHDR header);
extern void pauseAudio(HWAVEOUT ad);
extern void startAudio(HWAVEOUT ad);
extern void setAudioVolume(HWAVEOUT ad, float volume);