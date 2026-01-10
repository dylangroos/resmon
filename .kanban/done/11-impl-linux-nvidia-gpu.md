# Implement Linux NVIDIA GPU

**Priority:** HIGH
**Phase:** 3 - Linux Backend

## Description
Use NVML (NVIDIA Management Library) for NVIDIA GPU stats.

## Approach
```cpp
#include <nvml.h>

nvmlInit();
nvmlDeviceGetCount(&deviceCount);
nvmlDeviceGetHandleByIndex(i, &device);
nvmlDeviceGetUtilizationRates(device, &utilization);
nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp);
nvmlDeviceGetMemoryInfo(device, &memory);
```

## Tasks
- [ ] Add NVML to CMake (find or bundle header)
- [ ] Runtime load libnvidia-ml.so (dlopen)
- [ ] Query GPU utilization, temp, VRAM
- [ ] Handle multiple GPUs
- [ ] Graceful fallback if no NVIDIA driver

## Acceptance Criteria
- Matches nvidia-smi output
- Works without NVML installed (returns empty)
- Supports multi-GPU
