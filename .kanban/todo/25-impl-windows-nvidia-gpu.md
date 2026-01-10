# Implement Windows NVIDIA GPU

**Priority:** MEDIUM
**Phase:** 6 - Windows Backend

## Description
NVML on Windows for NVIDIA GPU stats.

## Approach
Same as Linux - NVML API is cross-platform.
```cpp
// nvml.dll ships with NVIDIA driver
// LoadLibrary("nvml.dll") for runtime loading
```

## Tasks
- [ ] Create src/backend/windows/gpu_nvidia_windows.cpp
- [ ] Runtime load nvml.dll
- [ ] Reuse NVML logic from Linux (share code)
- [ ] Handle driver not installed

## Acceptance Criteria
- Matches nvidia-smi output
- Works without NVML (returns empty)
