# Implement Linux CPU Usage

**Priority:** HIGH
**Phase:** 3 - Linux Backend

## Description
Read CPU usage percentage from /proc/stat.

## Approach
```cpp
// /proc/stat format:
// cpu  user nice system idle iowait irq softirq steal guest guest_nice

// Calculate: usage = 1 - (idle_delta / total_delta)
```

## Tasks
- [ ] Create src/backend/linux/cpu_linux.cpp
- [ ] Parse /proc/stat
- [ ] Calculate usage % between samples
- [ ] Handle multi-core (aggregate + per-core)
- [ ] Store previous sample for delta calculation

## Acceptance Criteria
- Returns 0-100% usage
- Matches htop/top values (within ~2%)
- Updates correctly over time
