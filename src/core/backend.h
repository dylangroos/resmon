#ifndef RESMON_CORE_BACKEND_H
#define RESMON_CORE_BACKEND_H

#include <memory>

#include "metrics.h"

namespace resmon {

class IMetricsBackend {
public:
    virtual ~IMetricsBackend() = default;
    virtual SystemMetrics collect() = 0;
};

std::unique_ptr<IMetricsBackend> createPlatformBackend();

} // namespace resmon

#endif // RESMON_CORE_BACKEND_H
