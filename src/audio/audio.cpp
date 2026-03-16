#define MINIAUDIO_IMPLEMENTATION
#include "audio.hpp"

char videoUrl[256];
audioParams audio;                           

bool isPlaying = false;

std::string ytExeDir = ".\\bin\\yt_dlp\\yt-dlp_x86.exe";
std::string ffExeDir = ".\\bin\\ffmpeg\\ffmpeg.exe";

audioParams downloadAudio(char *url) {
    audioParams downloadedAudio;
    std::string downloadCommand = ytExeDir + " -f bestaudio[ext=m4a] -o temp\\temp.%(ext)s " + url;
    std::system(downloadCommand.c_str());

    std::string nameCommand = ytExeDir + " --print title " + url;
    FILE* pipe = _popen(nameCommand.c_str(), "r");

    char buffer[512];
    fgets(buffer, sizeof(buffer), pipe);

    _pclose(pipe);

    downloadedAudio.audiofileName = std::string(buffer);

    return downloadedAudio;
}

bool convertAudio(audioParams *audio) {
    std::string command = ffExeDir + " -i temp\\temp.m4a -ac 2 -ar 44100 -f wav pipe:1";

    FILE* pipe = _popen(command.c_str(), "rb");
    unsigned char buffer[65536];

    while (true) {
        size_t bytes = fread(buffer, 1 , sizeof(buffer), pipe);
        if (bytes == 0) {
            break;
        }
        audio->audiofile.insert(audio->audiofile.end(), buffer, buffer + bytes);
    }

    _pclose(pipe);
    std::remove("temp\\temp.m4a");
    return true;
}

bool initAudio(audioParams &audio, ma_decoder &decoder) {
    ma_result result;

    result = ma_decoder_init_memory(audio.audiofile.data(), audio.audiofile.size(), nullptr, &decoder);
    if (result != MA_SUCCESS) {
        return false;
    }

    return true;
}

bool setUpAudioDevice(ma_decoder &decoder, ma_device &device) {
    ma_result result;
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = [](ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
    };
    deviceConfig.pUserData = &decoder;

    result = ma_device_init(nullptr, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        ma_decoder_uninit(&decoder);
        return false;
    }

    return true;
}

void playAudio(ma_device &device) {
    if (!isPlaying) {
        ma_device_start(&device);
    }
}

void pauseAudio() {
    if (isPlaying) {
        
    }
}

void startAudio() {
    if (!isPlaying) {
        
    }
}

void setAudioVolume(float volume) {
    
}