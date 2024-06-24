// Example.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include <Windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include <DMALibrary/Memory/Memory.h>

#include "imgui.h"
#include "backend/imgui_impl_glfw.h"
#include "backend/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Gui/gui.hpp>
#include <Features/esp.hpp> 
#include <atomic>

std::atomic<bool> running(true);

ESP* esp = new ESP();

int main(int argc, char* argv[]) {

    // Initialize DMA
    while (!mem.Init("cs2.exe", true, false)) {
        std::cerr << "waiting to initialize DMA" << std::endl;
        Sleep(1);
    }
    std::cout << "DMA initialized" << std::endl;

    if (!mem.GetKeyboard()->InitKeyboard()) {
        std::cerr << "Failed to initialize keyboard hotkeys through kernel." << std::endl;
        std::cerr << "Press ENTER to continue...";
        std::cin.get();
        return -1;
    }

    uintptr_t BasePointer = mem.GetBaseDaddy("client.dll");
    if (!BasePointer) {
        std::cerr << "Failed to get client base address" << std::endl;
        return -1;
    }
    std::cout << "Client base address: " << BasePointer << std::endl;

   // std::thread update(updateCore, &esp, BasePointer);
    //std::thread update(updateCore);
    //update.detach();
    //std::thread gui(startGui);
    //gui.detach();


    gui::start(esp, BasePointer);



    // Join threads (or add a mechanism to stop them gracefully)
    //update.join();
    //gui.join();

    return 0;
}


