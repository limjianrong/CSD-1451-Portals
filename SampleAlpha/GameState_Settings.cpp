#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"


extern AEGfxVertexList* button;
extern AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;

extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;
extern f32 originX, originY;
extern AEVec2 center_cursor;
extern AEVec2 cursor;		 // Origin is CENTER of window
extern AEVec2 world_center_cursor;		 // Origin is CENTER of window
// Origin is CENTER of window

bool fullscreen = { false };

void GameStateSettingsLoad(void) {
	
}

void GameStateSettingsInit(void) {


}

void GameStateSettingsUpdate(void) {


	// full screen button pressed
	if (AEInputCheckReleased(AEVK_LBUTTON) &&
		center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
		center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 15 - WINDOWLENGTH_Y / 16 &&
		center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 15 + WINDOWLENGTH_Y / 16) {
		fullscreen = !fullscreen;
	}

	if (AEInputCheckReleased(AEVK_F)) fullscreen = !fullscreen;

	AESysToggleFullScreen(fullscreen);

	// if prev game state was main menu, return to main menu
	if (AEInputCheckReleased(AEVK_LBUTTON) &&
		center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
		center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 19 - WINDOWLENGTH_Y / 16 &&
		center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 19 + WINDOWLENGTH_Y / 16 &&
		gGameStatePrev == GS_MainMenu) {
		gGameStateNext = GS_MainMenu;
	}

	// if prev game state was game level, return to game level
	if (AEInputCheckReleased(AEVK_LBUTTON) &&
		center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
		center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 19 - WINDOWLENGTH_Y / 16 &&
		center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 19 + WINDOWLENGTH_Y / 16 &&
		gGameStatePrev == GS_Platformer) {
		gGameStateNext = GS_Platformer;
	}

}

void GameStateSettingsDraw(void) {

	//AEGfxPrint(Albam_fontID, (s8*)"Click ANYWHERE", -0.5, 0.35, 2.0f, 1, 1, 1);
	//AEGfxPrint(Albam_fontID, (s8*)"to go back to", -0.5, 0, 2.0f, 1, 1, 1);
	//AEGfxPrint(Albam_fontID, (s8*)"Main Menu!", -0.5, -0.38, 2.0f, 1, 1, 1);

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);

	// ------- Background -------
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(backgroundTex, 0, 0);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = 15; i <= 19; i += 4) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3, WINDOWLENGTH_Y / 8); // scaling it up
		AEMtx33Trans(&translate, originX, originY + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i); // x=0, start counting y from bottom edge of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16)
			AEGfxTextureSet(buttonPressed, 0, 0);
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);
	}



		AEGfxPrint(Albam_fontID, (s8*)"Full Screen", -0.2, -0.05, 0.95F, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"Back To Game", -0.3, -0.3, 0.95F, 1, 1, 1);




}
void GameStateSettingsFree() {

}

void GameStateSettingsUnload(void) {
	
}