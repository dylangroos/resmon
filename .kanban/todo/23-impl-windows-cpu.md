# Implement Windows CPU

**Priority:** HIGH
**Phase:** 6 - Windows Backend

## Description
CPU usage and temperature on Windows using PDH and WMI.

## Approach
```cpp
// CPU Usage via PDH
PdhOpenQuery(NULL, 0, &query);
PdhAddCounter(query, "\\Processor(_Total)\\% Processor Time", 0, &counter);
PdhCollectQueryData(query);
PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);

// Temperature via WMI (MSAcpi_ThermalZoneTemperature)
// Or OpenHardwareMonitor WMI interface
```

## Tasks
- [ ] Create src/backend/windows/cpu_windows.cpp
- [ ] PDH for CPU usage
- [ ] WMI for temperature (if available)
- [ ] Handle missing temperature sensor

## Acceptance Criteria
- Matches Task Manager CPU %
- Temperature works where available
