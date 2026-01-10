# Add Cross-Platform CI

**Priority:** LOW
**Phase:** 8 - Polish

## Description
GitHub Actions workflow for building on all platforms.

## Workflow
```yaml
name: Build
on: [push, pull_request]
jobs:
  build:
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
    runs-on: ${{ matrix.os }}
    steps:
      - uses: actions/checkout@v4
      - name: Configure
        run: cmake -B build
      - name: Build
        run: cmake --build build
```

## Tasks
- [ ] Create .github/workflows/build.yml
- [ ] Test on Ubuntu, Windows, macOS
- [ ] Add artifact upload for binaries
- [ ] Add release workflow

## Acceptance Criteria
- Builds pass on all 3 platforms
- Binaries available as artifacts
