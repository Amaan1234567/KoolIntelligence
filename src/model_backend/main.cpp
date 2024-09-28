#include "model_api.hpp"
#include <iostream>

int main()
{
    // Create an instance of ModelAPI with a specific model
    model_api api;

    // Set additional options if needed
    api.set_option("seed", 42); // Example of setting an option

    // Define the prompt and image path
    std::string prompt = "describe this image";
    std::string image_path = "/home/amaan/Pictures/backgrounds/raiden epic background.png";

    // Get the synchronous response from the model and print it
    std::string response = api.generation_with_image(prompt, image_path);
    std::cout << "Response: " << response << std::endl;

    return 0;
}