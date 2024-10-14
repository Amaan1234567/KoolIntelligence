#include "transcribe_service.hpp"

std::string TranscribeService::paramStructToStringConverter()
{
    std::string command = "";
    command += "--model " + this->params.model;
    if (this->params.fname_out.size() != 0)
        command += " --file " + this->params.fname_out;

    command += " --threads " + std::to_string(this->params.n_threads);
    command += " --step " + std::to_string(this->params.step_ms);
    command += " --length " + std::to_string(this->params.length_ms);
    command += " --keep " + std::to_string(this->params.keep_ms);
    command += " --capture " + std::to_string(this->params.capture_id);
    command += " --max-tokens " + std::to_string(this->params.max_tokens);
    command += " --audio-ctx " + std::to_string(this->params.audio_ctx);
    command += " --vad-thold " + std::to_string(this->params.vad_thold);
    command += " --freq-thold " + std::to_string(this->params.freq_thold);
    /* command += " --translate "+ this->params.translate;
    command += " --no-fallback "+ this->params.no_fallback;
    command += " --print-special "+ this->params.print_special;
    command += " --keep-context "+ this->params.keep_context;
    command += " --tinydiarize "+ this->params.tinydiarize;
    command += " --save-audio "+ this->params.save_audio; */
    if (this->params.no_gpu)
        command += " --no-gpu ";
    if (this->params.flash_attn)
        command += " --flash-attn ";
    return command;
}

std::future<std::string> TranscribeService::asyncRun()
{
    return std::async(std::launch::async, [this] {
        return this->run();
    });
}

std::string TranscribeService::run()
{
    if (std::filesystem::exists("/usr/lib/libcuda.so")) {
        std::array<char, 128> buffer;
        std::string result;
        std::string command = this->paramStructToStringConverter();
        command = "/usr/bin/cudaOnlyWhisper " + command;
        std::cout << command << std::endl;
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    } else {
        std::array<char, 128> buffer;
        std::string result;
        std::string command = this->paramStructToStringConverter();
        command = "/usr/bin/cpuOnlyWhisper " + command;
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
}
