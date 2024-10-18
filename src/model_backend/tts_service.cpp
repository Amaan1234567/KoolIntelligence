#include "tts_service.hpp"

void TTSService::speak(std::string text)
{
    system(("voice_out -p " + std::string("\"" + text + "\"") + " -m " + std::string(getenv("HOME")) + "/koolintelligence/models/ggml_weights_q4.bin -o "
            + std::string(getenv("HOME")) + "/koolintelligence/voice_out.wav" + " -t 8 -s 60")
               .c_str());
    system(("aplay " + std::string(getenv("HOME")) + "/koolintelligence/voice_out.wav").c_str());
    system(("rm " + std::string(getenv("HOME")) + "/koolintelligence/voice_out.wav").c_str());
}