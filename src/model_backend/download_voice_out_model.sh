#!/bin/sh

# Set variables
REPO_ID="abdulamaan/voice_out_koolintelligence"
FILENAME="ggml_weights_q4.bin"  # Assuming this is the weights filename, adjust if needed
OUTPUT_DIR="${HOME}/koolintelligence/models"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Construct the download URL
DOWNLOAD_URL="https://huggingface.co/${REPO_ID}/resolve/main/${FILENAME}"

# Download the file
echo "Downloading ${FILENAME} from ${REPO_ID}..."
if command -v wget >/dev/null 2>&1; then
    wget --no-config --quiet --show-progress -O "${OUTPUT_DIR}/${FILENAME}" "${DOWNLOAD_URL}"
elif command -v curl >/dev/null 2>&1; then
    curl -L --output "${OUTPUT_DIR}/${FILENAME}" "${DOWNLOAD_URL}"
else
    echo "Error: Neither wget nor curl is available. Please install one of them and try again."
    exit 1
fi

# Check if the download was successful
if [ $? -eq 0 ]; then
    echo "Download completed successfully. File saved in ${OUTPUT_DIR}/${FILENAME}"
else
    echo "Download failed. Please check your internet connection and the repository details."
    exit 1
fi