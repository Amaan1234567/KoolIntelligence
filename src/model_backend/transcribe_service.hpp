#pragma once
#include "common-sdl.h"
#include "common.h"
#include "logging.hpp"
#include "whisper.h"

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

    bool translate = false;
    bool no_fallback = false;
    bool print_special = false;
    bool no_context = true;
    bool no_timestamps = false;
    bool tinydiarize = false;
    bool save_audio = false; // save audio to wav file
    bool use_gpu = false;
    bool flash_attn = true;

    std::string language = "en";
    std::string model = std::string(getenv("HOME")) + "/koolintelligence/models/ggml-small.en.bin";
    std::string fname_out;
    int timeoutDuration = 5;
};

class TranscribeService
{
    WhisperParams params;
    bool silenceStart = false;

    bool whisperParamsParse(std::vector<std::string> argv);

    bool timeoutChecker(std::string &output);

    std::chrono::time_point<std::chrono::high_resolution_clock> beg = std::chrono::high_resolution_clock::now();

public:
    TranscribeService()
    {
        if (std::filesystem::exists("/usr/lib/libcuda.so"))
            this->params.use_gpu = true;
        else
            this->params.use_gpu = false;
    }

    std::string run(std::vector<std::string> argv = std::vector<std::string>());
};