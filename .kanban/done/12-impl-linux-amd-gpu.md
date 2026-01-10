# Implement Linux AMD GPU

**Priority:** MEDIUM
**Phase:** 3 - Linux Backend

## Description
Read AMD GPU stats from sysfs (amdgpu driver).

## Approach
```bash
# AMD GPU sysfs paths:
/sys/class/drm/card*/device/gpu_busy_percent
/sys/class/drm/card*/device/hwmon/hwmon*/temp1_input
/sys/class/drm/card*/device/mem_info_vram_used
/sys/class/drm/card*/device/mem_info_vram_total
```

## Tasks
- [ ] Scan /sys/class/drm for AMD cards
- [ ] Read gpu_busy_percent for utilization
- [ ] Read temperature from hwmon
- [ ] Read VRAM usage
- [ ] Handle APUs vs discrete GPUs

## Acceptance Criteria
- Works with RDNA/RDNA2/RDNA3 GPUs
- Works with Ryzen APUs
- Returns empty if no AMD GPU
