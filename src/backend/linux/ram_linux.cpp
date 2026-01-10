#include "ram_linux.h"

#include <fstream>
#include <sstream>
#include <string>

namespace resmon {
namespace platform {

RamMetrics RamCollector::collect() {
    RamMetrics metrics;
    metrics.used_bytes = 0;
    metrics.total_bytes = 0;
    metrics.usage_percent = 0.0f;

    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return metrics;
    }

    uint64_t mem_total_kb = 0;
    uint64_t mem_available_kb = 0;
    uint64_t mem_free_kb = 0;
    uint64_t buffers_kb = 0;
    uint64_t cached_kb = 0;

    bool has_mem_available = false;

    std::string line;
    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            mem_total_kb = parseMemInfoLine(line);
        } else if (line.find("MemAvailable:") == 0) {
            mem_available_kb = parseMemInfoLine(line);
            has_mem_available = true;
        } else if (line.find("MemFree:") == 0) {
            mem_free_kb = parseMemInfoLine(line);
        } else if (line.find("Buffers:") == 0) {
            buffers_kb = parseMemInfoLine(line);
        } else if (line.find("Cached:") == 0) {
            // Make sure we get "Cached:" and not "SwapCached:"
            if (line.find("SwapCached:") != 0) {
                cached_kb = parseMemInfoLine(line);
            }
        }
    }

    // Convert to bytes (meminfo reports in kB)
    metrics.total_bytes = mem_total_kb * 1024;

    uint64_t available_kb;
    if (has_mem_available) {
        // MemAvailable is the best metric (available since Linux 3.14)
        available_kb = mem_available_kb;
    } else {
        // Fallback for older kernels: approximate available as free + buffers + cached
        available_kb = mem_free_kb + buffers_kb + cached_kb;
    }

    uint64_t available_bytes = available_kb * 1024;

    // Calculate used memory
    if (metrics.total_bytes > available_bytes) {
        metrics.used_bytes = metrics.total_bytes - available_bytes;
    } else {
        metrics.used_bytes = 0;
    }

    // Calculate percentage
    if (metrics.total_bytes > 0) {
        metrics.usage_percent = static_cast<float>(
            (static_cast<double>(metrics.used_bytes) / static_cast<double>(metrics.total_bytes)) * 100.0
        );

        // Clamp to valid range
        if (metrics.usage_percent < 0.0f) metrics.usage_percent = 0.0f;
        if (metrics.usage_percent > 100.0f) metrics.usage_percent = 100.0f;
    }

    return metrics;
}

uint64_t RamCollector::parseMemInfoLine(const std::string& line) {
    // Format: "FieldName:      12345 kB"
    std::istringstream iss(line);
    std::string field_name;
    uint64_t value = 0;
    std::string unit;

    // Skip the field name (includes the colon)
    iss >> field_name >> value >> unit;

    return value;
}

} // namespace platform
} // namespace resmon
