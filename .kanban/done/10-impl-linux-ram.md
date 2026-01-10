# Implement Linux RAM Metrics

**Priority:** HIGH
**Phase:** 3 - Linux Backend

## Description
Parse /proc/meminfo for RAM usage.

## Approach
```bash
# /proc/meminfo format:
MemTotal:       16384000 kB
MemFree:         1234567 kB
MemAvailable:    8000000 kB
Buffers:          123456 kB
Cached:          4000000 kB
```

```cpp
// Used = Total - Available (most accurate)
// Or: Used = Total - Free - Buffers - Cached
```

## Tasks
- [ ] Create src/backend/linux/ram_linux.cpp
- [ ] Parse /proc/meminfo
- [ ] Calculate used memory
- [ ] Return total, used, percentage

## Acceptance Criteria
- Matches `free -h` output
- Handles edge cases (no MemAvailable on old kernels)
