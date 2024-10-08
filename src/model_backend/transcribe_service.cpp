#include "transcribe_service.hpp"

//NOTE: DONOT MESS WITH ANY VARIABLES HERE THAT DONT FOLLOW STYLE GUIDELINES THEY ARE VARIABLES USED INTERNALLY
// IN WHISPER.CPP, THEY CHANGE, AND IT WILL DEFINITELY BREAK SOMETHING
bool TranscribeService::whisperParamsParse(std::vector<std::string> argv)
{
    for (size_t i = 1; i < argv.size(); i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            LOG_ERROR("TranscribeService", "wrong parameter names. please double check them");
            exit(0);
        } else if (arg == "-t" || arg == "--threads") {
            this->params.n_threads = std::stoi(argv[++i]);
        } else if (arg == "--step") {
            this->params.step_ms = std::stoi(argv[++i]);
        } else if (arg == "--length") {
            this->params.length_ms = std::stoi(argv[++i]);
        } else if (arg == "--keep") {
            this->params.keep_ms = std::stoi(argv[++i]);
        } else if (arg == "-c" || arg == "--capture") {
            this->params.capture_id = std::stoi(argv[++i]);
        } else if (arg == "-mt" || arg == "--max-tokens") {
            this->params.max_tokens = std::stoi(argv[++i]);
        } else if (arg == "-ac" || arg == "--audio-ctx") {
            this->params.audio_ctx = std::stoi(argv[++i]);
        } else if (arg == "-vth" || arg == "--vad-thold") {
            this->params.vad_thold = std::stof(argv[++i]);
        } else if (arg == "-fth" || arg == "--freq-thold") {
            this->params.freq_thold = std::stof(argv[++i]);
        } else if (arg == "-tr" || arg == "--translate") {
            this->params.translate = true;
        } else if (arg == "-nf" || arg == "--no-fallback") {
            this->params.no_fallback = true;
        } else if (arg == "-ps" || arg == "--print-special") {
            this->params.print_special = true;
        } else if (arg == "-kc" || arg == "--keep-context") {
            this->params.no_context = false;
        } else if (arg == "-l" || arg == "--language") {
            this->params.language = argv[++i];
        } else if (arg == "-m" || arg == "--model") {
            this->params.model = argv[++i];
        } else if (arg == "-f" || arg == "--file") {
            this->params.fname_out = argv[++i];
        } else if (arg == "-tdrz" || arg == "--tinydiarize") {
            this->params.tinydiarize = true;
        } else if (arg == "-sa" || arg == "--save-audio") {
            this->params.save_audio = true;
        } else if (arg == "-ng" || arg == "--no-gpu") {
            this->params.use_gpu = false;
        } else if (arg == "-fa" || arg == "--flash-attn") {
            this->params.flash_attn = true;
        }

        else {
            LOG_ERROR("TranscribeService", ("error: unknown argument: " + arg).c_str());
            exit(0);
        }
    }

    return true;
}

