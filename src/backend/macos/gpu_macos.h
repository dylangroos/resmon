#ifndef RESMON_BACKEND_MACOS_GPU_MACOS_H
#define RESMON_BACKEND_MACOS_GPU_MACOS_H

#include "../../core/metrics.h"

#include <cstdint>
#include <string>
#include <vector>

namespace resmon {
namespace platform {

class GpuCollector {
public:
    GpuCollector();
    ~GpuCollector();

    std::vector<GpuMetrics> collect();

private:
    // Detect available GPUs using IOKit
    void detectGpus();

    // Collect metrics for Apple Silicon integrated GPU
    GpuMetrics collectAppleSiliconGpu();

    // Collect metrics for discrete AMD GPU (older Macs)
    GpuMetrics collectAmdGpu();

    // Collect metrics for discrete NVIDIA GPU (very old Macs)
    GpuMetrics collectNvidiaGpu();

    // GPU type enumeration
    enum class GpuType {
        AppleSilicon,
        AMD,
        NVIDIA,
        Intel,
        Unknown
    };

    struct GpuInfo {
        GpuType type;
        std::string name;
        uint64_t vram_bytes;
    };

    std::vector<GpuInfo> detected_gpus_;
    bool gpus_detected_;
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_MACOS_GPU_MACOS_H
