# Optimize Performance

**Priority:** MEDIUM
**Phase:** 8 - Polish

## Description
Ensure resmon stays lightweight (<1% CPU, <50MB RAM).

## Areas to Check
- File reading efficiency (cache file handles?)
- ImGui render overhead
- Polling frequency tuning
- Memory allocations per frame

## Tasks
- [ ] Profile CPU usage while running
- [ ] Profile memory usage
- [ ] Optimize hot paths
- [ ] Consider async metric collection
- [ ] Test with Valgrind for leaks

## Acceptance Criteria
- <1% CPU usage at 1-second polling
- <50MB RAM baseline
- No memory leaks
- Responsive UI (60fps)
