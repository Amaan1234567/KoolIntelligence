#include "model_api.hpp"
#include "logging.hpp"
#include "transcribe_service.hpp"

// Function to execute a command and get the output
std::string ModelApi::execCommand(const std::string &command)
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
bool ModelApi::isOllamaInstalled()
{
    try {
        std::string result = execCommand("which ollama");
        return !result.empty(); // If ollama is found, `which` returns the path
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "Ollama not found. Please install Ollama: ");
        return false;
    }
}

bool ModelApi::doesFolderExist()
{
    std::filesystem::directory_entry path{std::string(getenv("HOME")) + "/koolintelligence"};
    return path.exists();
}

void ModelApi::mainFolderInit()
{
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/koolintelligence/models");
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/koolintelligence/sessions");
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/koolintelligence/screenshots");
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/koolintelligence/terminal_settings");
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/koolintelligence/scripts");
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/koolintelligence/logs");

    std::ofstream logs(std::string(getenv("HOME")) + "/koolintelligence/logs/logs.log");
    logs.close();

    std::filesystem::copy(std::string(PROJECT_DIR) + "/src/model_backend/download-ggml-model.sh",
                          std::string(getenv("HOME")) + "/koolintelligence/scripts/download-ggml-model.sh");
}

// main function which checks if the folder for our app is present in user's HOME directory
void ModelApi::folderCheck()
{
    if (ModelApi::doesFolderExist()) {
        LOG_INFO("ModelApi", "inside folderCheck: main folder found skipping init");
    } else {
        ModelApi::mainFolderInit();
    }
}

// Function to check if the required model is installed
bool ModelApi::isModelInstalled(const std::string &modelName)
{
    try {
        std::string result = execCommand("ollama list");
        return result.find(modelName) != std::string::npos; // Check if modelName is in the output
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "inside isModelInstalled: " + modelName + " not found ");
        return false;
    }
}

// Function to pull a model if it is not installed
void ModelApi::pullModel(const std::string &modelName)
{
    LOG_INFO("ModelApi", "inside pullModel: Pulling model: " + modelName);
    try {
        execCommand("ollama pull " + modelName); // Pull the model
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "inside pullModel Error pulling model: ");
    }
}

// Function to check if Ollama is running by making a simple request to localhost:11434
bool ModelApi::isOllamaRunning()
{
    try {
        std::string result = execCommand("curl -s http://localhost:11434");
        return !result.empty(); // If Ollama is running, it will respond
    } catch (const std::runtime_error &err) {
        LOG_ERROR("ModelApi", "Ollama not running: ");
        return false;
    }
}

