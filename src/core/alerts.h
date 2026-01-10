#ifndef RESMON_CORE_ALERTS_H
#define RESMON_CORE_ALERTS_H

namespace resmon {

enum class AlertSeverity { None, Warning, Critical };

struct AlertThreshold {
    float warning;   // e.g., 80.0
    float critical;  // e.g., 95.0
    bool enabled = true;
};

struct AlertConfig {
    AlertThreshold cpu_usage{80.0f, 95.0f};
    AlertThreshold cpu_temp{70.0f, 85.0f};
    AlertThreshold gpu_usage{80.0f, 95.0f};
    AlertThreshold gpu_temp{75.0f, 90.0f};
    AlertThreshold ram_usage{80.0f, 95.0f};
};

} // namespace resmon

#endif // RESMON_CORE_ALERTS_H
