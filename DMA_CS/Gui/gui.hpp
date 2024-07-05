#pragma once

#include <chrono>
#include <iostream>
#include "Features/esp.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backend/imgui_impl_glfw.h>
#include <backend/imgui_impl_opengl3.h>

namespace gui {
    // Definition of variables
    bool IsMenuOpened = false;
    GLFWwindow* window = nullptr;
    const GLFWvidmode* vidMode = nullptr;
    int ProcessingTime = 0;
    long long StartTime = 0;
    int SleepTime = 0;
    int TimeLeftToSleep = 0;
    int display_w = 0, display_h = 0;
    int ScreenWidth = 0;
    int ScreenHeight = 0;

    void GetScreenResolution(int& Width, int& Height) {
        Width = ScreenWidth;
        Height = ScreenHeight;
    }

    long long CurrentEpochMilliseconds() {
        // Get current time in milliseconds since epoch
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void GrabScreenSize() {
        // Get screen resolution using GLFW
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        vidMode = glfwGetVideoMode(primaryMonitor);
        display_w = vidMode->width;
        display_h = vidMode->height;
    }

    void InitializeGLFW() {
        // Initialize GLFW with appropriate hints
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Additional GLFW window hints for desired behavior
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_REFRESH_RATE, 240); // Adjust refresh rate as needed

        // Swap interval for synchronization with monitor's refresh rate
        glfwSwapInterval(1);

        std::cout << "InitializeGLFW" << std::endl;
    }

    void renderGUI(ESP *esp) {
        // ImGui rendering for overlay and menus
        auto io = ImGui::GetIO();
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("##Overlay", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoBackground
        );

        ImDrawList* Canvas = ImGui::GetWindowDrawList();
        esp->RenderDrawings(Canvas);
        ImGui::End();

        // Toggle menu visibility with the 'Insert' key
        if (ImGui::IsKeyPressed(ImGuiKey_Insert))
            IsMenuOpened = !IsMenuOpened;

        if (!IsMenuOpened) return;

        ImGui::SetNextWindowSizeConstraints(ImVec2(440, 420), ImVec2(440, 420));
        ImGui::SetNextWindowSize(ImVec2(440, 420), ImGuiCond_FirstUseEver);
        ImGui::Begin("echo-dma", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

        // Determine color based on processing time
        ImVec4 ProcessingTimeColor = ProcessingTime > 20 ? ImVec4(1, 0.343f, 0.475f, 1) : ImVec4(0.4f, 1, 0.343f, 1);
        ImGui::TextColored(ProcessingTimeColor, "Processing Time: %02dms", ProcessingTime);
        ImGui::SameLine();

        // Example button to save configuration
        if (ImGui::Button("save")) {
            // SaveConfig(AimAssist, ESP, Trigger); // Uncomment to implement save functionality
        }

        if (ImGui::BeginTabBar("Menus"), ImGuiTabBarFlags_NoCloseWithMiddleMouseButton) {
            // Render UI elements for various tabs
            esp->RenderUI(); // Example: Render UI for ESP

            // Draw Credits tab
            if (ImGui::BeginTabItem("Credits", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
                ImGui::TextColored(ImVec4(0.65f, 1, 0.95f, 1), "Credits:");
                ImGui::TextColored(ImVec4(0.6f, 1, 0.64f, 1), "Made by me");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImVec2 window1_pos = ImGui::GetWindowPos();
        ImVec2 window1_size = ImGui::GetWindowSize();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(window1_pos.x + window1_size.x + 10, window1_pos.y), ImGuiCond_Always); // Position next to window 1
        ImGui::SetNextWindowSize(ImVec2(220, 210), ImGuiCond_FirstUseEver);
        ImGui::Begin("Overview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

        ImVec2 window2_pos = ImGui::GetWindowPos();  // Window position

        ImDrawList* CanvasOverview = ImGui::GetWindowDrawList();
        esp->RenderOverView(CanvasOverview, window2_pos);

        ImGui::End();
    }

    void start(ESP *esp) {
        // Initialize GLFW and grab screen size
        
        GrabScreenSize();

        // Create GLFW window and set context
        window = glfwCreateWindow(display_w, display_h, "echo", glfwGetPrimaryMonitor(), NULL);
        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        std::cout << "start Visuals" << std::endl;

        // Main loop
        while (!glfwWindowShouldClose(window)) {
            StartTime = CurrentEpochMilliseconds();
            glfwPollEvents();
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.00f, 0.00f, 0.00f, 0.00f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Render ImGui GUI
            renderGUI(esp);

            // Render ImGui and swap buffers
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            // Limit frame rate to monitor refresh rate
            ProcessingTime = static_cast<int>(CurrentEpochMilliseconds() - StartTime);
            SleepTime = 1000 / vidMode->refreshRate;
            TimeLeftToSleep = max(0, SleepTime - ProcessingTime);
            std::this_thread::sleep_for(std::chrono::milliseconds(TimeLeftToSleep));
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
