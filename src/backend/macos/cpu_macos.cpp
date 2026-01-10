#include "cpu_macos.h"

#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <sys/sysctl.h>
#include <IOKit/IOKitLib.h>

namespace resmon {
namespace platform {

CpuCollector::CpuCollector()
    : prev_idle_ticks_(0)
    , prev_total_ticks_(0)
    , has_previous_sample_(false)
{
}

CpuMetrics CpuCollector::collect() {
    CpuMetrics metrics;
    metrics.usage_percent = 0.0f;
    metrics.temperature_celsius = -1.0f;
    metrics.core_count = getCoreCount();

    uint64_t idle_ticks = 0;
    uint64_t total_ticks = 0;

    if (readCpuTicks(idle_ticks, total_ticks)) {
        if (has_previous_sample_) {
            uint64_t idle_delta = idle_ticks - prev_idle_ticks_;
            uint64_t total_delta = total_ticks - prev_total_ticks_;

            if (total_delta > 0) {
                float idle_percent = static_cast<float>(idle_delta) / static_cast<float>(total_delta) * 100.0f;
                metrics.usage_percent = 100.0f - idle_percent;

                // Clamp to valid range
                if (metrics.usage_percent < 0.0f) metrics.usage_percent = 0.0f;
                if (metrics.usage_percent > 100.0f) metrics.usage_percent = 100.0f;
            }
        }

        prev_idle_ticks_ = idle_ticks;
        prev_total_ticks_ = total_ticks;
        has_previous_sample_ = true;
    }

    metrics.temperature_celsius = readTemperature();

    return metrics;
}

bool CpuCollector::readCpuTicks(uint64_t& idle_ticks, uint64_t& total_ticks) {
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    host_cpu_load_info_data_t cpu_load;

    kern_return_t result = host_statistics(
        mach_host_self(),
        HOST_CPU_LOAD_INFO,
        reinterpret_cast<host_info_t>(&cpu_load),
        &count
    );

    if (result != KERN_SUCCESS) {
        return false;
    }

    // CPU_STATE_USER, CPU_STATE_SYSTEM, CPU_STATE_IDLE, CPU_STATE_NICE
    idle_ticks = cpu_load.cpu_ticks[CPU_STATE_IDLE];
    total_ticks = cpu_load.cpu_ticks[CPU_STATE_USER] +
                  cpu_load.cpu_ticks[CPU_STATE_SYSTEM] +
                  cpu_load.cpu_ticks[CPU_STATE_IDLE] +
                  cpu_load.cpu_ticks[CPU_STATE_NICE];

    return true;
}

float CpuCollector::readTemperature() {
    // SMC temperature reading requires AppleSMC access
    // This is complex and requires IOKit SMC calls
    // For now, return -1 (unavailable)
    // A full implementation would use SMCReadKey with "TC0P" or similar keys

    // Note: Direct SMC access may require elevated privileges on newer macOS versions
    // Consider using external tools like 'osx-cpu-temp' or system_profiler as alternatives

    return -1.0f;
}

int CpuCollector::getCoreCount() {
    int core_count = 0;
    size_t size = sizeof(core_count);

    // Get physical core count
    if (sysctlbyname("hw.physicalcpu", &core_count, &size, nullptr, 0) == 0) {
        return core_count;
    }

    // Fallback to logical CPU count
    if (sysctlbyname("hw.ncpu", &core_count, &size, nullptr, 0) == 0) {
        return core_count;
    }

    return 1; // Default fallback
}

} // namespace platform
} // namespace resmon
