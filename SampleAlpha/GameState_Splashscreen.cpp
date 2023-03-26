/******************************************************************************//*!
\file		GameState_Splashscreen.cpp
\author 	Digipen, Tay Zhun Hang
\par    	email: zhunhang.tay@digipen.edu
\date   	26 March, 2023
\brief		

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"
#include <iostream>

AEGfxTexture* logo;
extern AEGfxVertexList* square_mesh;
extern AEMtx33 scale, rotate, translate, transform;
extern f32 originX, originY; 
f32 transparancy;
f64 timer;

void GameStateSplashscreenLoad(void) {
	mesh_load();
	logo = AEGfxTextureLoad("Assets/Digipen_flipped.png");

}
void GameStateSplashscreenInit(void) {
	timer = 0;
	transparancy = 1.0f;
}

void GameStateSplashscreenUpdate(void) {

	timer += AEFrameRateControllerGetFrameTime();

	if (timer >= 2) {
		transparancy -= 0.1f;
		if(transparancy < 0.f) gGameStateNext = GS_MainMenu;
	}

}

void GameStateSplashscreenDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(transparancy);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(logo, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}
void GameStateSplashscreenFree(void) {

}
void GameStateSplashscreenUnload(void) {
	AEGfxTextureUnload(logo);
	AEGfxMeshFree(square_mesh);
	AESysFrameEnd();
}
