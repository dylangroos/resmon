# Implement Windows AMD/Intel GPU

**Priority:** LOW
**Phase:** 6 - Windows Backend

## Description
AMD ADL and Intel APIs for non-NVIDIA GPUs on Windows.

## Approach
```cpp
// AMD: ADL (AMD Display Library)
ADL_Main_Control_Create();
ADL_Adapter_NumberOfAdapters_Get(&numAdapters);
ADL_Overdrive_Temperature_Get();

// Intel: IGCL or DirectX queries
```

## Tasks
- [ ] AMD ADL integration (optional DLL)
- [ ] Intel GPU detection
- [ ] Basic stats where available
- [ ] Graceful fallback

## Acceptance Criteria
- Detects AMD/Intel GPUs
- Shows available stats
- No crash if libraries missing
