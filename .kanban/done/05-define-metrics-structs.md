# Define Metrics Structs

**Priority:** HIGH
**Phase:** 2 - Core Data Structures

## Description
Define the core data structures for CPU, GPU, and RAM metrics.

## Structs
```cpp
struct CpuMetrics {
    float usage_percent;      // 0-100
    float temperature_celsius; // -1 if unavailable
    int core_count;
};

struct GpuMetrics {
    std::string name;
    std::string vendor;       // "NVIDIA", "AMD", "Intel"
    float usage_percent;
    float temperature_celsius;
    uint64_t vram_used_mb;
    uint64_t vram_total_mb;
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
    std::chrono::steady_clock::time_point timestamp;
};
```

## Tasks
- [ ] Create src/core/metrics.h
- [ ] Define all structs
- [ ] Add helper functions (format bytes, etc.)

## Acceptance Criteria
- Compiles without errors
- Structs are easy to use from GUI layer
