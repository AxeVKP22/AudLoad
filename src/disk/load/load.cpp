#include "load.hpp"

void loadSettings(std::string path, settingsParams* settings) {
    std::ifstream f(path);
    nlohmann::json data = nlohmann::json::parse(f);
    if (data.contains("theme")) {
        settings->theme = data["theme"];
    }
    if (data.contains("welcomeWindow")) {
        settings->showWelcomeWindow = data["welcomeWindow"];
        settings->welcomeWindow = data["welcomeWindow"];
    }
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void donwloadYtDlp() {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe";
    char outfilename[FILENAME_MAX] = "bin/yt_dlp/yt-dlp.exe";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}