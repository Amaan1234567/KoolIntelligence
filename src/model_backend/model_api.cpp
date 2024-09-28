#include <iostream>
#include <string>
#include <cstdlib>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "./ollama.hpp"

class ModelAPI
{
private:
    std::string model;
    ollama::options options;
    pid_t ollama_pid; // For tracking the Ollama server process

    // Function to execute a command and get the output
    std::string exec_command(const std::string& command)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    // Function to check if Ollama is installed
    bool is_ollama_installed()
    {
        try {
            std::string result = exec_command("which ollama");
            return !result.empty(); // If ollama is found, `which` returns the path
        } catch (const std::runtime_error& e) {
            std::cerr << "Ollama not found. Please install Ollama: " << e.what() << std::endl;
            return false;
        }
    }

    // Function to check if the required model is installed
    bool is_model_installed(const std::string& model_name)
    {
        try {
            std::string result = exec_command("ollama list");
            return result.find(model_name) != std::string::npos; // Check if model_name is in the output
        } catch (const std::runtime_error& e) {
            std::cerr << model_name << "not installed" << e.what() << std::endl;
            return false;
        }
    }

    // Function to pull a model if it is not installed
    void pull_model(const std::string& model_name)
    {
        std::cout << "Pulling model: " << model_name << std::endl;
        try {
            exec_command("ollama pull " + model_name); // Pull the model
        } catch (const std::runtime_error& e) {
            std::cerr << "Error pulling model: " << e.what() << std::endl;
        }
    }

    // Function to check if Ollama is running by making a simple request to localhost:11434
    bool is_ollama_running()
    {
        try {
            std::string result = exec_command("curl -s http://localhost:11434");
            return !result.empty(); // If Ollama is running, it will respond
        } catch (const std::runtime_error& e) {
            std::cerr << "Error checking Ollama status: " << e.what() << std::endl;
            return false;
        }
    }

    // Function to start Ollama server in a separate process
    void start_ollama()
    {
        std::cout << "Starting Ollama server in a separate process..." << std::endl;
        ollama_pid = fork(); // Fork a new process
        if (ollama_pid == 0) {
            // Child process: run the Ollama server
            execlp("ollama", "ollama", "serve", nullptr);
            exit(1); // Exit if execlp fails
        }
        // Give the server a moment to start up
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // Function to stop Ollama server
    void stop_ollama()
    {
        if (ollama_pid > 0) {
            std::cout << "Stopping Ollama server..." << std::endl;
            kill(ollama_pid, SIGTERM); // Stop the server process
            ollama_pid = 0;
        }
    }

public:
    // Constructor with initializer list
    ModelAPI(std::string model = "moondream:1.8b-v2-q5_K_M")
        : model(model)
        , ollama_pid(0)
    {
        // Initialize options with some default values
        options["temperature"] = 1.2;
        options["num_predict"] = 256;
        options["keep_alive"] = 0;
        if (!is_ollama_running())
            start_ollama();
        // Check if the model is installed; if not, pull it
        if (!is_model_installed(model)) {
            std::cout << "Model " << model << " not installed. Pulling model..." << std::endl;
            pull_model(model);
        }
    }

    ~ModelAPI()
    {
        // Ensure the Ollama server stops when the object is destroyed
        stop_ollama();
    }

    // Set additional options if needed
    void set_option(const std::string& key, int value)
    {
        options[key] = value;
    }

    // Method to run inference and return the response, now passing the prompt as a parameter
    std::string get_response(const std::string& prompt)
    {
        // Check if Ollama is installed and running
        if (!is_ollama_installed()) {
            std::cerr << "Ollama is not installed on this system." << std::endl;
            return "";
        }
        if (!is_ollama_running()) {
            std::cout << "Ollama is installed but not running. Starting Ollama..." << std::endl;
            start_ollama();
        }

        // Call Ollama's generate function with the provided prompt
        ollama::response response = ollama::generate(model, prompt, options);
        return response.as_simple_string(); // Return the response as a string
    }

    // Async method to handle token-based streaming, now passing the prompt as a parameter
    void get_response_async(const std::string& prompt)
    {
        auto on_receive_response = [](const ollama::response& response) {
            std::cout << response << std::flush;
            if (response.as_json()["done"] == true) {
                std::cout << std::endl;
            }
        };

        // Check if Ollama is installed and running
        if (!is_ollama_installed()) {
            std::cerr << "Ollama is not installed on this system." << std::endl;
            return;
        }
        if (!is_ollama_running()) {
            std::cout << "Ollama is installed but not running. Starting Ollama..." << std::endl;
            start_ollama();
        }

        // Call Ollama's generate function with the provided prompt and the callback
        ollama::generate(model, prompt, on_receive_response);
    }

    // Method to generate an image
    std::string generation_with_image(const std::string& prompt, const std::string& image_path)
    {
        // Check if Ollama is installed and running
        if (!is_ollama_installed()) {
            std::cerr << "Ollama is not installed on this system." << std::endl;
            return "";
        }
        if (!is_ollama_running()) {
            std::cout << "Ollama is installed but not running. Starting Ollama..." << std::endl;
            start_ollama();
        }

        ollama::image image = ollama::image::from_file(image_path);

        // Call Ollama's generate function with the prompt and image path
        ollama::response response = ollama::generate(model, prompt, options, image);
        return response.as_simple_string(); // Return the response as a string
    }
};

int main()
{
    // Create an instance of ModelAPI with a specific model
    ModelAPI model_api("moondream:1.8b-v2-q5_K_M");

    // Set additional options if needed
    model_api.set_option("seed", 42); // Example of setting an option

    // Define the prompt and image path
    std::string prompt = "describe this image";
    std::string image_path = "/home/amaan/Pictures/backgrounds/raiden epic background.png";

    // Get the synchronous response from the model and print it
    std::string response = model_api.generation_with_image(prompt, image_path);
    std::cout << "Response: " << response << std::endl;

    return 0;
}
