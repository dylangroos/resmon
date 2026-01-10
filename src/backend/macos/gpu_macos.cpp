#include "gpu_macos.h"

#ifdef __APPLE__

#include <IOKit/IOKitLib.h>
#include <IOKit/graphics/IOGraphicsLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <cstring>

namespace resmon {
namespace platform {

namespace {

// Helper to convert CFString to std::string
std::string cfStringToStdString(CFStringRef cf_string) {
    if (!cf_string) {
        return "";
    }

    CFIndex length = CFStringGetLength(cf_string);
    CFIndex max_size = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;

    std::string result(max_size, '\0');
    if (CFStringGetCString(cf_string, &result[0], max_size, kCFStringEncodingUTF8)) {
        result.resize(std::strlen(result.c_str()));
        return result;
    }

    return "";
}

// Helper to get integer property from IOKit service
uint64_t getIOKitIntProperty(io_service_t service, CFStringRef key) {
    CFTypeRef property = IORegistryEntryCreateCFProperty(service, key, kCFAllocatorDefault, 0);
    if (!property) {
        return 0;
    }

    uint64_t value = 0;
    if (CFGetTypeID(property) == CFNumberGetTypeID()) {
        CFNumberGetValue(static_cast<CFNumberRef>(property), kCFNumberSInt64Type, &value);
    }

    CFRelease(property);
    return value;
}

// Helper to get string property from IOKit service
std::string getIOKitStringProperty(io_service_t service, CFStringRef key) {
    CFTypeRef property = IORegistryEntryCreateCFProperty(service, key, kCFAllocatorDefault, 0);
    if (!property) {
        return "";
    }

    std::string result;
    if (CFGetTypeID(property) == CFStringGetTypeID()) {
        result = cfStringToStdString(static_cast<CFStringRef>(property));
    } else if (CFGetTypeID(property) == CFDataGetTypeID()) {
        // Some properties are stored as CFData (null-terminated strings)
        CFDataRef dataRef = static_cast<CFDataRef>(property);
        CFIndex length = CFDataGetLength(dataRef);
        if (length > 0) {
            result.resize(length);
            CFDataGetBytes(dataRef, CFRangeMake(0, length), reinterpret_cast<UInt8*>(&result[0]));
            // Remove null terminator if present
            size_t nullPos = result.find('\0');
            if (nullPos != std::string::npos) {
                result.resize(nullPos);
            }
        }
    }

    CFRelease(property);
    return result;
}

// Helper to get vendor-id from data property
uint32_t getVendorId(io_service_t service) {
    CFTypeRef property = IORegistryEntrySearchCFProperty(
        service,
        kIOServicePlane,
        CFSTR("vendor-id"),
        kCFAllocatorDefault,
        kIORegistryIterateRecursively | kIORegistryIterateParents
    );

    if (!property) {
        return 0;
    }

    uint32_t value = 0;
    if (CFGetTypeID(property) == CFDataGetTypeID()) {
        CFDataRef dataRef = static_cast<CFDataRef>(property);
        CFIndex length = CFDataGetLength(dataRef);
        if (length >= 2) {
            // Vendor IDs are typically stored as little-endian
            CFDataGetBytes(dataRef, CFRangeMake(0, (length < 4) ? length : 4),
                          reinterpret_cast<UInt8*>(&value));
        }
    } else if (CFGetTypeID(property) == CFNumberGetTypeID()) {
        CFNumberGetValue(static_cast<CFNumberRef>(property), kCFNumberSInt32Type, &value);
    }

    CFRelease(property);
    return value;
}

// PCI Vendor IDs
constexpr uint32_t VENDOR_ID_APPLE = 0x106B;
constexpr uint32_t VENDOR_ID_AMD = 0x1002;
constexpr uint32_t VENDOR_ID_INTEL = 0x8086;
constexpr uint32_t VENDOR_ID_NVIDIA = 0x10DE;

} // anonymous namespace

GpuCollector::GpuCollector()
    : gpus_detected_(false)
{
}

GpuCollector::~GpuCollector() {
}

std::vector<GpuMetrics> GpuCollector::collect() {
    if (!gpus_detected_) {
        detectGpus();
        gpus_detected_ = true;
    }

    std::vector<GpuMetrics> results;

    for (const auto& gpu : detected_gpus_) {
        GpuMetrics metrics;
        metrics.name = gpu.name;
        metrics.vram_total_bytes = gpu.vram_bytes;
        metrics.vram_used_bytes = 0;  // Not available without private APIs
        metrics.usage_percent = 0.0f; // Not available without private APIs
        metrics.temperature_celsius = -1.0f; // Requires SMC access

        switch (gpu.type) {
            case GpuType::AppleSilicon:
                metrics.vendor = "Apple";
                // Apple Silicon uses unified memory - VRAM is shared with system RAM
                break;

            case GpuType::AMD:
                metrics.vendor = "AMD";
                break;

            case GpuType::NVIDIA:
                metrics.vendor = "NVIDIA";
                break;

            case GpuType::Intel:
                metrics.vendor = "Intel";
                break;

            default:
                metrics.vendor = "Unknown";
                break;
        }

        results.push_back(metrics);
    }

    return results;
}

GpuMetrics GpuCollector::collectAppleSiliconGpu() {
    GpuMetrics metrics;
    metrics.name = "Apple GPU";
    metrics.vendor = "Apple";
    metrics.usage_percent = 0.0f;       // Not available without private APIs
    metrics.temperature_celsius = -1.0f; // Requires SMC access
    metrics.vram_used_bytes = 0;         // Unified memory
    metrics.vram_total_bytes = 0;        // Shared with system RAM
    return metrics;
}

GpuMetrics GpuCollector::collectAmdGpu() {
    GpuMetrics metrics;
    metrics.name = "AMD GPU";
    metrics.vendor = "AMD";
    metrics.usage_percent = 0.0f;        // Not available without private APIs
    metrics.temperature_celsius = -1.0f; // Requires SMC access
    metrics.vram_used_bytes = 0;         // Would require Metal or private APIs
    metrics.vram_total_bytes = 0;
    return metrics;
}

GpuMetrics GpuCollector::collectNvidiaGpu() {
    GpuMetrics metrics;
    metrics.name = "NVIDIA GPU";
    metrics.vendor = "NVIDIA";
    metrics.usage_percent = 0.0f;        // Not available without private APIs
    metrics.temperature_celsius = -1.0f; // Requires SMC access
    metrics.vram_used_bytes = 0;         // Would require private APIs
    metrics.vram_total_bytes = 0;
    return metrics;
}

void GpuCollector::detectGpus() {
    detected_gpus_.clear();

    // First, try to match GPU accelerators (covers most modern GPUs)
    CFMutableDictionaryRef matching = IOServiceMatching("IOAccelerator");
    if (!matching) {
        return;
    }

    io_iterator_t iterator;
    kern_return_t result = IOServiceGetMatchingServices(kIOMasterPortDefault, matching, &iterator);

    if (result != KERN_SUCCESS) {
        return;
    }

    io_service_t service;
    while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
        GpuInfo info;
        info.type = GpuType::Unknown;
        info.vram_bytes = 0;

        // Try to get GPU model name
        io_name_t name;
        if (IORegistryEntryGetName(service, name) == KERN_SUCCESS) {
            info.name = name;
        }

        // Try to get VRAM size (various property names used)
        info.vram_bytes = getIOKitIntProperty(service, CFSTR("VRAM,totalMB")) * 1024 * 1024;
        if (info.vram_bytes == 0) {
            info.vram_bytes = getIOKitIntProperty(service, CFSTR("VRAM,totalsize"));
        }

        // Try to determine GPU type from class or properties
        std::string io_class = getIOKitStringProperty(service, CFSTR("IOClass"));

        // Check for Apple Silicon (AGX class indicates Apple GPU)
        if (io_class.find("AGX") != std::string::npos ||
            info.name.find("Apple") != std::string::npos ||
            info.name.find("M1") != std::string::npos ||
            info.name.find("M2") != std::string::npos ||
            info.name.find("M3") != std::string::npos ||
            info.name.find("M4") != std::string::npos) {
            info.type = GpuType::AppleSilicon;
            // Improve name for Apple Silicon GPUs
            if (info.name.empty() || info.name == "AGXAccelerator" ||
                info.name == "AGXAcceleratorG13G" || info.name == "AGXAcceleratorG14G") {
                info.name = "Apple GPU";
            } else if (info.name.find("Apple") == std::string::npos &&
                       info.name.find("M1") == std::string::npos &&
                       info.name.find("M2") == std::string::npos &&
                       info.name.find("M3") == std::string::npos &&
                       info.name.find("M4") == std::string::npos) {
                // If name doesn't contain Apple or M-series identifier, prepend "Apple"
                info.name = "Apple " + info.name + " GPU";
            }
        } else if (io_class.find("AMD") != std::string::npos ||
                   io_class.find("ATI") != std::string::npos ||
                   info.name.find("AMD") != std::string::npos ||
                   info.name.find("Radeon") != std::string::npos) {
            info.type = GpuType::AMD;
        } else if (io_class.find("NVIDIA") != std::string::npos ||
                   io_class.find("GeForce") != std::string::npos ||
                   info.name.find("NVIDIA") != std::string::npos ||
                   info.name.find("GeForce") != std::string::npos) {
            info.type = GpuType::NVIDIA;
        } else if (io_class.find("Intel") != std::string::npos ||
                   info.name.find("Intel") != std::string::npos) {
            info.type = GpuType::Intel;
        }

        // Only add if we got a reasonable name
        if (!info.name.empty() && info.name != "IOAccelerator") {
            detected_gpus_.push_back(info);
        }

        IOObjectRelease(service);
    }

