# Implement Threshold Checker

**Priority:** MEDIUM
**Phase:** 5 - Alert System

## Description
AlertManager class that checks metrics against thresholds.

## Design
```cpp
class AlertManager {
    AlertConfig config;
    AlertState state;
    std::chrono::steady_clock::time_point lastNotification;

    AlertState checkMetrics(const SystemMetrics& metrics);
    bool shouldNotify(AlertSeverity severity);
};
```

## Tasks
- [ ] Create src/alerts/alert_manager.h/.cpp
- [ ] Compare metrics to thresholds
- [ ] Track state changes (entering/exiting alert)
- [ ] Implement notification cooldown (e.g., 60 seconds)
- [ ] Return severity for UI coloring

## Acceptance Criteria
- Correctly identifies warning/critical states
- Cooldown prevents notification spam
- State tracking is accurate
