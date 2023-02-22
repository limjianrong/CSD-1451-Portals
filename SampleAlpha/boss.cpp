#include "AEEngine.h"
#include <iostream>
#include "Utilities.hpp"
#include "boss.hpp"


int constexpr BOSS_WIDTH{ 200 };
int constexpr BOSS_HEIGHT{ 200 };
int constexpr BOSS_VELOCITY{ 100 };
int boss_movement{ UP };
AEGfxVertexList* boss_mesh{};
AEGfxTexture* boss_picture{};
struct boss boss;
void initialize_boss() {
	boss_mesh = create_Square_Mesh();
	boss_picture = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");
	if (!boss_picture) {
		std::cout << "Failed to load boss picture}";
	}
	boss.x = static_cast<f32>(AEGetWindowWidth());
	boss.y = static_cast<f32>(-AEGetWindowHeight() / 2 + BOSS_HEIGHT / 2);
}


void draw_boss() {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(boss_picture, 0.0f, 0.0f);
	AEMtx33Scale(&boss.scale, BOSS_WIDTH, BOSS_HEIGHT);
	AEMtx33Trans(&boss.translate, boss.x, boss.y);
	AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);
	AEGfxSetTransform(boss.matrix.m);
	AEGfxMeshDraw(boss_mesh, AE_GFX_MDM_TRIANGLES);
}

void update_boss() {

	if (boss_movement == UP) {
		boss.y += AEFrameRateControllerGetFrameTime() * BOSS_VELOCITY;
		if (boss.y+ static_cast<f32>(BOSS_HEIGHT/2) > static_cast<f32>(AEGetWindowHeight() / 2)){
		boss_movement = DOWN;
		}
	}

	if (boss_movement == DOWN) {
		boss.y -= AEFrameRateControllerGetFrameTime() * BOSS_VELOCITY;
		if (boss.y- static_cast<f32>(BOSS_HEIGHT / 2) < static_cast<f32>(-AEGetWindowHeight() / 2)) {
			boss_movement = UP;
		}
	}
	
	//u32 value = AEFrameRateControllerGetFrameCount() % 401;
	//if (value < 201) {
	//	boss.y += AEFrameRateControllerGetFrameTime() * BOSS_VELOCITY;
	//}
	//else {
	//	boss.y -= AEFrameRateControllerGetFrameTime() * BOSS_VELOCITY;
	//}


}