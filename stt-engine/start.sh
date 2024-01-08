#!/bin/bash

# compile
cd ..
cmake -B build 
cmake --build build -j

# Start service
./build/bin/stt-engine -m ./models/ggml-large-v2.bin -t 8
