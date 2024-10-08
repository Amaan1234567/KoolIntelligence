#pragma once

#include "thirdparty/ollama.hpp"
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

class ModelApi
{
private:
    std::string model;
    std::string transcriptionModel;
    std::string systemPrompt;
    ollama::options options;
    std::string mainFolderPath;
    pid_t ollamaPid; // For tracking the Ollama server process

    ollama::messages curSessionHistory;

    // Function to execute a command and get the output
    std::string execCommand(const std::string &command);

    // Function to check if Ollama is installed
    bool isOllamaInstalled();

    // Function to check if the required model is installed
    bool isModelInstalled(const std::string &modelName);

    // Function to pull a model if it is not installed
    void pullModel(const std::string &modelName);

    // Function to check if Ollama is running by making a simple request to localhost:11434
    bool isOllamaRunning();

    // Function to start Ollama server in a separate process

    bool doesFolderExist();

    void folderCheck();

    void mainFolderInit();

public:
    // Constructor with initializer list
    ModelApi(std::string model = "moondream:1.8b-v2-q5_K_M",
             std::string transcriptionModel = "ggml-small.en.bin",
             std::string systemPrompt =
                 "You are a helpful AI assistant, who is good at solving the user's queries be it related to coding, general knowledge or anything else a "
                 "human assistant is capable of. you also have excellent image recognition and analysis skills due to have multi model capabilites, now "
                 "following will be the user's queries, analyse them carefully and repsond back to the user properly. ");

    // Destructor to stop the Ollama server
    ~ModelApi();

    void startOllama();

    // Function to stop Ollama server
    void stopOllama();

    // Set additional options if needed
    void setOption(const std::string &key, int value);

    // Method to run inference and return the response, passing the prompt as a parameter
    std::string getResponse(const std::string &prompt);

    // Async method to handle token-based streaming, passing the prompt as a parameter
    void getResponseAsync(const std::string &prompt);

    // Method to generate an image
    std::string generationWithImage(const std::string &prompt, const std::string &imagePath);

    std::string transcriptionService(std::vector<std::string> argv = std::vector<std::string>());
};
