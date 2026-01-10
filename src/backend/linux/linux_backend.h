#ifndef RESMON_BACKEND_LINUX_LINUX_BACKEND_H
#define RESMON_BACKEND_LINUX_LINUX_BACKEND_H

#include "../../core/backend.h"
#include "cpu_linux.h"
#include "ram_linux.h"
#include "gpu_nvidia.h"
#include "gpu_amd.h"
#include "gpu_intel.h"

namespace resmon {
namespace platform {

class LinuxBackend : public IMetricsBackend {
public:
    LinuxBackend();
    ~LinuxBackend() override = default;

    SystemMetrics collect() override;

private:
    CpuCollector cpu_collector_;
    RamCollector ram_collector_;
    NvidiaGpuCollector nvidia_gpu_collector_;
    AmdGpuCollector amd_gpu_collector_;
    IntelGpuCollector intel_gpu_collector_;
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_LINUX_LINUX_BACKEND_H
