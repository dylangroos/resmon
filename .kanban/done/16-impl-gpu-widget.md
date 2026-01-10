# Implement GPU Widget

**Priority:** HIGH
**Phase:** 4 - GUI Implementation

## Description
Create the GPU monitoring widget, supporting multiple GPUs.

## Layout
```
┌─ GPU: RTX 3080 ───────────┐
│ Usage  [██████░░░░] 62%   │
│ Temp   71°C               │
│ VRAM   6.2 / 10 GB        │
└───────────────────────────┘
```

## Tasks
- [ ] Display GPU name and vendor
- [ ] Usage progress bar
- [ ] Temperature display
- [ ] VRAM bar (used/total)
- [ ] Handle multiple GPUs (tabs or stacked)
- [ ] Handle "no GPU detected" state

## Acceptance Criteria
- Shows all detected GPUs
- VRAM displayed in human-readable format
- Graceful when no GPU found
