#!/bin/bash
export NVCC_PREPEND_FLAGS='-ccbin /home/linuxbrew/.linuxbrew/bin/g++-12'
export PATH="/usr/local/cuda-12.6/bin:$PATH"
set -e  # Exit immediately if a command exits with a non-zero status.

# if [ -d "/opt/cuda/bin" ]; then
#     export PATH="/opt/cuda/bin:$PATH"
# fi

# Function to check if NVIDIA GPU is present
check_nvidia_gpu() {
    if lspci | grep -i nvidia > /dev/null; then
        return 0  # NVIDIA GPU found
    else
        return 1  # NVIDIA GPU not found
    fi
}

git clone https://github.com/ggerganov/whisper.cpp.git
cd whisper.cpp/

echo "Attempting CPU build..."
make clean
make stream -j8 || echo "CPU build failed"
if [ -f stream ]; then
    sudo cp ./stream /usr/bin/cpuOnlyWhisper
    echo "CPU version of 'stream' copied to /usr/bin/cpuOnlyWhisper"
    rm stream
else
    echo "CPU build didn't produce 'stream' binary"
fi

rm -rf build

if check_nvidia_gpu; then
echo "NVIDIA GPU detected. Attempting CUDA build..."
make clean
    GGML_CUDA=1 make stream -j8 || echo "CUDA build failed"
if [ -f stream ]; then
    sudo cp ./stream /usr/bin/cudaOnlyWhisper
    echo "CUDA version of 'stream' copied to /usr/bin/cudaOnlyWhisper"
    rm stream
else
    echo "CUDA build didn't produce 'stream' binary"
fi
else
    echo "No NVIDIA GPU detected. Skipping CUDA build."
fi



cd ../
rm -rf whisper.cpp