#ifndef RESMON_BACKEND_LINUX_RAM_LINUX_H
#define RESMON_BACKEND_LINUX_RAM_LINUX_H

#include "../../core/metrics.h"

namespace resmon {
namespace platform {

class RamCollector {
public:
    RamMetrics collect();

private:
    // Parse a line from /proc/meminfo and extract the value in kB
    // Returns 0 if parsing fails
    uint64_t parseMemInfoLine(const std::string& line);
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_LINUX_RAM_LINUX_H
