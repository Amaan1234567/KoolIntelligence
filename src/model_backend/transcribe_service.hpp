#pragma once
#include "common-sdl.h"
#include "common.h"
#include "logging.hpp"
#include "whisper.h"

#include <cassert>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// command-line parameters
struct whisper_params {
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
    bool save_audio = true; // save audio to wav file
    bool use_gpu = true;
    bool flash_attn = true;

    std::string language = "en";
    std::string model = "../../src/model_backend/thirdparty/whisper.cpp/models/ggml-small.en.bin";
    std::string fname_out = "../../src/model_backend/thirdparty/whisper.cpp/samples/output_from_koointelligence.wav";

    int TimeoutDuration = 5;
};

class TranscribeService
{
    bool whisper_params_parse(std::vector<std::string> argv, whisper_params &params);

    bool timeout_checker(std::string &output, whisper_params &params);

    std::chrono::time_point<std::chrono::high_resolution_clock> beg = std::chrono::high_resolution_clock::now();
    bool silence_start = false;

public:
    std::string run(std::vector<std::string> argv = std::vector<std::string>());
};