    IOObjectRelease(iterator);

    // If no GPUs found via IOAccelerator, try IOPCIDevice with display class
    if (detected_gpus_.empty()) {
        matching = IOServiceMatching("IOPCIDevice");
        if (!matching) {
            return;
        }

        result = IOServiceGetMatchingServices(kIOMasterPortDefault, matching, &iterator);
        if (result != KERN_SUCCESS) {
            return;
        }

        while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
            uint32_t class_code = static_cast<uint32_t>(getIOKitIntProperty(service, CFSTR("class-code")));

            // Check if this is a display controller (class code 0x03xxxx)
            uint8_t base_class = (class_code >> 16) & 0xFF;
            if (base_class == 0x03) {
                GpuInfo info;
                info.vram_bytes = 0;

                // Get model name
                std::string model = getIOKitStringProperty(service, CFSTR("model"));
                if (!model.empty()) {
                    info.name = model;
                }

                // Try to get VRAM
                info.vram_bytes = getIOKitIntProperty(service, CFSTR("VRAM,totalMB")) * 1024 * 1024;

                // Determine vendor from vendor-id
                uint32_t vendor_id = getVendorId(service);

                switch (vendor_id) {
                    case VENDOR_ID_APPLE:
                        info.type = GpuType::AppleSilicon;
                        if (info.name.empty()) info.name = "Apple GPU";
                        break;
                    case VENDOR_ID_AMD:
                        info.type = GpuType::AMD;
                        if (info.name.empty()) info.name = "AMD GPU";
                        break;
                    case VENDOR_ID_NVIDIA:
                        info.type = GpuType::NVIDIA;
                        if (info.name.empty()) info.name = "NVIDIA GPU";
                        break;
                    case VENDOR_ID_INTEL:
                        info.type = GpuType::Intel;
                        if (info.name.empty()) info.name = "Intel GPU";
                        break;
                    default:
                        // Also check model string for vendor hints
                        if (model.find("AMD") != std::string::npos ||
                            model.find("Radeon") != std::string::npos) {
                            info.type = GpuType::AMD;
                        } else if (model.find("NVIDIA") != std::string::npos ||
                                   model.find("GeForce") != std::string::npos) {
                            info.type = GpuType::NVIDIA;
                        } else if (model.find("Intel") != std::string::npos) {
                            info.type = GpuType::Intel;
                        } else {
                            info.type = GpuType::Unknown;
                        }
                        if (info.name.empty()) info.name = "Unknown GPU";
                        break;
                }

                if (!info.name.empty()) {
                    detected_gpus_.push_back(info);
                }
            }

            IOObjectRelease(service);
        }

