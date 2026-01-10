#ifndef RESMON_BACKEND_LINUX_CPU_LINUX_H
#define RESMON_BACKEND_LINUX_CPU_LINUX_H

#include "../../core/metrics.h"

namespace resmon {
namespace platform {

class CpuCollector {
public:
    CpuCollector();

    CpuMetrics collect();

private:
    // Previous sample values for calculating delta
    uint64_t prev_idle_time_;
    uint64_t prev_total_time_;
    bool has_previous_sample_;

    // Read /proc/stat and parse CPU times
    // Returns false if failed to read
    bool readCpuTimes(uint64_t& idle_time, uint64_t& total_time);

    // Read CPU temperature from /sys/class/hwmon
    // Returns -1 if not found
    float readTemperature();

    // Get CPU core count
    int getCoreCount();
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_LINUX_CPU_LINUX_H
