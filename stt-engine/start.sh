#!/bin/bash

# Start service
cd ..
./build/bin/stt-engine -m ./models/ggml-large-v2.bin -t 8 -l de
