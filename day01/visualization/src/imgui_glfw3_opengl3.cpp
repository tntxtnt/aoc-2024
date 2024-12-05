#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <lodepng.h>
#include "constants.h"
#include "day01_visualization.h"

static void glfwErrorCallback(int error, const char* description) {
    fmt::print(std::cerr, "GLFW Error {}: {}\n", error, description);
}

float getDpiScale() {
    float xscale, yscale;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    return xscale;
}

bool buildFonts(ImGuiIO& io, float size, float scale = 1.0f) {
    io.Fonts->Clear();

    const ImWchar bmpRanges[] = {0x0001, 0xFFFF, 0};
    auto* mainFont =
        io.Fonts->AddFontFromFileTTF(kMainFontPath, size * scale, nullptr, static_cast<const ImWchar*>(bmpRanges));
    if (mainFont == nullptr) return false;

    ImFontConfig cfg;
    cfg.MergeMode = true;
    auto* japaneseFont =
        io.Fonts->AddFontFromFileTTF(kJapaneseFontPath, size * scale, &cfg, io.Fonts->GetGlyphRangesJapanese());
    if (japaneseFont == nullptr) return false;

    io.Fonts->Build();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    return true;
}

int main(int, char**) {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) return 1;

    const char* glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac

    // Load window icon
    std::vector<unsigned char> windowIconBuff;
    GLFWimage windowIcon;
    lodepng::decode(windowIconBuff, (unsigned int&)windowIcon.width, (unsigned int&)windowIcon.height,
                    std::string{kWindowIconFilePath});
    windowIcon.pixels = &windowIconBuff[0];

    // Create window with graphics context and center it
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window =
        glfwCreateWindow(kWindowW, kWindowH, "Advent of Code 2024 - Day 1 - Visualization", nullptr, nullptr);
    glfwDefaultWindowHints();
    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int monitorX;
    int monitorY;
    glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);
    glfwSetWindowPos(window, monitorX + (videoMode->width - kWindowW) / 2,
                     monitorY + (videoMode->height - kWindowH) / 2);
    glfwSetWindowIcon(window, 1, &windowIcon);
    glfwShowWindow(window);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    // Load Fonts
    float dpiScale = getDpiScale();
    fmt::println("DPI scale: {}", dpiScale);
    if (!buildFonts(io, kFontSize, dpiScale)) return 2;

    // Don't create or load from imgui.ini file
    io.IniFilename = nullptr;

    Day1Visualization day1Viz;

    bool isImMode = false;
    const ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.).
        if (isImMode) {
            glfwPollEvents();
        } else if (io.WantTextInput) {
            glfwWaitEventsTimeout(1.0 / kTextInputFps);
        } else {
            glfwWaitEvents();
        }

        // Check for DPI
        float currentDpiScale = getDpiScale();
        if (dpiScale != currentDpiScale) {
            dpiScale = currentDpiScale;
            buildFonts(io, kFontSize, dpiScale);
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Custom UI
        day1Viz.draw();

        // Rendering
        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
