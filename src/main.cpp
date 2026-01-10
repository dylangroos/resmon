#include <iostream>
#include <chrono>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "core/metrics.h"
#include "core/backend.h"
#include "alerts/alert_manager.h"

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << "\n";
}

// Format bytes to human-readable string
static std::string formatBytes(uint64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    while (size >= 1024.0 && unit < 4) {
        size /= 1024.0;
        unit++;
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "%.1f %s", size, units[unit]);
    return buf;
}

// Get color for progress bar based on alert severity
static ImVec4 getSeverityColor(resmon::AlertSeverity severity) {
    switch (severity) {
        case resmon::AlertSeverity::Critical:
            return ImVec4(0.9f, 0.2f, 0.2f, 1.0f);  // Red
        case resmon::AlertSeverity::Warning:
            return ImVec4(0.9f, 0.7f, 0.0f, 1.0f);  // Yellow/orange
        case resmon::AlertSeverity::None:
        default:
            return ImVec4(0.26f, 0.59f, 0.98f, 1.0f);  // Default blue
    }
}

int main() {
    // Setup GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(350, 280, "resmon", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }
    glfwSetWindowSizeLimits(window, 300, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup style - polished dark theme
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 3.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8, 8);
    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(8, 4);

    // Professional dark color scheme with blue accents
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.08f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.22f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.25f, 1.0f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.2f, 0.25f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.25f, 1.0f);

    // Setup backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Create platform backend and alert manager
    auto backend = resmon::createPlatformBackend();
    resmon::SystemMetrics metrics{};
    resmon::AlertManager alertManager;
    resmon::AlertManager::AlertState alertState;

    // Timing for 1-second updates
    auto last_update = std::chrono::steady_clock::now();
    const auto update_interval = std::chrono::seconds(1);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Update metrics every second
        auto now = std::chrono::steady_clock::now();
        if (now - last_update >= update_interval) {
            metrics = backend->collect();
            alertState = alertManager.check(metrics);
            last_update = now;
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main window (fills viewport)
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("resmon", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse);

        // Header
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.65f, 1.0f));
        ImGui::Text("RESMON");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled("v0.1.0");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // CPU Section
        if (metrics.cpu.core_count > 0) {
            ImGui::Text("CPU (%d cores)", metrics.cpu.core_count);
        } else {
            ImGui::Text("CPU");
        }
        char cpu_overlay[64];
        snprintf(cpu_overlay, sizeof(cpu_overlay), "%.1f%%", metrics.cpu.usage_percent);
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, getSeverityColor(alertState.cpu));
        ImGui::ProgressBar(metrics.cpu.usage_percent / 100.0f, ImVec2(-1, 18), cpu_overlay);
        ImGui::PopStyleColor();
        if (metrics.cpu.temperature_celsius >= 0) {
            ImGui::SameLine();
            ImGui::Text("%.0f\xC2\xB0""C", metrics.cpu.temperature_celsius);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // RAM Section
        ImGui::Text("Memory");
        char ram_overlay[64];
        snprintf(ram_overlay, sizeof(ram_overlay), "%.1f%% (%s / %s)",
            metrics.ram.usage_percent,
            formatBytes(metrics.ram.used_bytes).c_str(),
            formatBytes(metrics.ram.total_bytes).c_str());
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, getSeverityColor(alertState.ram));
        ImGui::ProgressBar(metrics.ram.usage_percent / 100.0f, ImVec2(-1, 18), ram_overlay);
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::Spacing();

        // GPU Section
        if (metrics.gpus.empty()) {
            ImGui::Text("GPU");
            ImGui::TextDisabled("No GPU detected");
        } else {
            for (size_t i = 0; i < metrics.gpus.size(); i++) {
                const auto& gpu = metrics.gpus[i];
                ImGui::Text("GPU: %s", gpu.name.c_str());
                char gpu_overlay[64];
                snprintf(gpu_overlay, sizeof(gpu_overlay), "%.1f%%", gpu.usage_percent);
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, getSeverityColor(alertState.gpu));
                ImGui::ProgressBar(gpu.usage_percent / 100.0f, ImVec2(-1, 18), gpu_overlay);
                ImGui::PopStyleColor();
                if (gpu.temperature_celsius >= 0) {
                    ImGui::SameLine();
                    ImGui::Text("%.0f\xC2\xB0""C", gpu.temperature_celsius);
                }
                if (gpu.vram_total_bytes > 0) {
                    ImGui::Text("VRAM: %s / %s",
                        formatBytes(gpu.vram_used_bytes).c_str(),
                        formatBytes(gpu.vram_total_bytes).c_str());
                }
                if (i < metrics.gpus.size() - 1) {
                    ImGui::Spacing();
                }
            }
        }

        ImGui::End();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.06f, 0.06f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
