#ifndef RESMON_BACKEND_MACOS_RAM_MACOS_H
#define RESMON_BACKEND_MACOS_RAM_MACOS_H

#include "../../core/metrics.h"

namespace resmon {
namespace platform {

class RamCollector {
public:
    RamMetrics collect();

private:
    // Get total physical memory
    uint64_t getTotalMemory();

    // Get used memory from vm_statistics
    uint64_t getUsedMemory();
};

} // namespace platform
} // namespace resmon

#endif // RESMON_BACKEND_MACOS_RAM_MACOS_H
