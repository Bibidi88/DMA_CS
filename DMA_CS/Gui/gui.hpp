#pragma once

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

namespace gui {

    bool IsMenuOpened = false;
    ImDrawList* Canvas;
    ImVec4 ProcessingTimeColor;
    GLFWwindow* window;
    int ProcessingTime;
    long long StartTime;
    int SleepTime;
    int TimeLeftToSleep;
    int display_w, display_h;
    int ScreenWidth;
    int ScreenHeight;

    void GetScreenResolution(int& Width, int& Height) {
        Width = ScreenWidth;
        Height = ScreenHeight;
    }

    /*void SaveConfig(Aimbot* AimAssist, Esp* ESP, Triggerbot* Trigger) {
        if (!AimAssist->Save()) std::cout << "something went wrong trying to save Aimbot settings" << std::endl;
        if (!ESP->Save()) std::cout << "something went wrong trying to save ESP settings" << std::endl;
        if (!Trigger->Save()) std::cout << "something went wrong trying to save Triggerbot settings" << std::endl;
        UpdateConfig();
        std::cout << "Config Saved" << std::endl;
    }*/

    long long CurrentEpochMilliseconds() {
        auto currentTime = std::chrono::system_clock::now();
        auto duration = currentTime.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    void GrabScreenSize() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);
        display_w = vidMode->width;
        display_h = vidMode->height;
    }

    void InitializeGLFW() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwDefaultWindowHints();

        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_REFRESH_RATE, 240);
        std::cout << "InitializeGLFW" << std::endl;
    }

    //void renderGUI(Esp* ESP, Aimbot* AimAssist, Triggerbot* Trigger) {
    void renderGUI() {
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

        Canvas = ImGui::GetWindowDrawList();
        //ESP->RenderDrawings(Canvas, AimAssist);
        ImGui::End();

        // Toggle menu visibility with the 'M' key
        if (ImGui::IsKeyPressed(ImGuiKey_Insert))
            IsMenuOpened = !IsMenuOpened;

        if (!IsMenuOpened) return;


        ImGui::SetNextWindowSizeConstraints(ImVec2(440, 420), ImVec2(440, 420));
        ImGui::SetNextWindowSize(ImVec2(440, 420), ImGuiCond_FirstUseEver);
        ImGui::Begin("echo-dma", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

        ProcessingTimeColor = ProcessingTime > 20 ? ProcessingTimeColor = ImVec4(1, 0.343f, 0.475f, 1) : ProcessingTimeColor = ImVec4(0.4f, 1, 0.343f, 1);
        ImGui::TextColored(ProcessingTimeColor, "Processing Time: %02dms", ProcessingTime);
        ImGui::SameLine();

        if (ImGui::Button("save")) {
            //SaveConfig(AimAssist, ESP, Trigger);
        }

        if (ImGui::BeginTabBar("Menus"), ImGuiTabBarFlags_NoCloseWithMiddleMouseButton) {

            // Draw shit
            //AimAssist->RenderUI();
            //Trigger->RenderUI();
            //ESP->RenderUI();

            // Draw Credits //
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

        ImGui::SetNextWindowPos(ImVec2(window1_pos.x + window1_size.x + 10, window1_pos.y), ImGuiCond_Always); // Position neben Fenster 1
        ImGui::SetNextWindowSize(ImVec2(220, 210), ImGuiCond_FirstUseEver);
        ImGui::Begin("Overview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

        ImVec2 window2_pos = ImGui::GetWindowPos();  // Position des Fensters

        ImDrawList* CanvasOverview = ImGui::GetWindowDrawList();

        //ESP->RenderOverView(CanvasOverview, window2_pos);


        ImGui::End();
    }

    //void start(Esp* ESP, Aimbot* AimAssist, Triggerbot* Trigger) {
    void start() {
        InitializeGLFW();
        GrabScreenSize();

        GLFWwindow* window = glfwCreateWindow(display_w, display_h, "echo", glfwGetPrimaryMonitor(), NULL);
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

        std::cout << "start Visuials" << std::endl;
        // Main loop
        while (!glfwWindowShouldClose(window)) {
            StartTime = CurrentEpochMilliseconds();
            glfwPollEvents();
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.00f, 0.00f, 0.00f, 0.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            // Render ImGui GUI
           // renderGUI(ESP, AimAssist, Trigger);
            renderGUI();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            ProcessingTime = static_cast<int>(CurrentEpochMilliseconds() - StartTime);
            SleepTime = 6; // 16.67 > 60hz | 6.97 > 144hz
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

};
