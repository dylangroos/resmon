#ifndef RESMON_BACKEND_LINUX_GPU_INTEL_H
#define RESMON_BACKEND_LINUX_GPU_INTEL_H

#include "../../core/metrics.h"

#include <string>
#include <vector>

namespace resmon {
namespace platform {

// Information about a detected Intel GPU
struct IntelGpuInfo {
    std::string card_path;     // e.g., /sys/class/drm/card0/device
    std::string name;
    std::string temp_path;     // hwmon temperature if available
};

class IntelGpuCollector {
public:
    IntelGpuCollector();
    ~IntelGpuCollector() = default;

    // Non-copyable
    IntelGpuCollector(const IntelGpuCollector&) = delete;
    IntelGpuCollector& operator=(const IntelGpuCollector&) = delete;

    // Collect metrics from all Intel GPUs
    // Returns empty vector if no Intel GPUs found
    std::vector<GpuMetrics> collect();

    // Check if any Intel GPUs were detected
    bool hasGpus() const { return !gpus_.empty(); }

private:
    void scanForGpus();
    std::string findHwmonTempPath(const std::string& device_path);
    std::string readSysfsString(const std::string& path);
    int64_t readSysfsInt(const std::string& path);

    std::vector<IntelGpuInfo> gpus_;
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_LINUX_GPU_INTEL_H
