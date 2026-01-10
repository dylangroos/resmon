#ifndef RESMON_CORE_METRICS_H
#define RESMON_CORE_METRICS_H

#include <cstdint>
#include <string>
#include <vector>

namespace resmon {

struct CpuMetrics {
    float usage_percent;       // 0-100
    float temperature_celsius; // -1 if unavailable
    int core_count;
};

struct GpuMetrics {
    std::string name;
    std::string vendor;        // "NVIDIA", "AMD", "Intel", "Unknown"
    float usage_percent;
    float temperature_celsius;
    uint64_t vram_used_bytes;
    uint64_t vram_total_bytes;
};

struct RamMetrics {
    uint64_t used_bytes;
    uint64_t total_bytes;
    float usage_percent;
};

struct SystemMetrics {
    CpuMetrics cpu;
    std::vector<GpuMetrics> gpus;
    RamMetrics ram;
};

} // namespace resmon

#endif // RESMON_CORE_METRICS_H
