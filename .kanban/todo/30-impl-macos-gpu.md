# Implement macOS GPU

**Priority:** LOW
**Phase:** 7 - macOS Backend

## Description
GPU stats on macOS via IOKit and Metal.

## Approach
```cpp
// Discrete AMD GPU: IOKit IOAccelerator
// Apple Silicon: Unified memory, GPU is part of SoC
// Metal Performance Shaders for some stats
```

## Challenges
- Apple Silicon has unified memory (no separate VRAM)
- GPU utilization is not easily exposed
- May need to use private APIs or Activity Monitor-style queries

## Tasks
- [ ] Create src/backend/macos/gpu_macos.cpp
- [ ] Detect GPU type (Intel/AMD/Apple)
- [ ] Query available stats
- [ ] Handle limited info gracefully

## Acceptance Criteria
- Shows GPU name/type
- Shows what stats are available
- Clear indication when stats unavailable
