# Implement Windows RAM

**Priority:** HIGH
**Phase:** 6 - Windows Backend

## Description
RAM metrics on Windows using GlobalMemoryStatusEx.

## Approach
```cpp
#include <windows.h>

MEMORYSTATUSEX memInfo;
memInfo.dwLength = sizeof(MEMORYSTATUSEX);
GlobalMemoryStatusEx(&memInfo);

uint64_t totalPhys = memInfo.ullTotalPhys;
uint64_t availPhys = memInfo.ullAvailPhys;
uint64_t usedPhys = totalPhys - availPhys;
```

## Tasks
- [ ] Create src/backend/windows/ram_windows.cpp
- [ ] Call GlobalMemoryStatusEx
- [ ] Calculate used/total/percentage
- [ ] Handle edge cases

## Acceptance Criteria
- Matches Task Manager memory usage
- Correct units (bytes)
