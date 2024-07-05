#pragma once

#include <vector>
#include <Memory/Memory.h>
#include <Core/offsets.hpp>
#include <Core/client.dll.hpp>

struct LocalPlayer {
	uintptr_t localPlayerPawn;
	BYTE team;
	Vector3D position;

	void Read(uintptr_t client) {
		localPlayerPawn = mem.Read< uintptr_t>(client + client_dll::dwLocalPlayerPawn);
		if (localPlayerPawn == 0)
			return;

		team = mem.Read<BYTE>(this->localPlayerPawn + C_BaseEntity::m_iTeamNum);
		position = mem.Read<Vector3D>(this->localPlayerPawn + C_BasePlayerPawn::m_vOldOrigin);
	}

	bool IsValid() {
		return localPlayerPawn != 0;
	}
};