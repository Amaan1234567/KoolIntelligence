#!/bin/bash

# Check if Ollama is already installed
if command -v ollama &> /dev/null
then
    echo "Ollama is already installed."
    exit 0
fi

# Check if curl is installed, if not, install it
if ! command -v curl &> /dev/null
then
    echo "curl could not be found, installing curl..."
fi

# Install Ollama
echo "Installing Ollama..."

# Use sudo with -S to pass the password via stdin if required
echo "Please enter your sudo password (if required):"
echo | sudo -S curl -fsSL https://ollama.com/install.sh | sh

# Check if Ollama was installed successfully
if command -v ollama &> /dev/null
then
    echo "Ollama installed successfully!"
else
    echo "Ollama installation failed."
    exit 1
fi
