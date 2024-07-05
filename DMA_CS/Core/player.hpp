#pragma once
#include <iostream>
#include <vector>

#include <Core/client.dll.hpp>
#include <Core/offsets.hpp>
#include "Math/Vector3D.hpp"
#include "Memory/Memory.h"
#include "Utils/hitboxType.hpp"




class Player
{
public:
	int health;
	int team;
	int index;
	bool spotted;
	uintptr_t entityList;
	uintptr_t localPlayerPawn;
	uintptr_t boneMatrix;
	Vector3D position;
	Vector3D viewOffset;
	std::vector<Vector3D> boneArray;

	uintptr_t pCSPlayerPawn = 0;

	Player(int index) : index(index) {}

	void Read(auto handle) {
		try {
			uintptr_t entityController;
			mem.AddScatterReadRequest(handle, entityList + ((8 * (index & 0x7ff) >> 9) + 16), &entityController, sizeof(entityController));
			mem.ExecuteReadScatter(handle);
			if (entityController == 0) {
				pCSPlayerPawn = 0;
				return;
			}

			uintptr_t entityControllerPawn;
			mem.AddScatterReadRequest(handle, entityController + 120 * (index & 0x1ff), &entityControllerPawn, sizeof(entityControllerPawn));
			mem.ExecuteReadScatter(handle);
			if (entityControllerPawn == 0) {
				pCSPlayerPawn = 0;
				return;
			}

			uintptr_t playerPawn;
			mem.AddScatterReadRequest(handle, entityControllerPawn + CBasePlayerController::m_hPawn, &playerPawn, sizeof(playerPawn));
			mem.ExecuteReadScatter(handle);
			//uint32_t playerPawn = mem.Read<uint32_t>(player + CBasePlayerController::m_hPawn);
			if (playerPawn == 0) {
				pCSPlayerPawn = 0;
				return;
			}

			uintptr_t listEntry2;
			mem.AddScatterReadRequest(handle, entityList + 8 * ((playerPawn & 0x7fff) >> 9) + 16, &listEntry2, sizeof(listEntry2));
			mem.ExecuteReadScatter(handle);
			// uintptr_t listEntry2 = mem.Read<uintptr_t>(entityList + 8 * ((playerPawn & 0x7fff) >> 9) + 16);
			if (listEntry2 == 0) {
				pCSPlayerPawn = 0;
				return;
			}

			mem.AddScatterReadRequest(handle, listEntry2 + 120 * (playerPawn & 0x1ff), &this->pCSPlayerPawn, sizeof(this->pCSPlayerPawn));
			mem.ExecuteReadScatter(handle);
			//uintptr_t pCsPlayerPawn = mem.Read<uintptr_t>(listEntry2 + 120 * (playerPawn & 0x1ff));
			if (this->pCSPlayerPawn == 0 || this->localPlayerPawn == this->pCSPlayerPawn) {
				pCSPlayerPawn = 0;
				return;
			}

			mem.AddScatterReadRequest(handle, this->pCSPlayerPawn + C_BaseEntity::m_iTeamNum, &this->team, sizeof(this->team));
			mem.ExecuteReadScatter(handle);

			/*BYTE localPLayerTeam;
			mem.AddScatterReadRequest(handle, this->localPlayerPawn + C_BaseEntity::m_iTeamNum, &localPLayerTeam, sizeof(localPLayerTeam));
			mem.ExecuteReadScatter(handle);
			if(this->team == localPLayerTeam && !Features::Esp::Teammate::DrawBoxes) {
				pCSPlayerPawn = 0;
				return;
			}*/

			uintptr_t sceneNode;
			mem.AddScatterReadRequest(handle, this->pCSPlayerPawn + C_BaseEntity::m_pGameSceneNode, &sceneNode, sizeof(sceneNode));
			mem.ExecuteReadScatter(handle);

			mem.AddScatterReadRequest(handle, sceneNode + CSkeletonInstance::m_modelState + 0x80, &this->boneMatrix, sizeof(this->boneMatrix));
			mem.ExecuteReadScatter(handle);

		}
		catch (const std::exception& e) {
			std::cerr << "Error reading entity data: " << e.what() << std::endl;
		}
	}

	struct KeyValuePair {
		std::string key;
		std::string value;
	};

	void update() {
		health = mem.Read<int>(this->pCSPlayerPawn + C_BaseEntity::m_iHealth);
		spotted = mem.Read<int>(this->pCSPlayerPawn + C_CSPlayerPawn::m_entitySpottedState + EntitySpottedState_t::m_bSpotted);
		position = mem.Read<Vector3D>(this->pCSPlayerPawn + C_BasePlayerPawn::m_vOldOrigin);

		boneArray = {
			getBonePosition(HitboxType::Waist),
			getBonePosition(HitboxType::Head),
			getBonePosition(HitboxType::Neck),
			getBonePosition(HitboxType::Leftshoulder),
			getBonePosition(HitboxType::Leftelbow),
			getBonePosition(HitboxType::Lefthand),
			getBonePosition(HitboxType::Rightshoulder),
			getBonePosition(HitboxType::RightelbowBone),
			getBonePosition(HitboxType::Righthand),
			getBonePosition(HitboxType::Leftknees),
			getBonePosition(HitboxType::Leftleg),
			getBonePosition(HitboxType::Rightknees),
			getBonePosition(HitboxType::Rightleg)
		};
	}

	bool isAlive() {
		return this->health > 0 && this->health <= 100;
	}

	Vector3D getBonePosition(HitboxType HitBox) {
		return mem.Read<Vector3D>(this->boneMatrix + static_cast<int>(HitBox) * 32);
	}
};