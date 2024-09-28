#ifndef MODEL_API_HPP
#define MODEL_API_HPP


#include <string>
#include <cstdlib>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "ollama.hpp"


class model_api {
private:
    std::string model;
    ollama::options options;
    pid_t ollama_pid; // For tracking the Ollama server process


    // Function to execute a command and get the output
    std::string exec_command(const std::string& command);

    // Function to check if Ollama is installed
    bool is_ollama_installed();

    // Function to check if the required model is installed
    bool is_model_installed(const std::string& model_name);

    // Function to pull a model if it is not installed
    void pull_model(const std::string& model_name);

    // Function to check if Ollama is running by making a simple request to localhost:11434
    bool is_ollama_running();

    // Function to start Ollama server in a separate process
    void start_ollama();

    // Function to stop Ollama server
    void stop_ollama();

public:
    // Constructor with initializer list
    model_api(std::string model = "moondream:1.8b-v2-q5_K_M");

    // Destructor to stop the Ollama server
    ~model_api();

    // Set additional options if needed
    void set_option(const std::string& key, int value);

    // Method to run inference and return the response, passing the prompt as a parameter
    std::string get_response(const std::string& prompt);

    // Async method to handle token-based streaming, passing the prompt as a parameter
    void get_response_async(const std::string& prompt);

    // Method to generate an image
    std::string generation_with_image(const std::string& prompt, const std::string& image_path);
};


#endif // MODEL_API_HPP
