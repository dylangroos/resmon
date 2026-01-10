#include "ram_macos.h"

#include <mach/mach.h>
#include <mach/mach_host.h>
#include <sys/sysctl.h>

namespace resmon {
namespace platform {

RamMetrics RamCollector::collect() {
    RamMetrics metrics;
    metrics.total_bytes = getTotalMemory();
    metrics.used_bytes = getUsedMemory();

    if (metrics.total_bytes > 0) {
        metrics.usage_percent = static_cast<float>(metrics.used_bytes) /
                                static_cast<float>(metrics.total_bytes) * 100.0f;

        // Clamp to valid range
        if (metrics.usage_percent < 0.0f) metrics.usage_percent = 0.0f;
        if (metrics.usage_percent > 100.0f) metrics.usage_percent = 100.0f;
    } else {
        metrics.usage_percent = 0.0f;
    }

    return metrics;
}

uint64_t RamCollector::getTotalMemory() {
    int64_t mem_size = 0;
    size_t size = sizeof(mem_size);

    if (sysctlbyname("hw.memsize", &mem_size, &size, nullptr, 0) == 0) {
        return static_cast<uint64_t>(mem_size);
    }

    return 0;
}

uint64_t RamCollector::getUsedMemory() {
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    vm_statistics64_data_t vm_stats;

    kern_return_t result = host_statistics64(
        mach_host_self(),
        HOST_VM_INFO64,
        reinterpret_cast<host_info64_t>(&vm_stats),
        &count
    );

    if (result != KERN_SUCCESS) {
        return 0;
    }

    // Get page size
    vm_size_t page_size;
    host_page_size(mach_host_self(), &page_size);

    // Calculate used memory
    // Active + Wired + Compressed - these represent memory that's actively in use
    // Inactive pages can be reclaimed but are currently holding data
    // Free pages are completely unused

    uint64_t active = static_cast<uint64_t>(vm_stats.active_count) * page_size;
    uint64_t wired = static_cast<uint64_t>(vm_stats.wire_count) * page_size;
    uint64_t compressed = static_cast<uint64_t>(vm_stats.compressor_page_count) * page_size;

    // Used memory = Active + Wired + Compressed
    // This matches Activity Monitor's "Memory Used" calculation
    return active + wired + compressed;
}

} // namespace platform
} // namespace resmon
