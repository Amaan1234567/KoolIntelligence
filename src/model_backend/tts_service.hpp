#include <cstdlib>
#include <filesystem>

class TTSService
{
public:
    TTSService()
    {
        if (!std::filesystem::exists(std::string(getenv("HOME")) + "/koolintelligence/models/ggml_weights_q4.bin")) {
            system("chmod +x src/model_backend/download_voice_out_model.sh && src/model_backend/download_voice_out_model.sh");
        }
    }

    void speak(std::string text);
};