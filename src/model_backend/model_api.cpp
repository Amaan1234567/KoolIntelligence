#include "model_api.hpp"
#include "ollama.hpp"
#include "logging.hpp"

// Function to execute a command and get the output
std::string model_api::exec_command(const std::string& command)
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
bool model_api::is_ollama_installed()
{
    try {
        std::string result = exec_command("which ollama");
        return !result.empty(); // If ollama is found, `which` returns the path
    } catch (const std::runtime_error& e) {
        LOG_ERROR("model_api", "Ollama not found. Please install Ollama: " + static_cast<std::string>(e.what()));
        return false;
    }
}

// Function to check if the required model is installed
bool model_api::is_model_installed(const std::string& model_name)
{
    try {
        std::string result = exec_command("ollama list");
        return result.find(model_name) != std::string::npos; // Check if model_name is in the output
    } catch (const std::runtime_error& e) {
        LOG_ERROR("model_api", model_name+" not found " + static_cast<std::string>(e.what()));
        return false;
    }
}

// Function to pull a model if it is not installed
void model_api::pull_model(const std::string& model_name)
{
    LOG_INFO("model_api", "Pulling model: " + model_name);
    try {
        exec_command("ollama pull " + model_name); // Pull the model
    } catch (const std::runtime_error& e) {
        LOG_ERROR("model_api", "Error pulling model: " + static_cast<std::string>(e.what()));
    }
}

// Function to check if Ollama is running by making a simple request to localhost:11434
bool model_api::is_ollama_running()
{
    try {
        std::string result = exec_command("curl -s http://localhost:11434");
        return !result.empty(); // If Ollama is running, it will respond
    } catch (const std::runtime_error& e) {
        LOG_ERROR("model_api", "Ollama not running: " + static_cast<std::string>(e.what()));
        return false;
    }
}

// Function to start Ollama server in a separate process
void model_api::start_ollama()
{
    LOG_INFO("model_api", "Starting Ollama server...");
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
void model_api::stop_ollama()
{
    if (ollama_pid > 0) {
        LOG_INFO("model_api", "Stopping Ollama server...");
        kill(ollama_pid, SIGTERM); // Stop the server process
        ollama_pid = 0;
    }
}

// Constructor with initializer list
model_api::model_api(std::string model )
    : model(model), ollama_pid(0)
{
    // Initialize options with some default values
    options["temperature"] = 0.5;
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

// Destructor to stop the Ollama server
model_api::~model_api()
{
    // Ensure the Ollama server stops when the object is destroyed
    stop_ollama();
}

// Set additional options if needed
void model_api::set_option(const std::string& key, int value)
{
    options[key] = value;
}

// Method to run inference and return the response, passing the prompt as a parameter
std::string model_api::get_response(const std::string& prompt)
{
    // Check if Ollama is installed and running
    if (!is_ollama_installed()) {
        LOG_ERROR("model_api", "Ollama is not installed on this system.");
        return "";
    }
    if (!is_ollama_running()) {
        LOG_INFO("model_api", "Ollama is installed but not running. Starting Ollama...");
        start_ollama();
    }

    // Call Ollama's generate function with the provided prompt
    ollama::response response = ollama::generate(model, prompt, options);
    return response.as_simple_string(); // Return the response as a string
}

// Async method to handle token-based streaming, passing the prompt as a parameter
void model_api::get_response_async(const std::string& prompt)
{
    auto on_receive_response = [](const ollama::response& response) {
        std::cout << response << std::flush;
        if (response.as_json()["done"] == true) {
            std::cout << std::endl;
        }
    };

    // Check if Ollama is installed and running
    if (!is_ollama_installed()) {
        LOG_ERROR("model_api", "Ollama is not installed on this system.");
        return ;
    }
    if (!is_ollama_running()) {
        LOG_INFO("model_api", "Ollama is installed but not running. Starting Ollama...");
        start_ollama();
    }

    // Call Ollama's generate function with the provided prompt and the callback
    ollama::generate(model, prompt, on_receive_response);
}

// Method to generate an image
std::string model_api::generation_with_image(const std::string& prompt, const std::string& image_path)
{
    // Check if Ollama is installed and running
    if (!is_ollama_installed()) {
        LOG_ERROR("model_api", "Ollama is not installed on this system.");
        return "";
    }
    if (!is_ollama_running()) {
        LOG_INFO("model_api", "Ollama is installed but not running. Starting Ollama...");
        start_ollama();
    }

    ollama::image image = ollama::image::from_file(image_path);

    // Call Ollama's generate function with the prompt and image path
    ollama::response response = ollama::generate(model, prompt, options, image);
    return response.as_simple_string(); // Return the response as a string
}
