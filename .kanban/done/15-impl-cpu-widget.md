# Implement CPU Widget

**Priority:** HIGH
**Phase:** 4 - GUI Implementation

## Description
Create the CPU monitoring widget with usage bar and temperature.

## Layout
```
┌─ CPU ─────────────────────┐
│ Usage  [████████░░] 78%   │
│ Temp   65°C               │
└───────────────────────────┘
```

## Tasks
- [ ] Create src/gui/widgets.h/.cpp
- [ ] Implement usage progress bar
- [ ] Display temperature with unit
- [ ] Color code based on thresholds
- [ ] Optional: per-core mini bars

## Acceptance Criteria
- Updates smoothly
- Shows current values clearly
- Color indicates severity (green/yellow/red)
