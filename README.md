# resmon

A lightweight system resource monitor with a minimal GUI.

Displays real-time CPU, memory, and GPU usage with visual alerts when thresholds are exceeded.

## Features

- CPU usage and temperature monitoring
- Memory usage tracking
- GPU monitoring (NVIDIA, AMD, Intel)
- VRAM usage display
- Visual alerts for high resource usage
- Minimal, dark-themed interface

## Building

Requires CMake 3.16+ and a C++17 compiler.

```bash
mkdir build && cd build
cmake ..
make
./resmon
```

Dependencies (GLFW, Dear ImGui) are fetched automatically via CMake.

## Platform Support

- Linux (full support)
- macOS (full support)
- Windows (planned)

## License

MIT
