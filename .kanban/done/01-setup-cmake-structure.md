# Setup CMake Structure

**Priority:** HIGH
**Phase:** 1 - Project Setup

## Description
Create the foundational CMakeLists.txt with C++17 standard, project directories, and compiler flags.

## Tasks
- [ ] Create root CMakeLists.txt
- [ ] Set C++17 standard
- [ ] Configure compiler warnings (-Wall -Wextra)
- [ ] Create src/ directory structure
- [ ] Add placeholder main.cpp

## Acceptance Criteria
- `cmake -B build` succeeds
- `cmake --build build` produces executable
- Executable runs without crash
