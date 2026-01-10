cmake_minimum_required(VERSION 3.16)
project(resmon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# ============================================================================
# Dependencies via FetchContent
# ============================================================================
include(FetchContent)

# GLFW - Window/Input/OpenGL context
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Dear ImGui - GUI
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.6
)
FetchContent_MakeAvailable(imgui)

# Build ImGui as a library
add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    # Backends
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_lib PUBLIC glfw)

# OpenGL
find_package(OpenGL REQUIRED)

# ============================================================================
# Main Application
# ============================================================================
set(CORE_SOURCES
    src/core/metrics.h
    src/core/backend.h
    src/core/alerts.h
)

# Platform-specific backend sources
if(UNIX AND NOT APPLE)
    set(BACKEND_SOURCES
        src/backend/linux/cpu_linux.cpp
        src/backend/linux/ram_linux.cpp
        src/backend/linux/gpu_nvidia.cpp
        src/backend/linux/gpu_amd.cpp
        src/backend/linux/gpu_intel.cpp
        src/backend/linux/linux_backend.cpp
    )
elseif(WIN32)
    set(BACKEND_SOURCES
        # Windows backend files will go here
    )
elseif(APPLE)
    set(BACKEND_SOURCES
        # macOS backend files will go here
    )
endif()

set(ALERT_SOURCES
    src/alerts/alert_manager.cpp
)

set(SOURCES
    src/main.cpp
    ${CORE_SOURCES}
    ${BACKEND_SOURCES}
    ${ALERT_SOURCES}
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui_lib
    OpenGL::GL
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESMON_WINDOWS)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESMON_MACOS)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa" "-framework IOKit")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESMON_LINUX)
    # Linux needs dl for dlopen (NVML runtime loading)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
    find_package(X11 QUIET)
    if(X11_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    endif()
endif()