// Function to start Ollama server in a separate process
void ModelApi::startOllama()
{
    LOG_INFO("ModelApi", "Starting Ollama server...");
    this->ollamaPid = fork(); // Fork a new process

    if (this->ollamaPid == 0) {
        // Child process: run the Ollama server
        execlp("ollama", "ollama", "serve", nullptr);
        exit(1); // Exit if execlp fails
    }
    // Give the server a moment to start up
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

// Function to stop Ollama server
void ModelApi::stopOllama()
{
    if (this->ollamaPid > 0) {
        LOG_INFO("ModelApi", "Stopping Ollama server...");
        LOG_INFO("ModelApi", "ollama pid: " + std::to_string(this->ollamaPid));

        kill(this->ollamaPid, 9); // Stop the server process
        this->ollamaPid = 0;
    }

    // to unload the model from VRAM
    ModelApi::execCommand("curl http://localhost:11434/api/generate -d '{\"model\": \"" + this->model + "\", \"keep_alive\": 0}\'");
}

// Constructor with initializer list
ModelApi::ModelApi(std::string model, std::string transcriptionModel, std::string systemPrompt)
    : model(model)
    , transcriptionModel(transcriptionModel)
    , systemPrompt(systemPrompt)
    , ollamaPid(0)

{
    ModelApi::folderCheck();
    this->mainFolderPath = std::string(getenv("HOME")) + "/koolintelligence/";
    // Initialize options with some default values
    this->options["temperature"] = 1;
    this->options["num_predict"] = 1024;
    this->options["keep_alive"] = -1;

    if (!isOllamaRunning())
        this->startOllama();
    // Check if the model is installed; if not, pull it
    if (!isModelInstalled(model)) {
        LOG_INFO("ModelApi", "Model " + model + " not installed. Pulling model...");
        pullModel(model);
    }

    ollama::message message1("system", this->systemPrompt);
}

// Destructor to stop the Ollama server
ModelApi::~ModelApi()
{
    // Ensure the Ollama server stops when the object is destroyed
    this->stopOllama();
}

// Set additional this->options if needed
void ModelApi::setOption(const std::string &key, int value)
{
    this->options[key] = value;
}

// Method to run inference and return the response, passing the prompt as a parameter
std::string ModelApi::getResponse(const std::string &prompt)
{
    // Check if Ollama is installed and running
    if (!isOllamaInstalled()) {
        LOG_ERROR("ModelApi", "Ollama is not installed on this system.");
        return "";
    }
    if (!isOllamaRunning()) {
        LOG_INFO("ModelApi", "Ollama is installed but not running. Starting Ollama...");
        this->startOllama();
    }

    ollama::message message("user", prompt);

    this->curSessionHistory.push_back(message);

    // Call Ollama's generate function with the provided prompt
    ollama::response response = ollama::chat(model, this->curSessionHistory, this->options);
    ollama::message model_response("assistant", response.as_simple_string());
    this->curSessionHistory.push_back(model_response);

    return response.as_simple_string(); // Return the response as a string
}

// Async method to handle token-based streaming, passing the prompt as a parameter
void ModelApi::getResponseAsync(const std::string &prompt)
{
    auto on_receive_response = [](const ollama::response &response) {
        std::cout << response << std::flush;
        if (response.as_json()["done"] == true) {
            std::cout << std::endl;
        }
    };

    // Check if Ollama is installed and running
    if (!isOllamaInstalled()) {
        LOG_ERROR("ModelApi", "Ollama is not installed on this system.");
        return;
    }
    if (!isOllamaRunning()) {
        LOG_INFO("ModelApi", "Ollama is installed but not running. Starting Ollama...");
        this->startOllama();
    }

    // Call Ollama's generate function with the provided prompt and the callback
    ollama::generate(model, prompt, on_receive_response, options = this->options);
}

// Method to generate an image
std::string ModelApi::generationWithImage(const std::string &prompt, const std::string &imagePath)
{
    // Check if Ollama is installed and running
    if (!isOllamaInstalled()) {
        LOG_ERROR("ModelApi", "Ollama is not installed on this system.");
        return "";
    }
    if (!isOllamaRunning()) {
        LOG_INFO("ModelApi", "Ollama is installed but not running. Starting Ollama...");
        this->startOllama();
    }

    ollama::image image = ollama::image::from_file(imagePath);

    // Call Ollama's generate function with the prompt and image path
    ollama::response response = ollama::generate(model, prompt, this->options, image);
    return response.as_simple_string(); // Return the response as a string
}

std::string ModelApi::transcriptionService(std::vector<std::string> args)
{
    // pass args for transciption service in format ({"parameter_name","param_value","param_name","param_value"})
    // check transcription_service.hpp to see all the parameters that can be set, for more info look at whisper.cpp repo

    std::string whisperCppPath = "~";
    std::string modelName = "ggml-small.en.bin";
    std::string neededFile = "/" + modelName;
    std::string modelNameInRepo = "small.en";
    std::string modelDownloadScriptPath = this->mainFolderPath + "scripts/download-ggml-model.sh";

    // Check if the file exists using std::filesystem
    if (!std::filesystem::exists(this->mainFolderPath + "models/" + neededFile)) {
        LOG_INFO("ModelApi", "File not found, running installation script...");

        int ret = system((modelDownloadScriptPath + " " + modelNameInRepo).c_str());

        // Check the result of the script execution
        if (ret != 0) {
            LOG_ERROR("ModelApi", "Failed to run the installation script.");
            return "error";
        }
    }

    TranscribeService *service = new TranscribeService();
    std::string transcript = service->run();
    LOG_INFO("ModelApi", transcript);
    return transcript;
}
