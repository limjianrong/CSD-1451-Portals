#include "AEEngine.h"

#include "GameState_Platformer.hpp"
#include "Utilities.hpp"
#include "boss.hpp"
#include "Player.hpp"
#include "weapon_fire.hpp"

#include <iostream>

extern Player_stats player;
Boss boss;
Laser_beam laser_beam;
int taken_damage{};

void initialize_boss() {
	laser_beam.mesh = boss.mesh = create_Square_Mesh();
	boss.standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");
	boss.deadTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/spikeBall_2.png");
	laser_beam.picture = AEGfxTextureLoad("Assets/uipack/PNG/red_button02.png");

}

void draw_boss() {

	// -------------  Boss   ---------------
	AEMtx33Scale(&boss.scale, boss.width, boss.height);
	AEMtx33Trans(&boss.translate, boss.x_pos, boss.y_pos);
	AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);
	AEGfxSetTransform(boss.matrix.m);
	AEGfxTextureSet(boss.standTex, 0.0f, 0.0f);
	AEGfxMeshDraw(boss.mesh, AE_GFX_MDM_TRIANGLES);

	// --- Boss dead ---
	if (boss.Hp <= 0) {
		AEMtx33Scale(&boss.scale, boss.width, boss.height);
		AEMtx33Trans(&boss.translate, boss.x_pos, boss.y_pos);
		AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);
		AEGfxSetTransform(boss.matrix.m);
		AEGfxTextureSet(boss.deadTex, 0.0f, 0.0f);
		AEGfxMeshDraw(boss.mesh, AE_GFX_MDM_TRIANGLES);
	}

	// -------------  Attack 1 (Laser)   ---------------
	if (laser_beam.status == TRUE) {
		AEMtx33Scale(&laser_beam.scale, laser_beam.width, laser_beam.height);
		AEMtx33Trans(&laser_beam.translate, static_cast<f32>(boss.x_pos - laser_beam.width / 2), boss.y_pos);
		AEMtx33Rot(&laser_beam.rotate, PI);
		AEMtx33Concat(&laser_beam.matrix, &laser_beam.translate, &laser_beam.scale);
		AEMtx33Concat(&laser_beam.matrix, &laser_beam.matrix, &laser_beam.rotate);
		AEGfxTextureSet(laser_beam.picture, 0.0f, 0.0f);
		AEGfxSetTransform(laser_beam.matrix.m);
		AEGfxMeshDraw(laser_beam.mesh, AE_GFX_MDM_TRIANGLES);
	}

	// -------------  Attack 2 (Bullet)   ---------------
	bullet_draw();
}

int token1{};
void update_boss() {
	//boss movement UP and DOWN
	/*if (boss.direction == UP) {
		boss.y_pos += AEFrameRateControllerGetFrameTime() * boss.velocity;
		if (boss.y_pos+ static_cast<f32>(boss.height/2) > static_cast<f32>(AEGetWindowHeight() / 2)){
		boss.direction = DOWN;
		}
	}

	if (boss.direction == DOWN) {
		boss.y_pos -= AEFrameRateControllerGetFrameTime() * boss.velocity;
		if (boss.y_pos- static_cast<f32>(boss.height / 2) < static_cast<f32>(-AEGetWindowHeight() / 2)) {
			boss.direction = UP;
		}
	}*/

	bullet_update();

	//laser_beam is firing left, use boss.x - xxx to get the center of laser beam
	AEVec2Set(&laser_beam.center, boss.x_pos-laser_beam.width/2, boss.y_pos);
	boss_laser_beam();
	//u32 value = AEFrameRateControllerGetFrameCount() % 401;
	//if (value < 201) {
	//	boss.y += AEFrameRateControllerGetFrameTime() * boss.velocity;
	//}
	//else {
	//	boss.y -= AEFrameRateControllerGetFrameTime() * boss.velocity;
	//}

	//boss attack #1
	if (AEInputCheckReleased(AEVK_1)) {
		token1 = 1;
	
	}
	if (token1 == 1) {
		boss_charge();
	}

}

int counter{};
int numberofhundreds{};

void boss_laser_beam() {

	if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
		std::cout << "\nframe is " << (numberofhundreds += 100 );
		laser_beam.status = TRUE;
		//std::cout << "\nframe count 1 is" << AEFrameRateControllerGetFrameCount();
		//std::cout << "\nlaser status true";
	}
	if (laser_beam.status == TRUE) {
		
		if (AEFrameRateControllerGetFrameCount() % 300 == 0) {
			laser_beam.status = FALSE;
			//std::cout << "\nlaser status false";
			taken_damage = 0;
			return;
			//std::cout << "\nframe count 2 is" << AEFrameRateControllerGetFrameCount();
		}

		if (AETestRectToRect(&laser_beam.center, laser_beam.width, laser_beam.height, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			//std::cout << "\ncollided";
			//std::cout << "\n health is" << player.Lives;
			if (taken_damage == 0) {
				--player.Lives;
				//std::cout << "\nacutally minused";
				taken_damage = 1;
			}
		}
	}
	
}

void boss_charge() {
	AEVec2 charge_direction{};
	AEVec2Sub(&charge_direction, &boss.center, &player.center);
	if (charge_direction.x < 0) {
		boss.x_pos -= boss.charge_velocity * AEFrameRateControllerGetFrameTime();
	}
	else {
		boss.x_pos += boss.charge_velocity * AEFrameRateControllerGetFrameTime();
	}


}