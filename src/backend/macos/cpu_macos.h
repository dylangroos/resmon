#ifndef RESMON_BACKEND_MACOS_CPU_MACOS_H
#define RESMON_BACKEND_MACOS_CPU_MACOS_H

#include "../../core/metrics.h"
#include <mach/mach.h>

namespace resmon {
namespace platform {

class CpuCollector {
public:
    CpuCollector();

    CpuMetrics collect();

private:
    // Previous sample values for calculating delta
    uint64_t prev_idle_ticks_;
    uint64_t prev_total_ticks_;
    bool has_previous_sample_;

    // Read CPU ticks from Mach host statistics
    // Returns false if failed to read
    bool readCpuTicks(uint64_t& idle_ticks, uint64_t& total_ticks);

    // Read CPU temperature via SMC (System Management Controller)
    // Returns -1 if not available
    float readTemperature();

    // Get CPU core count
    int getCoreCount();
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_MACOS_CPU_MACOS_H
