#include "../../core/backend.h"
#include "macos_backend.h"

namespace resmon {

std::unique_ptr<IMetricsBackend> createPlatformBackend() {
    return std::make_unique<platform::MacOSBackend>();
}

} // namespace resmon
