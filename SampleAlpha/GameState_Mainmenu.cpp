#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"
//#include <iostream>

extern s8 Albam_fontID;
AEGfxVertexList* button;
extern AEMtx33 scale, rotate, translate, transform;


void GameStateMainmenuLoad(void) {
	button = create_Square_Mesh();
	//fontID = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
}

void GameStateMainmenuInit(void) {
	

}

void GameStateMainmenuUpdate(void) {

	// ------ Start game button ------
	if (AEInputCheckCurr(AEVK_LBUTTON) && get_cursor_center_position().x >= -150 && get_cursor_center_position().x <= 150
		&& get_cursor_center_position().y >= -40 && get_cursor_center_position().y <= 60) {
		gGameStateNext = GS_Platformer;
	}
	// ------ Quit game button ------
	if (AEInputCheckCurr(AEVK_LBUTTON) &&
		(!AEInputCheckReleased(AEVK_LBUTTON)) && get_cursor_center_position().x >= -150 && get_cursor_center_position().x <= 150
		&& get_cursor_center_position().y >= -150 && get_cursor_center_position().y <= -50) {
		//gGameStateNext = GS_QUIT;
		//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	}

}

void GameStateMainmenuDraw(void) {

	// ------ Start game button ------
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTintColor(0, 0, 0, 1.0f);
	AEMtx33Scale(&scale, 300, 100); // scaling it up
	AEMtx33Trans(&translate, 0, 10); // shifts along x & y axis
	AEMtx33Rot(&rotate, 0); // rotation
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);
	// ------ Quit game button ------
	AEMtx33Scale(&scale, 300, 100);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Trans(&translate, 0, -100);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);

	// ------ Texts ------
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Albam_fontID, (s8*)"PORTALS", -0.3, 0.4, 1.5F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Start Game", -0.25, 0, 1.0F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Quit Game", -0.26, -0.4, 1.0F, 1, 1, 1);
}
void GameStateMainmenuFree() {
	
}

void GameStateMainmenuUnload(void) {
	//AEGfxDestroyFont(fontID);
	AESysFrameEnd();
}
