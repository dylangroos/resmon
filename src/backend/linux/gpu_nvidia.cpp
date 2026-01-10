#include "gpu_nvidia.h"

#include <dlfcn.h>
#include <cstring>

namespace resmon {
namespace platform {

NvidiaGpuCollector::NvidiaGpuCollector()
    : nvml_handle_(nullptr)
    , nvml_available_(false)
    , nvml_initialized_(false)
    , nvmlInit_v2_(nullptr)
    , nvmlShutdown_(nullptr)
    , nvmlDeviceGetCount_v2_(nullptr)
    , nvmlDeviceGetHandleByIndex_v2_(nullptr)
    , nvmlDeviceGetName_(nullptr)
    , nvmlDeviceGetUtilizationRates_(nullptr)
    , nvmlDeviceGetTemperature_(nullptr)
    , nvmlDeviceGetMemoryInfo_(nullptr)
{
    nvml_available_ = loadNvml();
}

NvidiaGpuCollector::~NvidiaGpuCollector() {
    unloadNvml();
}

bool NvidiaGpuCollector::loadNvml() {
    // Try to load NVML library
    // Try versioned library first, then unversioned
    nvml_handle_ = dlopen("libnvidia-ml.so.1", RTLD_LAZY);
    if (!nvml_handle_) {
        nvml_handle_ = dlopen("libnvidia-ml.so", RTLD_LAZY);
    }

    if (!nvml_handle_) {
        // NVML not available - this is not an error, just means no NVIDIA GPU
        return false;
    }

    // Load all required function pointers
    nvmlInit_v2_ = reinterpret_cast<nvmlInit_v2_t>(dlsym(nvml_handle_, "nvmlInit_v2"));
    nvmlShutdown_ = reinterpret_cast<nvmlShutdown_t>(dlsym(nvml_handle_, "nvmlShutdown"));
    nvmlDeviceGetCount_v2_ = reinterpret_cast<nvmlDeviceGetCount_v2_t>(dlsym(nvml_handle_, "nvmlDeviceGetCount_v2"));
    nvmlDeviceGetHandleByIndex_v2_ = reinterpret_cast<nvmlDeviceGetHandleByIndex_v2_t>(dlsym(nvml_handle_, "nvmlDeviceGetHandleByIndex_v2"));
    nvmlDeviceGetName_ = reinterpret_cast<nvmlDeviceGetName_t>(dlsym(nvml_handle_, "nvmlDeviceGetName"));
    nvmlDeviceGetUtilizationRates_ = reinterpret_cast<nvmlDeviceGetUtilizationRates_t>(dlsym(nvml_handle_, "nvmlDeviceGetUtilizationRates"));
    nvmlDeviceGetTemperature_ = reinterpret_cast<nvmlDeviceGetTemperature_t>(dlsym(nvml_handle_, "nvmlDeviceGetTemperature"));
    nvmlDeviceGetMemoryInfo_ = reinterpret_cast<nvmlDeviceGetMemoryInfo_t>(dlsym(nvml_handle_, "nvmlDeviceGetMemoryInfo"));

    // Check that all required functions were loaded
    if (!nvmlInit_v2_ || !nvmlShutdown_ || !nvmlDeviceGetCount_v2_ ||
        !nvmlDeviceGetHandleByIndex_v2_ || !nvmlDeviceGetName_ ||
        !nvmlDeviceGetUtilizationRates_ || !nvmlDeviceGetTemperature_ ||
        !nvmlDeviceGetMemoryInfo_) {
        dlclose(nvml_handle_);
        nvml_handle_ = nullptr;
        return false;
    }

    // Initialize NVML
    nvmlReturn_t result = nvmlInit_v2_();
    if (result != NVML_SUCCESS) {
        dlclose(nvml_handle_);
        nvml_handle_ = nullptr;
        return false;
    }

    nvml_initialized_ = true;
    return true;
}

void NvidiaGpuCollector::unloadNvml() {
    if (nvml_initialized_ && nvmlShutdown_) {
        nvmlShutdown_();
        nvml_initialized_ = false;
    }

    if (nvml_handle_) {
        dlclose(nvml_handle_);
        nvml_handle_ = nullptr;
    }

    nvml_available_ = false;
}

std::vector<GpuMetrics> NvidiaGpuCollector::collect() {
    std::vector<GpuMetrics> gpus;

    if (!nvml_available_ || !nvml_initialized_) {
        return gpus;
    }

    // Get device count
    unsigned int device_count = 0;
    nvmlReturn_t result = nvmlDeviceGetCount_v2_(&device_count);
    if (result != NVML_SUCCESS || device_count == 0) {
        return gpus;
    }

    // Collect metrics for each device
    for (unsigned int i = 0; i < device_count; ++i) {
        nvmlDevice_t device;
        result = nvmlDeviceGetHandleByIndex_v2_(i, &device);
        if (result != NVML_SUCCESS) {
            continue;
        }

        GpuMetrics metrics;
        metrics.vendor = "NVIDIA";
        metrics.usage_percent = 0.0f;
        metrics.temperature_celsius = -1.0f;
        metrics.vram_used_bytes = 0;
        metrics.vram_total_bytes = 0;

        // Get device name
        char name_buffer[256];
        std::memset(name_buffer, 0, sizeof(name_buffer));
        result = nvmlDeviceGetName_(device, name_buffer, sizeof(name_buffer) - 1);
        if (result == NVML_SUCCESS) {
            metrics.name = name_buffer;
        } else {
            metrics.name = "NVIDIA GPU";
        }

        // Get utilization rates
        nvmlUtilization_t utilization;
        result = nvmlDeviceGetUtilizationRates_(device, &utilization);
        if (result == NVML_SUCCESS) {
            metrics.usage_percent = static_cast<float>(utilization.gpu);
        }

        // Get temperature
        unsigned int temp = 0;
        result = nvmlDeviceGetTemperature_(device, NVML_TEMPERATURE_GPU, &temp);
        if (result == NVML_SUCCESS) {
            metrics.temperature_celsius = static_cast<float>(temp);
        }

        // Get memory info
        nvmlMemory_t memory;
        result = nvmlDeviceGetMemoryInfo_(device, &memory);
        if (result == NVML_SUCCESS) {
            metrics.vram_used_bytes = static_cast<uint64_t>(memory.used);
            metrics.vram_total_bytes = static_cast<uint64_t>(memory.total);
        }

        gpus.push_back(metrics);
    }

    return gpus;
}

} // namespace platform
} // namespace resmon
