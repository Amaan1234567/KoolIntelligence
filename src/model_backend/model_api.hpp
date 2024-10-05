#ifndef MODEL_API_HPP
#define MODEL_API_HPP

#include "thirdparty/ollama.hpp"
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

class ModelApi
{
private:
    std::string model;
    ollama::options options;
    pid_t ollama_pid; // For tracking the Ollama server process

    // Function to execute a command and get the output
    std::string ExecCommand(const std::string &command);

    // Function to check if Ollama is installed
    bool IsOllamaInstalled();

    // Function to check if the required model is installed
    bool IsModelInstalled(const std::string &model_name);

    // Function to pull a model if it is not installed
    void PullModel(const std::string &model_name);

    // Function to check if Ollama is running by making a simple request to localhost:11434
    bool IsOllamaRunning();

    // Function to start Ollama server in a separate process
    void StartOllama();

    // Function to stop Ollama server
    void StopOllama();

public:
    // Constructor with initializer list
    ModelApi(std::string model = "moondream:1.8b-v2-q5_K_M");

    // Destructor to stop the Ollama server
    ~ModelApi();

    // Set additional options if needed
    void SetOption(const std::string &key, int value);

    // Method to run inference and return the response, passing the prompt as a parameter
    std::string GetResponse(const std::string &prompt);

    // Async method to handle token-based streaming, passing the prompt as a parameter
    void GetResponseAsync(const std::string &prompt);

    // Method to generate an image
    std::string GenerationWithImage(const std::string &prompt, const std::string &image_path);

    std::string TranscriptionService(std::vector<std::string> argv = std::vector<std::string>());
};

#endif // MODEL_API_HPP
