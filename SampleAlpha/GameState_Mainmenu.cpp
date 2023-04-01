#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "Settings.hpp"
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
extern AEVec2 origin; // origin (0,0) is in middle of screen, no matter where the camera moves

// ---- Settings Menu ---- 
bool is_Settings{ FALSE }; // if TRUE, screen is currently showing settings menu

void GameStateMainmenuLoad(void) {

	// Texture load
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest_resized.png");
	
	// Settings texture load
	settings_load();

	mesh_load();
	
}

void GameStateMainmenuInit(void) {

	// Initialize settings menu variables
	settings_init();

}

void GameStateMainmenuUpdate(void) {

	variables_update();			// Updating all global variables commonly used is utmost priority

	if (is_Settings == FALSE)	// Settings menu buttons & Main Menu buttons located
								// in same locations, so collision should carry out main menu functions when is_Settings disabled
								// and settings functions when is_Settings enabled
	{
		for (s32 i = 15; i <= 27; i += 4) {
			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16) {
				if (i == 15) gGameStateNext = GS_Platformer;
				else if (i == 19) gGameStateNext = GS_Tutorial;
				else if (i == 23) is_Settings = TRUE;
				else if (i == 27) gGameStateNext = GS_QUIT;	

			}
		}
	}


}

void GameStateMainmenuDraw(void) {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, origin.x, origin.y);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(backgroundTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = 15; i <= 27 ; i+=4) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 4, WINDOWLENGTH_Y / 8); // scaling it up
		AEMtx33Trans(&translate, origin.x, origin.y+ WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i); // x=0, start counting y from bottom edge of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16)
			AEGfxTextureSet(buttonPressed, 0, 0);
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------ Texts ------
	AEGfxPrint(Albam_fontID, (s8*)"PORTALS", -0.45, 0.0, 3.0F, 1, 1, 0);
	AEGfxPrint(Albam_fontID, (s8*)"Start Game", -0.20, -0.05, 1.0f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Tutorial", -0.14, -0.3, 1.0f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Settings", -0.14, -0.56, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Quit Game", -0.18, -0.83, 0.95F, 1, 1, 1);

	if (is_Settings == TRUE) {
		settings_draw();	// Draws menu to screen
		settings_update();	// Updates collision within menu
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 6.f && center_cursor.x <= WINDOWLENGTH_X / 6.f &&
			center_cursor.y >= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 - WINDOWLENGTH_Y / 16.f &&
			center_cursor.y <= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 + WINDOWLENGTH_Y / 16.f) {
			is_Settings = FALSE;
		}
	}
}
void GameStateMainmenuFree() {
	
}

void GameStateMainmenuUnload(void) {
	// Settings Texture unload
	settings_unload(); 
	
	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	AESysFrameEnd();
}