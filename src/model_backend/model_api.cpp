#include "model_api.hpp"
#include "logging.hpp"
#include "transcribe_service.hpp"

// Function to execute a command and get the output
std::string ModelApi::ExecCommand(const std::string &command)
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
bool ModelApi::IsOllamaInstalled()
{
    try {
        std::string result = ExecCommand("which ollama");
        return !result.empty(); // If ollama is found, `which` returns the path
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "Ollama not found. Please install Ollama: ");
        return false;
    }
}

// Function to check if the required model is installed
bool ModelApi::IsModelInstalled(const std::string &model_name)
{
    try {
        std::string result = ExecCommand("ollama list");
        return result.find(model_name) != std::string::npos; // Check if model_name is in the output
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "inside IsModelInstalled: " + model_name + " not found ");
        return false;
    }
}

// Function to pull a model if it is not installed
void ModelApi::PullModel(const std::string &model_name)
{
    LOG_INFO("ModelApi", "inside PullModel: Pulling model: " + model_name);
    try {
        ExecCommand("ollama pull " + model_name); // Pull the model
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "inside pull_modelError pulling model: ");
    }
}

// Function to check if Ollama is running by making a simple request to localhost:11434
bool ModelApi::IsOllamaRunning()
{
    try {
        std::string result = ExecCommand("curl -s http://localhost:11434");
        return !result.empty(); // If Ollama is running, it will respond
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "Ollama not running: ");
        return false;
    }
}

// Function to start Ollama server in a separate process
void ModelApi::StartOllama()
{
    LOG_INFO("ModelApi", "Starting Ollama server...");
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
void ModelApi::StopOllama()
{
    if (ollama_pid > 0) {
        LOG_INFO("ModelApi", "Stopping Ollama server...");
        kill(ollama_pid, SIGTERM); // Stop the server process
        ollama_pid = 0;
    }
}

// Constructor with initializer list
ModelApi::ModelApi(std::string model)
    : model(model)
    , ollama_pid(0)
{
    // Initialize options with some default values
    options["temperature"] = 0.5;
    options["num_predict"] = 256;
    options["keep_alive"] = 0;
    if (!IsOllamaRunning())
        StartOllama();
    // Check if the model is installed; if not, pull it
    if (!IsModelInstalled(model)) {
        LOG_INFO("ModelApi", "Model " + model + " not installed. Pulling model...");
        PullModel(model);
    }
}

// Destructor to stop the Ollama server
ModelApi::~ModelApi()
{
    // Ensure the Ollama server stops when the object is destroyed
    StopOllama();
}

// Set additional options if needed
void ModelApi::SetOption(const std::string &key, int value)
{
    options[key] = value;
}

// Method to run inference and return the response, passing the prompt as a parameter
std::string ModelApi::GetResponse(const std::string &prompt)
{
    // Check if Ollama is installed and running
    if (!IsOllamaInstalled()) {
        LOG_ERROR("ModelApi", "Ollama is not installed on this system.");
        return "";
    }
    if (!IsOllamaRunning()) {
        LOG_INFO("ModelApi", "Ollama is installed but not running. Starting Ollama...");
        StartOllama();
    }

    // Call Ollama's generate function with the provided prompt
    ollama::response response = ollama::generate(model, prompt, options);
    return response.as_simple_string(); // Return the response as a string
}

// Async method to handle token-based streaming, passing the prompt as a parameter
void ModelApi::GetResponseAsync(const std::string &prompt)
{
    auto on_receive_response = [](const ollama::response &response) {
        std::cout << response << std::flush;
        if (response.as_json()["done"] == true) {
            std::cout << std::endl;
        }
    };

    // Check if Ollama is installed and running
    if (!IsOllamaInstalled()) {
        LOG_ERROR("ModelApi", "Ollama is not installed on this system.");
        return;
    }
    if (!IsOllamaRunning()) {
        LOG_INFO("ModelApi", "Ollama is installed but not running. Starting Ollama...");
        StartOllama();
    }

    // Call Ollama's generate function with the provided prompt and the callback
    ollama::generate(model, prompt, on_receive_response);
}

// Method to generate an image
std::string ModelApi::GenerationWithImage(const std::string &prompt, const std::string &image_path)
{
    // Check if Ollama is installed and running
    if (!IsOllamaInstalled()) {
        LOG_ERROR("ModelApi", "Ollama is not installed on this system.");
        return "";
    }
    if (!IsOllamaRunning()) {
        LOG_INFO("ModelApi", "Ollama is installed but not running. Starting Ollama...");
        StartOllama();
    }

    ollama::image image = ollama::image::from_file(image_path);

    // Call Ollama's generate function with the prompt and image path
    ollama::response response = ollama::generate(model, prompt, options, image);
    return response.as_simple_string(); // Return the response as a string
}

std::string ModelApi::TranscriptionService(std::vector<std::string> args)
{
    // pass args for transciption service in format ({"parameter_name","param_value","param_name","param_value"})
    // check transcription_service.hpp to see all the parameters that can be set, for more info look at whisper.cpp repo

    std::string whisper_cpp_path = "../../src/model_backend/thirdparty/whisper.cpp";
    std::string model_name = "ggml-small.en.bin";
    std::string needed_file = "/model/" + model_name;
    std::string model_name_in_repo = "small.en";
    std::string model_download_script_path = "/models/download-ggml-model.sh";

    // Check if the file exists using std::filesystem
    if (!std::filesystem::exists(whisper_cpp_path + needed_file)) {
        std::cout << "File not found, running installation script..." << std::endl;

        // Unix-based system (Linux, macOS, etc.)
        LOG_ERROR("ModelApi", (whisper_cpp_path + model_download_script_path + " " + model_name_in_repo).c_str());
        int ret = system((whisper_cpp_path + model_download_script_path + " " + model_name_in_repo).c_str());

        // Check the result of the script execution
        if (ret != 0) {
            LOG_ERROR("ModelApi", "Failed to run the installation script.");
            return "error";
        }
    }

    std::string transcription_service_cmd = "./stream_bin -m " + whisper_cpp_path + "/models/ggml-small.en.bin --step 1700 --length 5000 --keep 1000 ";

    TranscribeService *service = new TranscribeService();
    std::string transcript = service->run();
    LOG_ERROR("ModelApi", transcript);
    return transcript;
}
