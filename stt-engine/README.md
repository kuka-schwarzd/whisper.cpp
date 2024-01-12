# stt-engine

This is a basic Speech To Text Engine that accepts voice commands from the microphone and sends them to a grpc Server.

```bash
# Run with default arguments and small model
./build/bin/stt-engine -m ./models/ggml-large-v2.bin -t 8

# On Raspberry Pi, use tiny or base models + "-ac 768" for better performance
./build/bin/stt-engine -m ./models/ggml-large-v2.bin -ac 768 -t 3 -c 0
```

## Guided mode

"Guided mode" allows you to specify a list of commands (i.e. strings) and the transcription will be guided to classify your command into one from the list. This can be useful in situations where a device is listening only for a small subset of commands.

```bash
# Run in guided mode, the list of allowed commands is in commands.txt
./build/bin/stt-engine -m ./models/ggml-large-v2.bin -cmd ./examples/rasa/commands.txt

# On Raspberry Pi, in guided mode you can use "-ac 128" for extra performance
./build/bin/stt-engine -m ./models/ggml-large-v2.bin -cmd ./examples/rasa/commands.txt -ac 128 -t 3 -c 0
```

## Building

The `stt-engine` tool depends on SDL2 library to capture audio from the microphone. You can build it like this:

```bash
# Install SDL2 on Linux
sudo apt-get install libsdl2-dev

# Install SDL2 on Mac OS
brew install sdl2

# run in root dor
cmake -B build 
cmake --build build -j
```

## Generate C++ grpc stubs manually

```bash
protoc -I="." --cpp_out="." --grpc_out="." --plugin=protoc-gen-grpc=/opt/homebrew/bin/grpc_cpp_plugin ./conv_agent_input_service.proto
```
