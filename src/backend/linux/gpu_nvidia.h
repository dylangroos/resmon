#ifndef RESMON_BACKEND_LINUX_GPU_NVIDIA_H
#define RESMON_BACKEND_LINUX_GPU_NVIDIA_H

#include "../../core/metrics.h"

#include <string>
#include <vector>

namespace resmon {
namespace platform {

// NVML types defined locally to avoid requiring NVML headers at compile time
typedef void* nvmlDevice_t;
typedef int nvmlReturn_t;

#define NVML_SUCCESS 0
#define NVML_TEMPERATURE_GPU 0

struct nvmlUtilization_t {
    unsigned int gpu;
    unsigned int memory;
};

struct nvmlMemory_t {
    unsigned long long total;
    unsigned long long free;
    unsigned long long used;
};

// Function pointer types for NVML functions
typedef nvmlReturn_t (*nvmlInit_v2_t)(void);
typedef nvmlReturn_t (*nvmlShutdown_t)(void);
typedef nvmlReturn_t (*nvmlDeviceGetCount_v2_t)(unsigned int* deviceCount);
typedef nvmlReturn_t (*nvmlDeviceGetHandleByIndex_v2_t)(unsigned int index, nvmlDevice_t* device);
typedef nvmlReturn_t (*nvmlDeviceGetName_t)(nvmlDevice_t device, char* name, unsigned int length);
typedef nvmlReturn_t (*nvmlDeviceGetUtilizationRates_t)(nvmlDevice_t device, nvmlUtilization_t* utilization);
typedef nvmlReturn_t (*nvmlDeviceGetTemperature_t)(nvmlDevice_t device, int sensorType, unsigned int* temp);
typedef nvmlReturn_t (*nvmlDeviceGetMemoryInfo_t)(nvmlDevice_t device, nvmlMemory_t* memory);

class NvidiaGpuCollector {
public:
    NvidiaGpuCollector();
    ~NvidiaGpuCollector();

    // Non-copyable
    NvidiaGpuCollector(const NvidiaGpuCollector&) = delete;
    NvidiaGpuCollector& operator=(const NvidiaGpuCollector&) = delete;

    // Collect metrics from all NVIDIA GPUs
    // Returns empty vector if NVML is not available
    std::vector<GpuMetrics> collect();

    // Check if NVML is available
    bool isAvailable() const { return nvml_available_; }

private:
    bool loadNvml();
    void unloadNvml();

    void* nvml_handle_;
    bool nvml_available_;
    bool nvml_initialized_;

    // Function pointers
    nvmlInit_v2_t nvmlInit_v2_;
    nvmlShutdown_t nvmlShutdown_;
    nvmlDeviceGetCount_v2_t nvmlDeviceGetCount_v2_;
    nvmlDeviceGetHandleByIndex_v2_t nvmlDeviceGetHandleByIndex_v2_;
    nvmlDeviceGetName_t nvmlDeviceGetName_;
    nvmlDeviceGetUtilizationRates_t nvmlDeviceGetUtilizationRates_;
    nvmlDeviceGetTemperature_t nvmlDeviceGetTemperature_;
    nvmlDeviceGetMemoryInfo_t nvmlDeviceGetMemoryInfo_;
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_LINUX_GPU_NVIDIA_H
