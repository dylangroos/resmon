# Define Alert Config

**Priority:** MEDIUM
**Phase:** 2 - Core Data Structures

## Description
Define structures for alert thresholds and state tracking.

## Structs
```cpp
enum class AlertSeverity { None, Warning, Critical };

struct AlertThreshold {
    float warning_threshold;  // e.g., 80%
    float critical_threshold; // e.g., 95%
    bool enabled = true;
};

struct AlertConfig {
    AlertThreshold cpu_usage;
    AlertThreshold cpu_temp;
    AlertThreshold gpu_usage;
    AlertThreshold gpu_temp;
    AlertThreshold ram_usage;
};

struct AlertState {
    AlertSeverity cpu_severity = AlertSeverity::None;
    AlertSeverity gpu_severity = AlertSeverity::None;
    AlertSeverity ram_severity = AlertSeverity::None;
    // Cooldown tracking to avoid spam
};
```

## Tasks
- [ ] Create src/core/alerts.h
- [ ] Define AlertConfig struct
- [ ] Define AlertState struct
- [ ] Add default threshold values

## Acceptance Criteria
- Thresholds are configurable
- State tracks current alert status