        IOObjectRelease(iterator);
    }

    // Final fallback: try AGXAccelerator specifically for Apple Silicon
    if (detected_gpus_.empty()) {
        matching = IOServiceMatching("AGXAccelerator");
        if (matching) {
            result = IOServiceGetMatchingServices(kIOMasterPortDefault, matching, &iterator);
            if (result == KERN_SUCCESS) {
                while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
                    GpuInfo info;
                    info.name = "Apple GPU";
                    info.type = GpuType::AppleSilicon;
                    info.vram_bytes = 0;  // Unified memory

                    detected_gpus_.push_back(info);
                    IOObjectRelease(service);
                    break; // Only add one Apple GPU
                }
                IOObjectRelease(iterator);
            }
        }
    }
}

} // namespace platform
} // namespace resmon

#else // !__APPLE__

// Stub implementation for non-macOS platforms (for compilation testing)
namespace resmon {
namespace platform {

GpuCollector::GpuCollector() : gpus_detected_(false) {}
GpuCollector::~GpuCollector() {}

void GpuCollector::detectGpus() {}

GpuMetrics GpuCollector::collectAppleSiliconGpu() { return {}; }
GpuMetrics GpuCollector::collectAmdGpu() { return {}; }
GpuMetrics GpuCollector::collectNvidiaGpu() { return {}; }

std::vector<GpuMetrics> GpuCollector::collect() {
    return {};
}

} // namespace platform
} // namespace resmon

#endif // __APPLE__
