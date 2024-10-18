#include <cstdlib>
#include <filesystem>

class TTSService
{
public:
    TTSService();

    void speak(std::string text);
};