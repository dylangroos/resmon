#ifndef RESMON_ALERTS_ALERT_MANAGER_H
#define RESMON_ALERTS_ALERT_MANAGER_H

#include <chrono>
#include "core/alerts.h"
#include "core/metrics.h"

namespace resmon {

class AlertManager {
public:
    AlertManager();

    // Check metrics against thresholds, return current severity per metric
    struct AlertState {
        AlertSeverity cpu = AlertSeverity::None;
        AlertSeverity ram = AlertSeverity::None;
        AlertSeverity gpu = AlertSeverity::None;  // worst of all GPUs
    };

    AlertState check(const SystemMetrics& metrics);

    // Get/set config
    AlertConfig& config();
    const AlertConfig& config() const;

private:
    // Helper to check a value against a threshold
    AlertSeverity checkThreshold(float value, const AlertThreshold& threshold) const;

    AlertConfig config_;
    std::chrono::steady_clock::time_point last_notification_;
};

} // namespace resmon

#endif // RESMON_ALERTS_ALERT_MANAGER_H
