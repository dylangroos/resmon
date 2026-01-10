# Define Backend Interfaces

**Priority:** MEDIUM
**Phase:** 2 - Core Data Structures

## Description
Create abstract interface for metrics collection, enabling platform-specific implementations.

## Interface
```cpp
class IMetricsBackend {
public:
    virtual ~IMetricsBackend() = default;
    virtual CpuMetrics getCpuMetrics() = 0;
    virtual std::vector<GpuMetrics> getGpuMetrics() = 0;
    virtual RamMetrics getRamMetrics() = 0;
    virtual SystemMetrics getAllMetrics() = 0;
};

// Factory
std::unique_ptr<IMetricsBackend> createPlatformBackend();
```

## Tasks
- [ ] Create src/core/backend_interface.h
- [ ] Define IMetricsBackend interface
- [ ] Add factory function declaration
- [ ] Document expected behavior

## Acceptance Criteria
- Clean interface that hides platform details
- Easy to implement for each OS
