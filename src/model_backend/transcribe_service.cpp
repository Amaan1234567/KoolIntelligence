#include "transcribe_service.hpp"
#include <array>

TranscribeService::TranscribeService()
{   
    const std::string whisperCppPath = "~";
    const std::string modelName = "ggml-small.en.bin";
    const std::string neededFile = "/" + modelName;
    const std::string modelNameInRepo = "small.en";
    const std::string modelDownloadScriptPath = std::string(getenv("HOME")) + "/koolintelligence/" + "scripts/download-ggml-model.sh";

    // Check if the file exists using std::filesystem
    if (!std::filesystem::exists(std::string(getenv("HOME")) + "/koolintelligence/" + "models/" + neededFile)) {
        LOG_INFO("ModelApi", "File not found, running installation script...");

        int ret = system((modelDownloadScriptPath + " " + modelNameInRepo).c_str());

        // Check the result of the script execution
        if (ret != 0) {
            LOG_ERROR("ModelApi", "Failed to run the installation script.");
        }
    }
    if (std::filesystem::exists("/usr/lib/libcuda.so"))
        this->params.no_gpu = false;
    else
        this->params.no_gpu = true;
}

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

//note: The string and bool have to be manually deleted after use
std::pair<std::string*, bool*>TranscribeService::startService()
{   
    std::string* result = new std::string();
    bool* isRunning = new bool(true);
    std::thread(&TranscribeService::asyncService, this, result, isRunning).detach();
    return std::make_pair(result, isRunning);
}

void TranscribeService::asyncService(std::string* result, bool* isRunning){
    std::array<char, 128> buffer;
    std::string command = this->paramStructToStringConverter();
    command = (std::filesystem::exists("/usr/lib/libcuda.so") ? "/usr/bin/cudaOnlyWhisper " : "/usr/bin/cpuOnlyWhisper ") + command;
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    LOG_INFO("TranscribeService", "Service started");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr && *isRunning) {
        LOG_INFO("TranscribeService", buffer.data());
        *result += buffer.data();
        if(!*isRunning){
            LOG_INFO("TranscribeService", "Service stopped");
            pclose(pipe);
            return;
        }
    }
    *isRunning = false;
}
