#include "AEEngine.h"
#include <iostream>
#include "Utilities.hpp"
#include "boss.hpp"
#include "Player.hpp"

extern Player_stats player;
struct boss boss;
struct laser_beam laser_beam;
int taken_damage{};

void initialize_boss() {
	laser_beam.mesh = boss.mesh = create_Square_Mesh();
	boss.picture= AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");
	if (!boss.picture) {
		std::cout << "Failed to load boss picture}";
	}
	boss.x = static_cast<f32>(AEGetWindowWidth()+300.0f);
	boss.y = static_cast<f32>(-AEGetWindowHeight() / 2 + boss.height/2);

}

void draw_boss() {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(boss.picture, 0.0f, 0.0f);
	AEMtx33Scale(&boss.scale, boss.width, boss.height);
	AEMtx33Trans(&boss.translate, boss.x, boss.y);
	AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);
	AEGfxSetTransform(boss.matrix.m);
	AEGfxMeshDraw(boss.mesh, AE_GFX_MDM_TRIANGLES);
}

void update_boss() {
	//boss movement UP and DOWN
	if (boss.direction == UP) {
		boss.y += AEFrameRateControllerGetFrameTime() * boss.velocity;
		if (boss.y+ static_cast<f32>(boss.height/2) > static_cast<f32>(AEGetWindowHeight() / 2)){
		boss.direction = DOWN;
		}
	}

	if (boss.direction == DOWN) {
		boss.y -= AEFrameRateControllerGetFrameTime() * boss.velocity;
		if (boss.y- static_cast<f32>(boss.height / 2) < static_cast<f32>(-AEGetWindowHeight() / 2)) {
			boss.direction = UP;
		}
	}
	//laser_beam is firing left, use boss.x - xxx to get the center of laser beam
	AEVec2Set(&laser_beam.center, boss.x-laser_beam.width/2, boss.y);
	boss_laser_beam();
	//u32 value = AEFrameRateControllerGetFrameCount() % 401;
	//if (value < 201) {
	//	boss.y += AEFrameRateControllerGetFrameTime() * boss.velocity;
	//}
	//else {
	//	boss.y -= AEFrameRateControllerGetFrameTime() * boss.velocity;
	//}

	//boss attack #1

}

void boss_laser_beam() {

	AEMtx33Scale(&laser_beam.scale, laser_beam.width, laser_beam.height);
	AEMtx33Trans(&laser_beam.translate, static_cast<f32>(boss.x-laser_beam.width/2), boss.y);
	AEMtx33Rot(&laser_beam.rotate, PI);
	AEMtx33Concat(&laser_beam.matrix, &laser_beam.translate, &laser_beam.scale);
	AEMtx33Concat(&laser_beam.matrix, &laser_beam.matrix, &laser_beam.rotate);
	if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
		laser_beam.status = TRUE;
		//std::cout << "\nframe count 1 is" << AEFrameRateControllerGetFrameCount();
	}
	if (laser_beam.status == TRUE) {
		
		AEGfxSetTransform(laser_beam.matrix.m);
		AEGfxMeshDraw(laser_beam.mesh, AE_GFX_MDM_TRIANGLES);
		if (AEFrameRateControllerGetFrameCount() % 300 == 0) {
			laser_beam.status = FALSE;
			taken_damage = 0;
			//std::cout << "\nframe count 2 is" << AEFrameRateControllerGetFrameCount();
		}

		if (AETestRectToRect(&laser_beam.center, laser_beam.width, laser_beam.height, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			//std::cout << "\ncollided";
			std::cout << "\n health is" << player.Lives;
			if (taken_damage == 0) {
				--player.Lives;
				taken_damage = 1;
			}
		}
	}

	

}