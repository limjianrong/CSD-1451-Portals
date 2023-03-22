#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"
#include <iostream>

extern s8 Albam_fontID;
//AEGfxVertexList* button;
extern AEMtx33 scale, rotate, translate, transform;
static AEGfxTexture* buttonNotPressed, * buttonPressed;

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
// ----- Background -----
static AEGfxTexture* backgroundTex;

// ----- Cursor positions -----
extern AEVec2 cursor;				 // Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;		 // Origin is CENTER of window
extern AEVec2 world_center_cursor;  // Origin is CENTER of window

// Maybe removed soon if bug fixed (WIP: Restart game & Set camera to default when BACK TO MAIN MENU)
extern f32 originX, originY; // origin (0,0) is in middle of screen, no matter where the camera moves

void GameStateMainmenuLoad(void) {
	// Texture load
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");

	mesh_load();
	
}

void GameStateMainmenuInit(void) {
	

}

void GameStateMainmenuUpdate(void) {

	variables_update();  // Updating all global variables commonly used is utmost priority
	
	for (s32 i = 15; i <= 27; i += 4) {
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16) {
			if (i == 15) gGameStateNext = GS_Platformer;
			else if (i == 19) gGameStateNext = GS_Tutorial;
			else if (i == 23) gGameStateNext = GS_Settings;
			else if (i == 27) gGameStateNext = GS_QUIT;
		}
	}

	//// ------ Start game button ------
	//if (AEInputCheckReleased(AEVK_LBUTTON) && 
	//	center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
	//	center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 15 - WINDOWLENGTH_Y / 16 &&
	//	center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 15 + WINDOWLENGTH_Y / 16) {
	//	gGameStateNext = GS_Platformer;
	//}
	//// ------ Tutorial button ------
	//if (AEInputCheckReleased(AEVK_LBUTTON) &&
	//	center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
	//	center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 19 - WINDOWLENGTH_Y / 16 &&
	//	center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 19 + WINDOWLENGTH_Y / 16) {
	//	//gGameStateNext = GS_Tutorial;
	//	std::cout << "TUTORIAL NOT YET" << std::endl;
	//}
	//// ------ Settings button ------
	//if (AEInputCheckReleased(AEVK_LBUTTON) &&
	//	center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
	//	center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 23 - WINDOWLENGTH_Y / 16 &&
	//	center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 23 + WINDOWLENGTH_Y / 16) {
	//	gGameStateNext = GS_Settings;
	//}
	//// ------ Quit game button ------
	//if (AEInputCheckReleased(AEVK_LBUTTON) && 
	//	center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
	//	center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 27 - WINDOWLENGTH_Y / 16 &&
	//	center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 27 + WINDOWLENGTH_Y / 16) {
	//	gGameStateNext = GS_QUIT;
	//}


}

void GameStateMainmenuDraw(void) {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(backgroundTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = 15; i <= 27 ; i+=4) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3, WINDOWLENGTH_Y / 8); // scaling it up
		AEMtx33Trans(&translate, originX, originY+ WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i); // x=0, start counting y from bottom edge of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16)
			AEGfxTextureSet(buttonPressed, 0, 0);
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------ Texts ------
	AEGfxPrint(Albam_fontID, (s8*)"PORTALS", -0.55, 0.4, 3.0F, 1, 1, 0);
	AEGfxPrint(Albam_fontID, (s8*)"Start Game", -0.25, -0.05, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Tutorial", -0.19, -0.3, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Settings", -0.19, -0.56, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Quit Game", -0.24, -0.83, 0.95F, 1, 1, 1);
}
void GameStateMainmenuFree() {
	
}

void GameStateMainmenuUnload(void) {

	
	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	AESysFrameEnd();
}
