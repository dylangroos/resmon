# Add ImGui and GLFW Dependencies

**Priority:** HIGH
**Phase:** 1 - Project Setup

## Description
Integrate Dear ImGui and GLFW as dependencies via git submodules or FetchContent.

## Tasks
- [ ] Add GLFW (submodule or FetchContent)
- [ ] Add Dear ImGui source files
- [ ] Add ImGui GLFW+OpenGL3 backend
- [ ] Update CMakeLists.txt to build dependencies
- [ ] Link against OpenGL

## Acceptance Criteria
- Dependencies download/build automatically
- No manual library installation required
- Builds on Linux with standard dev tools
