#ifndef RESMON_BACKEND_MACOS_MACOS_BACKEND_H
#define RESMON_BACKEND_MACOS_MACOS_BACKEND_H

#include "../../core/backend.h"
#include "cpu_macos.h"
#include "ram_macos.h"
#include "gpu_macos.h"

namespace resmon {
namespace platform {

class MacOSBackend : public IMetricsBackend {
public:
    MacOSBackend();
    ~MacOSBackend() override = default;
    SystemMetrics collect() override;

private:
    CpuCollector cpu_collector_;
    RamCollector ram_collector_;
    GpuCollector gpu_collector_;
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_MACOS_MACOS_BACKEND_H
