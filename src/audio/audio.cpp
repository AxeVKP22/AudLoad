#define MINIAUDIO_IMPLEMENTATION
#include "audio.hpp"

char videoUrl[256];
audioParams audio;

std::string ytExeDir = ".\\bin\\yt_dlp\\yt-dlp_x86.exe";
std::string ffExeDir = ".\\bin\\ffmpeg\\ffmpeg.exe";

audioParams downloadAudio(const char* url) {
    audioParams downloadedAudio;
    std::string downloadCommand = ytExeDir + " --no-playlist --no-warnings --no-progress -f bestaudio[ext=m4a] -o temp\\temp.%(ext)s " + url;
    std::system(downloadCommand.c_str());

    std::string nameCommand = ytExeDir + " --no-playlist --no-warnings --no-progress " "--print \"%(title).200s\t%(uploader).100s\t%(duration)s\" " + url;

    FILE* pipe = _popen(nameCommand.c_str(), "r");

    char buffer[1024];
    std::string output;
    if (fgets(buffer, sizeof(buffer), pipe)) {
        output = buffer;
        output.erase(output.find_last_not_of("\r\n") + 1);
    }

    _pclose(pipe);

    std::stringstream ss(output);

    std::getline(ss, downloadedAudio.audiofileName, '\t');
    std::getline(ss, downloadedAudio.audioAuthor, '\t');
    std::getline(ss, downloadedAudio.audioDuration, '\t');

    downloadedAudio.loaded = true;
    downloadedAudio.isPlaying = false;
    downloadedAudio.volume = 0.5;

    return downloadedAudio;
}

bool convertAudio(audioParams &audio) {
    std::string command = ffExeDir + " -i temp\\temp.m4a -ac 2 -ar 44100 -f wav pipe:1";

    FILE* pipe = _popen(command.c_str(), "rb");
    unsigned char buffer[65536];

    while (true) {
        size_t bytes = fread(buffer, 1 , sizeof(buffer), pipe);
        if (bytes == 0) {
            break;
        }
        audio.audiofile.insert(audio.audiofile.end(), buffer, buffer + bytes);
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

void unInitAudio(ma_decoder &decoder ,ma_device &device) {
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

bool setUpAudioDevice(ma_decoder &decoder, ma_device &device, audioParams &audio) {
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

    ma_decoder_get_length_in_pcm_frames(&decoder, &audio.totalFrames);

    return true;
}

void playAudio(ma_device &device) {
    ma_device_start(&device);
}

void pauseAudio(ma_device &device) {
    ma_device_stop(&device);
}

void setAudioVolume(ma_device &device, float volume) {
    ma_device_set_master_volume(&device, volume);
}