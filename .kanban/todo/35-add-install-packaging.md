# Add Install/Packaging

**Priority:** LOW
**Phase:** 8 - Polish

## Description
CMake install targets and optional packaging.

## Options
- CMake install() targets
- CPack for cross-platform packaging
- AppImage for Linux
- MSI/NSIS for Windows
- DMG for macOS

## Tasks
- [ ] Add CMake install rules
- [ ] Configure CPack basics
- [ ] Optional: AppImage setup
- [ ] Optional: Windows installer
- [ ] Optional: macOS DMG

## Acceptance Criteria
- `cmake --install` works
- At least one distributable format per platform
