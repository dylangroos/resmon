# Implement Main Window Layout

**Priority:** HIGH
**Phase:** 4 - GUI Implementation

## Description
Arrange all widgets in the main window with polling loop.

## Layout
```
┌─────────────────────────────────┐
│           resmon                │
├─────────────────────────────────┤
│  ┌─ CPU ─────┐  ┌─ RAM ─────┐  │
│  │ ...       │  │ ...       │  │
│  └───────────┘  └───────────┘  │
│  ┌─ GPU ─────────────────────┐  │
│  │ ...                       │  │
│  └───────────────────────────┘  │
│                    [Settings]   │
└─────────────────────────────────┘
```

## Tasks
- [ ] Create src/gui/app.h/.cpp
- [ ] Setup ImGui window (no decoration, fills GLFW window)
- [ ] Arrange widgets using ImGui columns/tables
- [ ] Implement 1-second polling timer
- [ ] Add settings button (opens config panel)
- [ ] Handle window resize

## Acceptance Criteria
- Clean layout, no wasted space
- Updates every 1 second
- Responsive to window resize
