#include "alerts/alert_manager.h"

namespace resmon {

AlertManager::AlertManager()
    : config_{}
    , last_notification_{std::chrono::steady_clock::now()}
{
}

AlertSeverity AlertManager::checkThreshold(float value, const AlertThreshold& threshold) const {
    if (!threshold.enabled) {
        return AlertSeverity::None;
    }
    if (value >= threshold.critical) {
        return AlertSeverity::Critical;
    }
    if (value >= threshold.warning) {
        return AlertSeverity::Warning;
    }
    return AlertSeverity::None;
}

AlertManager::AlertState AlertManager::check(const SystemMetrics& metrics) {
    AlertState state;

    // Check CPU usage
    AlertSeverity cpu_usage_severity = checkThreshold(metrics.cpu.usage_percent, config_.cpu_usage);

    // Check CPU temperature (only if valid)
    AlertSeverity cpu_temp_severity = AlertSeverity::None;
    if (metrics.cpu.temperature_celsius >= 0) {
        cpu_temp_severity = checkThreshold(metrics.cpu.temperature_celsius, config_.cpu_temp);
    }

    // CPU severity is the worst of usage and temperature
    state.cpu = (cpu_temp_severity > cpu_usage_severity) ? cpu_temp_severity : cpu_usage_severity;

    // Check RAM usage
    state.ram = checkThreshold(metrics.ram.usage_percent, config_.ram_usage);

    // Check GPUs - return worst severity across all GPUs
    state.gpu = AlertSeverity::None;
    for (const auto& gpu : metrics.gpus) {
        AlertSeverity gpu_usage_severity = checkThreshold(gpu.usage_percent, config_.gpu_usage);

        AlertSeverity gpu_temp_severity = AlertSeverity::None;
        if (gpu.temperature_celsius >= 0) {
            gpu_temp_severity = checkThreshold(gpu.temperature_celsius, config_.gpu_temp);
        }

        // Get worst severity for this GPU
        AlertSeverity gpu_severity = (gpu_temp_severity > gpu_usage_severity) ? gpu_temp_severity : gpu_usage_severity;

        // Update overall GPU severity if this one is worse
        if (gpu_severity > state.gpu) {
            state.gpu = gpu_severity;
        }
    }

    return state;
}

AlertConfig& AlertManager::config() {
    return config_;
}

const AlertConfig& AlertManager::config() const {
    return config_;
}

} // namespace resmon
