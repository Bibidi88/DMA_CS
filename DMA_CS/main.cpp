// Example.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include <Windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include <Utils/features.hpp>
#include <DMALibrary/Memory/Memory.h>


#include <Gui/gui.hpp>
#include <Features/esp.hpp> 
#include <Features/Aimbot.hpp> 



#include <Core/CoreUpdater.hpp>


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
    
    gui::InitializeGLFW();
    
    
    if (Core.Initialize()) {
        std::thread baseCore(&CoreUpdater::BaseThreadLoop, &Core);
        baseCore.detach();
        std::thread updateCore(&CoreUpdater::UpdateThreadLoop, &Core);
        updateCore.detach();
        std::thread GameCamarea(&CoreUpdater::GameCameraTHreadLoop, &Core);
        GameCamarea.detach();

        
        gui::start(&esp);


    }

    return 0;
}


