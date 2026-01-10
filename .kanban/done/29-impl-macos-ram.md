# Implement macOS RAM

**Priority:** MEDIUM
**Phase:** 7 - macOS Backend

## Description
RAM metrics on macOS using Mach APIs.

## Approach
```cpp
#include <mach/mach.h>

vm_statistics64_data_t vmStats;
mach_msg_type_number_t infoCount = HOST_VM_INFO64_COUNT;
host_statistics64(mach_host_self(), HOST_VM_INFO64,
                  (host_info64_t)&vmStats, &infoCount);

// Calculate from page counts * page_size
```

## Tasks
- [ ] Create src/backend/macos/ram_macos.cpp
- [ ] Query vm_statistics64
- [ ] Calculate used/free/total
- [ ] Handle unified memory on Apple Silicon

## Acceptance Criteria
- Matches Activity Monitor memory pressure
- Correct on Intel and Apple Silicon Macs
