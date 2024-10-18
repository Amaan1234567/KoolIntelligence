#pragma once

#include "logging.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <thread>

// command-line parameters
struct WhisperParams {
    // do not fuck with the names of these params thinking it dosent follow style, these have to be as it is, dependency of whisper.cpp
    int32_t n_threads = std::min(4, (int32_t)std::thread::hardware_concurrency());
    int32_t step_ms = 100;
    int32_t length_ms = 5000;
    int32_t keep_ms = 0;
    int32_t capture_id = -1;
    int32_t max_tokens = 32;
    int32_t audio_ctx = 0;

    float vad_thold = 0.8f;
    float freq_thold = 100.0f;

    std::string translate = "false";
    std::string no_fallback = "false";
    std::string print_special = "false";
    std::string keep_context = "true";
    std::string tinydiarize = "false";
    std::string save_audio = "false"; // save audio to wav file
    bool no_gpu = true;
    bool flash_attn = true;

    std::string language = "en";
    std::string model = std::string(getenv("HOME")) + "/koolintelligence/models/ggml-small.en.bin";
    std::string fname_out;
};

class TranscribeService
{
private:
    bool silenceStart = false;

    std::string paramStructToStringConverter();
    void asyncService(std::string* result, bool* isRunning);
    WhisperParams params;

public:
    TranscribeService();
    //the string and bool have to be manually deleted after use
    //returns the result of the transcription and a bool to check if the service is still running
    //The string will be continuously updated with the transcription
    std::pair<std::string*, bool*> startService(); 
};
