# Setup Project Layout

**Priority:** MEDIUM
**Phase:** 1 - Project Setup

## Description
Create the directory structure for organized code.

## Structure
```
src/
  main.cpp
  core/
    metrics.h
  backend/
    linux/
    windows/
    macos/
  gui/
  alerts/
external/
  imgui/
  glfw/
```

## Tasks
- [ ] Create directory structure
- [ ] Move main.cpp to src/
- [ ] Update CMakeLists.txt include paths
- [ ] Add .gitignore

## Acceptance Criteria
- Clean separation of concerns
- Build still works after reorganization
