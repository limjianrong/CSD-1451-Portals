/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameState_Credits.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Tay Zhun Hang (zhunhang.tay@digipen.edu)
*
* Brief:
  This source file implements the functions used for the credits.
==================================================================================*/
#include "AEEngine.h"
#include "GameState_Credits.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"

#define NUM 4
#define DECREMENT 0.2f
extern AEGfxVertexList* square_mesh;
extern AEMtx33 scale, rotate, translate, transform;
extern AEVec2 origin;
extern s8 Albam_fontID;
credit_stat credits[NUM];
static f64 timer;

void GameStateCreditsLoad(void) {
	credits[0].texture = AEGfxTextureLoad("Assets/Digipen_flipped.png");
	credits[1].texture = AEGfxTextureLoad("Assets/Credits_1.png");
	credits[2].texture = AEGfxTextureLoad("Assets/Credits_2.png");
	credits[3].texture = AEGfxTextureLoad("Assets/Credits_3.png");
	mesh_load();
}

void GameStateCreditsInit(void) {
	AEGfxSetBackgroundColor(0, 0, 0);
	timer = 0;
	for (int i = 0; i < NUM; i++) {
		credits[i].y = origin.y - i * WINDOWLENGTH_Y;
	}
}

void GameStateCreditsUpdate(void) {

	timer += AEFrameRateControllerGetFrameTime();
	//if (timer >= 5) { 
	//	credits[0].trans -= DECREMENT;
	//	if (credits[0].trans <= 0) credits[0].flag = DISAPPEAR;
	//}

	//if (timer >= 10) {
	//	credits[1].trans -= DECREMENT;
	//	if (credits[1].trans <= 0) credits[1].flag = DISAPPEAR;
	//}

	//if (timer >= 15) {
	//	credits[2].trans -= DECREMENT;
	//	if (credits[2].trans <= 0) credits[2].flag = DISAPPEAR;
	//}

	//if (timer >= 20) {
	//	credits[3].trans -= DECREMENT;
	//	if (credits[3].trans <= 0) {
	//		credits[3].flag = DISAPPEAR;
	//		AEGfxPrint(Albam_fontID, (s8*)"Click anywhere to return to main menu", -0.3f, -0.9f, 0.5f, 1.f, 1.f, 1.f);
	//		if (AEInputCheckTriggered(AEVK_LBUTTON)) gGameStateNext = GS_MainMenu;
	//	}
	//}
	if (timer > 1) {
		for (int i = 0; i < NUM; i++) {
			credits[i].y += 1.f;
			if (credits[3].y >= (origin.y + WINDOWLENGTH_Y)) {
				AEGfxPrint(Albam_fontID, (s8*)"Click anywhere to return to main menu", -0.3f, -0.9f, 0.5f, 1.f, 1.f, 1.f);
				if (AEInputCheckTriggered(AEVK_LBUTTON)) gGameStateNext = GS_MainMenu;
			}
		}
	}

}

void GameStateCreditsDraw(void) {
	//draw_credit_4();
	//draw_credit_3();
	//draw_credit_2();
	//draw_credit_1();
	
	for (int i = 0; i < NUM; i++) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTintColor(1, 1, 1, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Trans(&translate, origin.x, credits[i].y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(credits[i].texture, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void GameStateCreditsFree(void) {

}

void GameStateCreditsUnload(void) {
	for (int i = 0; i < NUM; i++) {
		AEGfxTextureUnload(credits[i].texture);
	}
	AEGfxMeshFree(square_mesh);
	AESysFrameEnd();
}

void draw_credit_1() {
	if (credits[0].flag == APPEAR) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(credits[0].trans);
		AEGfxSetTintColor(1, 1, 1, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Trans(&translate, origin.x, origin.y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(credits[0].texture, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
void draw_credit_2() {
	if (credits[1].flag == APPEAR) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(credits[1].trans);
		AEGfxSetTintColor(1, 1, 1, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Trans(&translate, origin.x, origin.y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(credits[1].texture, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
void draw_credit_3() {
	if (credits[2].flag == APPEAR) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(credits[2].trans);
		AEGfxSetTintColor(1, 1, 1, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Trans(&translate, origin.x, origin.y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(credits[2].texture, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
void draw_credit_4() {
	if (credits[3].flag == APPEAR) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(credits[3].trans);
		AEGfxSetTintColor(1, 1, 1, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Trans(&translate, origin.x, origin.y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(credits[3].texture, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}