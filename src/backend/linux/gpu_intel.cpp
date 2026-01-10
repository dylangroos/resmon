#include "gpu_intel.h"

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <cstring>

namespace resmon {
namespace platform {

// Intel vendor ID
static constexpr const char* INTEL_VENDOR_ID = "0x8086";

IntelGpuCollector::IntelGpuCollector() {
    scanForGpus();
}

void IntelGpuCollector::scanForGpus() {
    gpus_.clear();

    DIR* drm_dir = opendir("/sys/class/drm");
    if (!drm_dir) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(drm_dir)) != nullptr) {
        // Look for card* entries (not card*-*)
        if (strncmp(entry->d_name, "card", 4) != 0) {
            continue;
        }
        // Skip entries like card0-HDMI-A-1
        if (strchr(entry->d_name + 4, '-') != nullptr) {
            continue;
        }

        std::string card_path = "/sys/class/drm/";
        card_path += entry->d_name;
        std::string device_path = card_path + "/device";

        // Check vendor ID
        std::string vendor_path = device_path + "/vendor";
        std::string vendor = readSysfsString(vendor_path);
        if (vendor != INTEL_VENDOR_ID) {
            continue;
        }

        // Found an Intel GPU
        IntelGpuInfo info;
        info.card_path = device_path;
        info.name = "Intel Graphics";

        // Find temperature path in hwmon if available
        info.temp_path = findHwmonTempPath(device_path);

        gpus_.push_back(info);
    }

    closedir(drm_dir);
}

std::string IntelGpuCollector::findHwmonTempPath(const std::string& device_path) {
    std::string hwmon_dir = device_path + "/hwmon";

    DIR* dir = opendir(hwmon_dir.c_str());
    if (!dir) {
        return "";
    }

    std::string temp_path;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strncmp(entry->d_name, "hwmon", 5) == 0) {
            temp_path = hwmon_dir + "/" + entry->d_name + "/temp1_input";
            // Check if file exists
            std::ifstream test(temp_path);
            if (test.good()) {
                closedir(dir);
                return temp_path;
            }
        }
    }

    closedir(dir);
    return "";
}

std::string IntelGpuCollector::readSysfsString(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }

    std::string value;
    std::getline(file, value);

    // Trim trailing whitespace/newlines
    while (!value.empty() && (value.back() == '\n' || value.back() == '\r' || value.back() == ' ')) {
        value.pop_back();
    }

    return value;
}

int64_t IntelGpuCollector::readSysfsInt(const std::string& path) {
    std::string str = readSysfsString(path);
    if (str.empty()) {
        return -1;
    }

    try {
        return std::stoll(str);
    } catch (...) {
        return -1;
    }
}

std::vector<GpuMetrics> IntelGpuCollector::collect() {
    std::vector<GpuMetrics> results;

    for (const auto& gpu : gpus_) {
        GpuMetrics metrics;
        metrics.name = gpu.name;
        metrics.vendor = "Intel";

        // Intel iGPU utilization not easily available without perf counters
        // Set to 0 as a placeholder
        metrics.usage_percent = 0.0f;

        // Read temperature if hwmon is available (in millidegrees, divide by 1000)
        if (!gpu.temp_path.empty()) {
            int64_t temp = readSysfsInt(gpu.temp_path);
            if (temp > 0) {
                metrics.temperature_celsius = static_cast<float>(temp) / 1000.0f;
            } else {
                metrics.temperature_celsius = -1.0f;
            }
        } else {
            metrics.temperature_celsius = -1.0f;
        }

        // Intel iGPU uses shared system memory, no dedicated VRAM
        metrics.vram_used_bytes = 0;
        metrics.vram_total_bytes = 0;

        results.push_back(metrics);
    }

    return results;
}

} // namespace platform
} // namespace resmon
