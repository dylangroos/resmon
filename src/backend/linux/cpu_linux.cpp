#include "cpu_linux.h"

#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <thread>

namespace resmon {
namespace platform {

CpuCollector::CpuCollector()
    : prev_idle_time_(0)
    , prev_total_time_(0)
    , has_previous_sample_(false)
{
}

CpuMetrics CpuCollector::collect() {
    CpuMetrics metrics;
    metrics.usage_percent = 0.0f;
    metrics.temperature_celsius = -1.0f;
    metrics.core_count = getCoreCount();

    uint64_t idle_time = 0;
    uint64_t total_time = 0;

    if (readCpuTimes(idle_time, total_time)) {
        if (has_previous_sample_ && total_time > prev_total_time_) {
            uint64_t idle_delta = idle_time - prev_idle_time_;
            uint64_t total_delta = total_time - prev_total_time_;

            if (total_delta > 0) {
                // usage = 100 * (1 - (idle_delta / total_delta))
                double idle_fraction = static_cast<double>(idle_delta) / static_cast<double>(total_delta);
                metrics.usage_percent = static_cast<float>(100.0 * (1.0 - idle_fraction));

                // Clamp to valid range
                if (metrics.usage_percent < 0.0f) metrics.usage_percent = 0.0f;
                if (metrics.usage_percent > 100.0f) metrics.usage_percent = 100.0f;
            }
        }

        prev_idle_time_ = idle_time;
        prev_total_time_ = total_time;
        has_previous_sample_ = true;
    }

    metrics.temperature_celsius = readTemperature();

    return metrics;
}

bool CpuCollector::readCpuTimes(uint64_t& idle_time, uint64_t& total_time) {
    std::ifstream stat_file("/proc/stat");
    if (!stat_file.is_open()) {
        return false;
    }

    std::string line;
    if (!std::getline(stat_file, line)) {
        return false;
    }

    // /proc/stat first line: cpu user nice system idle iowait irq softirq steal
    std::istringstream iss(line);
    std::string cpu_label;
    iss >> cpu_label;

    if (cpu_label != "cpu") {
        return false;
    }

    uint64_t user = 0, nice = 0, system = 0, idle = 0;
    uint64_t iowait = 0, irq = 0, softirq = 0, steal = 0;

    iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    // idle_time = idle + iowait
    idle_time = idle + iowait;

    // total_time = user + nice + system + idle + iowait + irq + softirq + steal
    total_time = user + nice + system + idle + iowait + irq + softirq + steal;

    return true;
}

float CpuCollector::readTemperature() {
    // Look for CPU temperature in /sys/class/hwmon
    // Common drivers: coretemp (Intel), k10temp (AMD)

    DIR* hwmon_dir = opendir("/sys/class/hwmon");
    if (!hwmon_dir) {
        return -1.0f;
    }

    struct dirent* entry;
    while ((entry = readdir(hwmon_dir)) != nullptr) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        std::string hwmon_path = "/sys/class/hwmon/";
        hwmon_path += entry->d_name;

        // Check the name file to identify the sensor
        std::string name_path = hwmon_path + "/name";
        std::ifstream name_file(name_path);
        if (!name_file.is_open()) {
            continue;
        }

        std::string sensor_name;
        std::getline(name_file, sensor_name);
        name_file.close();

        // Look for CPU temperature sensors
        if (sensor_name == "coretemp" || sensor_name == "k10temp" ||
            sensor_name == "zenpower" || sensor_name == "cpu_thermal") {

            // Try temp1_input first (usually package/die temp)
            std::string temp_path = hwmon_path + "/temp1_input";
            std::ifstream temp_file(temp_path);
            if (temp_file.is_open()) {
                int temp_millidegrees = 0;
                temp_file >> temp_millidegrees;
                temp_file.close();
                closedir(hwmon_dir);

                // Convert from millidegrees to degrees
                return static_cast<float>(temp_millidegrees) / 1000.0f;
            }
        }
    }

    closedir(hwmon_dir);

    // Fallback: try thermal zones
    for (int i = 0; i < 10; ++i) {
        std::string thermal_path = "/sys/class/thermal/thermal_zone" + std::to_string(i) + "/type";
        std::ifstream type_file(thermal_path);
        if (!type_file.is_open()) {
            continue;
        }

        std::string zone_type;
        std::getline(type_file, zone_type);
        type_file.close();

        // Look for CPU-related thermal zones
        if (zone_type.find("cpu") != std::string::npos ||
            zone_type.find("x86") != std::string::npos ||
            zone_type.find("acpi") != std::string::npos) {

            std::string temp_path = "/sys/class/thermal/thermal_zone" + std::to_string(i) + "/temp";
            std::ifstream temp_file(temp_path);
            if (temp_file.is_open()) {
                int temp_millidegrees = 0;
                temp_file >> temp_millidegrees;
                temp_file.close();

                return static_cast<float>(temp_millidegrees) / 1000.0f;
            }
        }
    }

    return -1.0f;
}

int CpuCollector::getCoreCount() {
    // Use hardware_concurrency as a reliable method
    unsigned int count = std::thread::hardware_concurrency();
    if (count > 0) {
        return static_cast<int>(count);
    }

    // Fallback: count processor entries in /proc/cpuinfo
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return 1;
    }

    int core_count = 0;
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("processor") == 0) {
            ++core_count;
        }
    }

    return core_count > 0 ? core_count : 1;
}

} // namespace platform
} // namespace resmon
