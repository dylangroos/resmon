#ifndef RESMON_BACKEND_LINUX_GPU_AMD_H
#define RESMON_BACKEND_LINUX_GPU_AMD_H

#include "../../core/metrics.h"

#include <string>
#include <vector>

namespace resmon {
namespace platform {

// Information about a detected AMD GPU
struct AmdGpuInfo {
    std::string card_path;           // e.g., /sys/class/drm/card0/device
    std::string name;
    std::string gpu_busy_path;       // gpu_busy_percent
    std::string temp_path;           // hwmon temperature
    std::string vram_used_path;      // mem_info_vram_used
    std::string vram_total_path;     // mem_info_vram_total
};

class AmdGpuCollector {
public:
    AmdGpuCollector();
    ~AmdGpuCollector() = default;

    // Non-copyable
    AmdGpuCollector(const AmdGpuCollector&) = delete;
    AmdGpuCollector& operator=(const AmdGpuCollector&) = delete;

    // Collect metrics from all AMD GPUs
    // Returns empty vector if no AMD GPUs found
    std::vector<GpuMetrics> collect();

    // Check if any AMD GPUs were detected
    bool hasGpus() const { return !gpus_.empty(); }

private:
    void scanForGpus();
    std::string findHwmonTempPath(const std::string& device_path);
    std::string readSysfsString(const std::string& path);
    int64_t readSysfsInt(const std::string& path);

    std::vector<AmdGpuInfo> gpus_;
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_LINUX_GPU_AMD_H
