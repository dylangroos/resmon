# Implement Threshold Config UI

**Priority:** MEDIUM
**Phase:** 5 - Alert System

## Description
Settings panel for configuring alert thresholds.

## Layout
```
┌─ Settings ────────────────────┐
│ CPU Usage Warning:  [--80--]  │
│ CPU Usage Critical: [--95--]  │
│ CPU Temp Warning:   [--70--]  │
│ ...                           │
│ [Save]  [Reset to Defaults]   │
└───────────────────────────────┘
```

## Tasks
- [ ] Create settings panel (ImGui popup/window)
- [ ] Sliders for each threshold
- [ ] Enable/disable toggle per metric
- [ ] Save to config file (~/.config/resmon/config.json)
- [ ] Load on startup

## Acceptance Criteria
- All thresholds configurable
- Settings persist across restarts
- Reset to defaults works
