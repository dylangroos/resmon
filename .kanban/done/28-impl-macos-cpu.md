# Implement macOS CPU

**Priority:** MEDIUM
**Phase:** 7 - macOS Backend

## Description
CPU usage and temperature on macOS.

## Approach
```cpp
// CPU Usage via host_processor_info
#include <mach/mach.h>
host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, ...);

// Temperature via IOKit/SMC
// Requires reading SMC keys (TC0P, TC0D, etc.)
```

## Tasks
- [ ] Create src/backend/macos/cpu_macos.cpp
- [ ] host_processor_info for usage
- [ ] SMC reading for temperature
- [ ] Handle Apple Silicon vs Intel

## Acceptance Criteria
- Matches Activity Monitor CPU %
- Temperature works on supported Macs
