# Implement Linux Intel GPU

**Priority:** LOW
**Phase:** 3 - Linux Backend

## Description
Read Intel integrated GPU stats from i915 driver.

## Approach
```bash
# Intel GPU sysfs (i915):
/sys/class/drm/card*/gt/gt0/rps_cur_freq_mhz
/sys/class/drm/card*/device/hwmon/hwmon*/temp1_input

# Or use intel_gpu_top parsing (if available)
```

## Tasks
- [ ] Detect Intel GPU via /sys/class/drm
- [ ] Read frequency info
- [ ] Read temperature if available
- [ ] Note: utilization % is harder on Intel

## Acceptance Criteria
- Detects Intel iGPU
- Shows what's available (may be limited)
- Graceful handling if no Intel GPU
