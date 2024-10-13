#pragma once

#include "logging.hpp"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <thread>
#include <future>


// command-line parameters
struct WhisperParams {
    // do not fuck with the names of these params thinking it dosent follow style, these have to be as it is, dependency of whisper.cpp
    int32_t n_threads = std::min(4, (int32_t)std::thread::hardware_concurrency());
    int32_t step_ms = 1200;
    int32_t length_ms = 5000;
    int32_t keep_ms = 600;
    int32_t capture_id = -1;
    int32_t max_tokens = 64;
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
   
    bool silenceStart = false;

    std::string paramStructToStringConverter();

    //std::chrono::time_point<std::chrono::high_resolution_clock> beg = std::chrono::high_resolution_clock::now();

public:
    
    WhisperParams params;
    TranscribeService()
    {
        if (std::filesystem::exists("/usr/lib/libcuda.so"))
            this->params.no_gpu = false;
        else
            this->params.no_gpu = true;
    }

    std::string run();
    std::future<std::string> asyncRun();
};


