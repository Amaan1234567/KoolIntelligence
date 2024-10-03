#pragma once
#include "common-sdl.h"
#include "common.h"
#include "whisper.h"
#include "logging.hpp"

#include <cassert>
#include <cstdio>
#include <string>
#include <thread>
#include <vector>
#include <fstream>


// command-line parameters
struct whisper_params {
    int32_t n_threads  = std::min(4, (int32_t) std::thread::hardware_concurrency());
    int32_t step_ms    = 1700;
    int32_t length_ms  = 2000;
    int32_t keep_ms    = 500;
    int32_t capture_id = -1;
    int32_t max_tokens = 32;
    int32_t audio_ctx  = 0;

    float vad_thold    = 0.6f;
    float freq_thold   = 100.0f;

    bool translate     = false;
    bool no_fallback   = false;
    bool print_special = false;
    bool no_context    = true;
    bool no_timestamps = false;
    bool tinydiarize   = false;
    bool save_audio    = false; // save audio to wav file
    bool use_gpu       = true;
    bool flash_attn    = true;

    std::string language  = "en";
    std::string model     = "models/ggml-small.en.bin";
    std::string fname_out;
};


class transcribe_service
{
    
    bool whisper_params_parse(std::vector<std::string> argv, whisper_params & params);

    public:
    int run(std::vector<std::string> argv = std::vector<std::string>());

};