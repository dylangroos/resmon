# Implement Visual Alerts

**Priority:** MEDIUM
**Phase:** 5 - Alert System

## Description
Visual indicators in widgets when thresholds exceeded.

## Design
- Normal: Default theme colors
- Warning: Yellow/orange tint on progress bar
- Critical: Red tint, possibly pulsing

## Tasks
- [ ] Add severity parameter to widget render functions
- [ ] Change progress bar color based on severity
- [ ] Optional: Add warning icon next to value
- [ ] Optional: Subtle pulse animation for critical

## Acceptance Criteria
- Clear visual distinction between states
- Not distracting during normal operation
- Immediately noticeable when critical
