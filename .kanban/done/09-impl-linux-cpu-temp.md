# Implement Linux CPU Temperature

**Priority:** MEDIUM
**Phase:** 3 - Linux Backend

## Description
Read CPU temperature from /sys/class/hwmon.

## Approach
```bash
# Find correct hwmon device
/sys/class/hwmon/hwmon*/name  # Look for "coretemp" or "k10temp"
/sys/class/hwmon/hwmon*/temp1_input  # Temperature in millidegrees
```

## Tasks
- [ ] Scan /sys/class/hwmon for CPU sensor
- [ ] Read temperature (convert from millidegrees)
- [ ] Handle missing sensor gracefully (-1)
- [ ] Cache hwmon path after first discovery

## Acceptance Criteria
- Returns temperature in Celsius
- Works on Intel (coretemp) and AMD (k10temp)
- Returns -1 if sensor not found