std::string TranscribeService::run(std::vector<std::string> argv)
{
    std::string transcript = "";

    if (argv.size() == 0) {
        LOG_DEBUG("TranscribeService", "found no arguments specifically set, using defaults");
    } else if (whisperParamsParse(argv) == false) {
        LOG_ERROR("TranscribeService", "couldnt parse arguments :(");
        return "";
    }

    this->params.keep_ms = std::min(this->params.keep_ms, this->params.step_ms);
    this->params.length_ms = std::max(this->params.length_ms, this->params.step_ms);

    const int n_samples_step = (1e-3 * this->params.step_ms) * WHISPER_SAMPLE_RATE;
    const int n_samples_len = (1e-3 * this->params.length_ms) * WHISPER_SAMPLE_RATE;
    const int n_samples_keep = (1e-3 * this->params.keep_ms) * WHISPER_SAMPLE_RATE;
    const int n_samples_30s = (1e-3 * 30000.0) * WHISPER_SAMPLE_RATE;

    const bool use_vad = n_samples_step <= 0; // sliding window mode uses VAD

    const int n_new_line = !use_vad ? std::max(1, this->params.length_ms / this->params.step_ms - 1) : 1; // number of steps to print new line

    this->params.no_timestamps = !use_vad;
    this->params.no_context |= use_vad;
    this->params.max_tokens = 0;

    // init audio

    audio_async audio(this->params.length_ms);
    if (!audio.init(this->params.capture_id, WHISPER_SAMPLE_RATE)) {
        LOG_ERROR("TranscribeService", std::string(__func__) + ": audio.init() failed!\n");
        return "";
    }

    audio.resume();

    // whisper init
    if (this->params.language != "auto" && whisper_lang_id(this->params.language.c_str()) == -1) {
        LOG_ERROR("TranscribeService", ("error: unknown language " + this->params.language).c_str());
        return "";
    }

    struct whisper_context_params cparams = whisper_context_default_params();

    cparams.use_gpu = this->params.use_gpu;
    cparams.flash_attn = this->params.flash_attn;

    struct whisper_context *ctx = whisper_init_from_file_with_params(this->params.model.c_str(), cparams);

    std::vector<float> pcmf32(n_samples_30s, 0.0f);
    std::vector<float> pcmf32_old;
    std::vector<float> pcmf32_new(n_samples_30s, 0.0f);

    std::vector<whisper_token> prompt_tokens;

    // print some info about the processing
    {
        LOG_INFO("TranscribeService", "\n");
        if (!whisper_is_multilingual(ctx)) {
            if (this->params.language != "en" || this->params.translate) {
                this->params.language = "en";
                this->params.translate = false;
                LOG_ERROR("TranscribeService", std::string(__func__) + ": WARNING: model is not multilingual, ignoring language and translation options\n");
            }
        }
        LOG_INFO("TranscribeService",
                 std::string(__func__) + ": processing " + std::to_string(n_samples_step)
                     + " samples (step = " + std::to_string(float(n_samples_step) / WHISPER_SAMPLE_RATE)
                     + " sec / len = " + std::to_string(float(n_samples_len) / WHISPER_SAMPLE_RATE)
                     + " sec / keep = " + std::to_string(float(n_samples_keep) / WHISPER_SAMPLE_RATE) + " sec), " + std::to_string(this->params.n_threads)
                     + " threads, lang = " + this->params.language + ", task = " + (this->params.translate ? "translate" : "transcribe")
                     + ", timestamps = " + (this->params.no_timestamps ? "0" : "1") + " ...\n");

        if (!use_vad) {
            LOG_INFO("TranscribeService",
                     std::string(__func__) + ": n_new_line = " + std::to_string(n_new_line) + ", no_context = " + std::to_string(this->params.no_context) + "\n");
        } else {
            LOG_INFO("TranscribeService", std::string(__func__) + ": using VAD, will transcribe on speech activity\n");
        }

        LOG_INFO("TranscribeService", "\n");
    }

    int n_iter = 0;

    bool is_running = true;

    std::ofstream fout;
    if (this->params.fname_out.length() > 0) {
        fout.open(this->params.fname_out);
        if (!fout.is_open()) {
            LOG_ERROR("TranscribeService", std::string(__func__) + ": failed to open output file \'" + this->params.fname_out + "\'!\n");
            return "";
        }
    }

    wav_writer wavWriter;
    // save wav file
    if (this->params.save_audio) {
        // Get current date/time for filename
        time_t now = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", localtime(&now));
        std::string filename = std::string(buffer) + ".wav";

        wavWriter.open(filename, WHISPER_SAMPLE_RATE, 16, 1);
    }
    // printf("[Start speaking]\n");
    fflush(stdout);

    auto t_last = std::chrono::high_resolution_clock::now();
    const auto t_start = t_last;

    // main audio loop
    while (is_running) {
        if (this->params.save_audio) {
            wavWriter.write(pcmf32_new.data(), pcmf32_new.size());
        }
        // handle Ctrl + C
        is_running = sdl_poll_events();

        if (!is_running) {
            break;
        }

        // process new audio

        if (!use_vad) {
            while (true) {
                audio.get(this->params.step_ms, pcmf32_new);

                if ((int)pcmf32_new.size() > 2 * n_samples_step) {
                    LOG_ERROR("TranscribeService", std::string(__func__) + "\n\n%s: WARNING: cannot process audio fast enough, dropping audio ...\n\n");
                    audio.clear();
                    continue;
                }

                if ((int)pcmf32_new.size() >= n_samples_step) {
                    audio.clear();
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            const int n_samples_new = pcmf32_new.size();

            // take up to this->params.length_ms audio from previous iteration
            const int n_samples_take = std::min((int)pcmf32_old.size(), std::max(0, n_samples_keep + n_samples_len - n_samples_new));

            // printf("processing: take = %d, new = %d, old = %d\n", n_samples_take, n_samples_new, (int) pcmf32_old.size());

            pcmf32.resize(n_samples_new + n_samples_take);

            for (int i = 0; i < n_samples_take; i++) {
                pcmf32[i] = pcmf32_old[pcmf32_old.size() - n_samples_take + i];
            }

            memcpy(pcmf32.data() + n_samples_take, pcmf32_new.data(), n_samples_new * sizeof(float));

            pcmf32_old = pcmf32;
        } else {
            const auto t_now = std::chrono::high_resolution_clock::now();
            const auto t_diff = std::chrono::duration_cast<std::chrono::milliseconds>(t_now - t_last).count();

            if (t_diff < 2000) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                continue;
            }

            audio.get(2000, pcmf32_new);

            if (::vad_simple(pcmf32_new, WHISPER_SAMPLE_RATE, 1000, this->params.vad_thold, this->params.freq_thold, false)) {
                audio.get(this->params.length_ms, pcmf32);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                continue;
            }

            t_last = t_now;
        }

        // run the inference
        {
            whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);

            wparams.print_progress = false;
            wparams.print_special = this->params.print_special;
            wparams.print_realtime = false;
            wparams.print_timestamps = !this->params.no_timestamps;
            wparams.translate = this->params.translate;
            wparams.single_segment = !use_vad;
            wparams.max_tokens = this->params.max_tokens;
            wparams.language = this->params.language.c_str();
            wparams.n_threads = this->params.n_threads;

            wparams.audio_ctx = this->params.audio_ctx;

            wparams.tdrz_enable = this->params.tinydiarize; // [TDRZ]

            // disable temperature fallback
            // wparams.temperature_inc  = -1.0f;
            wparams.temperature_inc = this->params.no_fallback ? 0.0f : wparams.temperature_inc;

            wparams.prompt_tokens = this->params.no_context ? nullptr : prompt_tokens.data();
            wparams.prompt_n_tokens = this->params.no_context ? 0 : prompt_tokens.size();

            if (whisper_full(ctx, wparams, pcmf32.data(), pcmf32.size()) != 0) {
                LOG_ERROR("TranscribeService", (argv[0] + " : failed to process audio\n").c_str());
                return "";
            }

            // print result;
            {
                if (!use_vad) {
                    transcript += "\33[2K\r";

                    // print long empty line to clear the previous line
                    transcript += std::string(100, ' ');

                    transcript += "\33[2K\r";
                } else {
                    const int64_t t1 = (t_last - t_start).count() / 1000000;
                    const int64_t t0 = std::max(0.0, t1 - pcmf32.size() * 1000.0 / WHISPER_SAMPLE_RATE);

                    transcript += "\n";
                    transcript += "### Transcription " + std::to_string(n_iter) + "START | t0 = " + std::to_string((int)t0)
                        + " ms | t1 = " + std::to_string((int)t1) + " ms\n";
                    transcript += "\n";
                }

                const int n_segments = whisper_full_n_segments(ctx);
                for (int i = 0; i < n_segments; ++i) {
                    const char *text = whisper_full_get_segment_text(ctx, i);

                    if (this->params.no_timestamps) {
                        transcript += text;

                        std::string output = text;

                        if (this->timeoutChecker(output)) {
                            audio.pause();

                            whisper_print_timings(ctx);
                            whisper_free(ctx);
                            return "";
                        }

                    } else {
                        // const int64_t t0 = whisper_full_get_segment_t0(ctx, i);
                        // const int64_t t1 = whisper_full_get_segment_t1(ctx, i);

                        // std::string output = "[" + to_timestamp(t0, false) + " --> " + to_timestamp(t1, false) + "]  " + text;
                        std::string output = text;

                        if (this->timeoutChecker(output)) {
                            audio.pause();

                            whisper_print_timings(ctx);
                            whisper_free(ctx);
                            return "";
                        }

                        if (whisper_full_get_segment_speaker_turn_next(ctx, i)) {
                            output += " [SPEAKER_TURN]";
                        }

                        output += "\n";

                        transcript += output;

                        if (this->params.fname_out.length() > 0) {
                            fout << output;
                        }
                    }
                }

                /* if (this->params.fname_out.length() > 0) {
                    fout << std::endl;
                } */

                if (use_vad) {
                    transcript += "\n";
                    transcript += "### Transcription " + std::to_string(n_iter) + "END\n";
                }
            }

            ++n_iter;

            if (!use_vad && (n_iter % n_new_line) == 0) {
                transcript += "\n";

                // keep part of the audio for next iteration to try to mitigate word boundary issues
                pcmf32_old = std::vector<float>(pcmf32.end() - n_samples_keep, pcmf32.end());

                // Add tokens of the last full length segment as the prompt
                if (!this->params.no_context) {
                    prompt_tokens.clear();

                    const int n_segments = whisper_full_n_segments(ctx);
                    for (int i = 0; i < n_segments; ++i) {
                        const int token_count = whisper_full_n_tokens(ctx, i);
                        for (int j = 0; j < token_count; ++j) {
                            prompt_tokens.push_back(whisper_full_get_token_id(ctx, i, j));
                        }
                    }
                }
            }
        }
    }

    audio.pause();

    whisper_print_timings(ctx);
    whisper_free(ctx);

    return transcript;
}

bool TranscribeService::timeoutChecker(std::string &output)
{
    // first silence
    LOG_DEBUG("TranscribeService", "inside timeout_checker: " + output);
    LOG_DEBUG("TranscribeService", "inside timeout_checker: " + std::to_string(output.compare(" [BLANK_AUDIO]")));

    if (output.compare(" [BLANK_AUDIO]") == 0 && !this->silenceStart) {
        LOG_DEBUG("TranscribeService", "inside timeout_checker: silence started");
        this->beg = std::chrono::high_resolution_clock::now();
        this->silenceStart = true;

    } else if (output.compare(" [BLANK_AUDIO]") == 0 && this->silenceStart) // check for timeout
    {
        auto end = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - beg).count() >= this->params.timeoutDuration) {
            LOG_INFO("TranscribeService", "Transcription timed out due to silence");
            LOG_DEBUG("TranscribeService", "inside timeout_checker: silence timeout initiated");
            return true;
        }
    } else if (output.compare(" [BLANK_AUDIO]") != 0) {
        this->silenceStart = false;
        LOG_DEBUG("TranscribeService", "inside timeout_checker: silence reset");
    }

    return false;
}