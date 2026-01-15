# resmon

A lightweight system resource monitor with a minimal GUI.

Displays real-time CPU, memory, and GPU usage with visual alerts when thresholds are exceeded.

## Installation

### Prebuilt Binaries

Download the latest release for your platform from the [Releases page](../../releases):

- **Linux (x86_64)**: `resmon-linux-x86_64.tar.gz`
- **macOS (Apple Silicon)**: `resmon-macos-arm64.tar.gz`
- **macOS (Intel)**: `resmon-macos-x86_64.tar.gz`

```bash
# Extract and run
tar -xzf resmon-*.tar.gz
./resmon
```

### Building from Source

See [Building](#building) below.

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
