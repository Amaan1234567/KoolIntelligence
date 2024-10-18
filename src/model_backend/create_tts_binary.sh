#!/bin/sh

cd "./thirdparty/bark.cpp"

mkdir build

cd build
command cmake ..

command cmake --build . --config Release

sudo -S cp examples/main/main /usr/bin/voice_out
