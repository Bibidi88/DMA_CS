#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <stdexcept>

#include "imgui.h"
#include "backend/imgui_impl_glfw.h"
#include "backend/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/player.hpp>
#include <Core/offsets.hpp>
#include <Core/client.dll.hpp>
#include <Core/camera.hpp>

struct CoreUpdater
{
public:
    uintptr_t client = 0;
    Camera GameCamera;
    std::mutex playerListMutex;
    std::vector<Player> playerList;



    void BaseThreadLoop() {
        while (true) {
            try {
                filterEntityList();
            }
            catch (const std::exception& e) {
                std::cerr << "Exception in BaseThreadLoop: " << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    }

    void UpdateThreadLoop() {
        while (true) {
            try {
                UpdatePlayerList();
                GameCamera.Update(client);
            }
            catch (const std::exception& e) {
                std::cerr << "Exception in UpdateThreadLoop: " << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void GameCameraTHreadLoop()  {
        while (true) {
            GameCamera.Update(client);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    bool Initialize() {
        try {
            while (client == 0) {
                client = mem.GetBaseDaddy("client.dll");
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }

            GrabScreenSize();

            for (int i = 0; i <= 64; i++) {
                entityList.push_back(Player(i));
            }

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Initialization failed: " << e.what() << std::endl;
            return false;
        }
    }

private:
    int ScreenWidth = 0;
    int ScreenHeight = 0;

    std::vector<Player> entityList;
    std::vector<Player> filteredPlayerList;

    std::mutex entityListMutex;

    // Define an array with all enum values for iteration


    void GrabScreenSize() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (!primaryMonitor) {
            throw std::runtime_error("Failed to get primary monitor.");
        }
        const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);
        if (!vidMode) {
            throw std::runtime_error("Failed to get video mode.");
        }
        ScreenWidth = vidMode->width;
        ScreenHeight = vidMode->height;
        GameCamera.Initialize(ScreenWidth, ScreenHeight);
    }

    void filterEntityList() {
        std::vector<Player> localPlayerList;

        uintptr_t entityList2 = mem.Read<uintptr_t>(client + client_dll::dwEntityList);
        if (entityList2 == 0) {
            throw std::runtime_error("Failed to read entity list.");
        }

        uintptr_t localPlayerPawn = mem.Read<uintptr_t>(client + client_dll::dwLocalPlayerPawn);
        if (localPlayerPawn == 0) {
            throw std::runtime_error("Failed to read local player pawn.");
        }

        auto handle = mem.CreateScatterHandle();

        for (auto& entity : entityList) {
            entity.entityList = entityList2;
            entity.localPlayerPawn = localPlayerPawn;
            entity.Read(handle);

            if (entity.pCSPlayerPawn != 0) {
                localPlayerList.push_back(entity);
               
            }
        }


        mem.CloseScatterHandle(handle);

        {
            std::lock_guard<std::mutex> lock(entityListMutex);
            filteredPlayerList = std::move(localPlayerList);
        }
    }

    void UpdatePlayerList() {
        std::vector<Player> localPlayerList;
        std::vector<Player> localUpdatedPlayerList;

        {
            std::lock_guard<std::mutex> lock(entityListMutex);
            localPlayerList = filteredPlayerList;
        }

        for (auto& player : localPlayerList) {
            player.update();
            localUpdatedPlayerList.push_back(player);
        }

        {
            std::lock_guard<std::mutex> lock(playerListMutex);
            playerList = std::move(localUpdatedPlayerList);
        }
    }
}; inline CoreUpdater Core;

