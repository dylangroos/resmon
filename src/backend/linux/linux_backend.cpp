#include "linux_backend.h"

namespace resmon {
namespace platform {

LinuxBackend::LinuxBackend()
    : cpu_collector_()
    , ram_collector_()
    , nvidia_gpu_collector_()
    , amd_gpu_collector_()
    , intel_gpu_collector_()
{
}

SystemMetrics LinuxBackend::collect() {
    SystemMetrics metrics;

    // Collect CPU metrics
    metrics.cpu = cpu_collector_.collect();

    // Collect RAM metrics
    metrics.ram = ram_collector_.collect();

    // Collect GPU metrics from all GPU vendors
    // Each collector returns empty vector if no GPUs of that type found

    // NVIDIA GPUs (via NVML)
    auto nvidia_gpus = nvidia_gpu_collector_.collect();
    metrics.gpus.insert(metrics.gpus.end(), nvidia_gpus.begin(), nvidia_gpus.end());

    // AMD GPUs (via sysfs/amdgpu driver)
    auto amd_gpus = amd_gpu_collector_.collect();
    metrics.gpus.insert(metrics.gpus.end(), amd_gpus.begin(), amd_gpus.end());

    // Intel GPUs (via sysfs)
    auto intel_gpus = intel_gpu_collector_.collect();
    metrics.gpus.insert(metrics.gpus.end(), intel_gpus.begin(), intel_gpus.end());

    return metrics;
}

} // namespace platform

// Factory function implementation for Linux
std::unique_ptr<IMetricsBackend> createPlatformBackend() {
    return std::make_unique<platform::LinuxBackend>();
}

} // namespace resmon
