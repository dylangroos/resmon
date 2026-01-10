#include "macos_backend.h"

namespace resmon {
namespace platform {

MacOSBackend::MacOSBackend()
    : cpu_collector_()
    , ram_collector_()
    , gpu_collector_()
{
}

SystemMetrics MacOSBackend::collect() {
    SystemMetrics metrics;

    // Collect CPU metrics
    metrics.cpu = cpu_collector_.collect();

    // Collect RAM metrics
    metrics.ram = ram_collector_.collect();

    // Collect GPU metrics
    // macOS supports Apple Silicon, AMD, NVIDIA (older Macs), and Intel GPUs
    auto gpus = gpu_collector_.collect();
    metrics.gpus.insert(metrics.gpus.end(), gpus.begin(), gpus.end());

    return metrics;
}

} // namespace platform
} // namespace resmon
