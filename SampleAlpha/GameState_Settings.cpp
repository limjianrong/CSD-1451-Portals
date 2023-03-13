#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "GameState_Settings.hpp"


// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;

AEGfxTexture* volume_bar, * volume_button;

extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;
extern f32 originX, originY;
extern AEVec2 center_cursor;

bool fullscreen = { false };
AEVec2 vbutton, vbar;
float buttonscalex, buttonscaley;
float barscalex, barscaley;

void GameStateSettingsLoad(void) {

	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");

	volume_bar = AEGfxTextureLoad("Assets/volume_bar.png");
	volume_button = AEGfxTextureLoad("Assets/slider_button.png");

	// Loads fontID into memory
	//Albam_fontID = AEGfxCreateFont("Assets/Albam.ttf", 50);
	mesh_load();
}


void GameStateSettingsInit(void) {
	vbutton.x = originX;
	vbutton.y = originY + 150.f;
	buttonscalex = WINDOWLENGTH_X / 27;
	buttonscaley = WINDOWLENGTH_Y / 18;

	vbar.x = originX;
	vbar.y = originY + 150.f;
	barscalex = WINDOWLENGTH_X / 2;
	barscaley = WINDOWLENGTH_Y / 80;
}

void GameStateSettingsUpdate(void) {

	variables_update();

	// full screen button pressed
	if (AEInputCheckReleased(AEVK_LBUTTON) &&
		center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
		center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 15 - WINDOWLENGTH_Y / 16 &&
		center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * 15 + WINDOWLENGTH_Y / 16) {
		fullscreen = !fullscreen;

		AESysToggleFullScreen(fullscreen);
	}

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


	if (AETestPointToRect(&center_cursor, &vbutton, buttonscalex, buttonscaley) && (AEInputCheckReleased(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_LBUTTON))) {
		AEVec2Set(&vbutton, center_cursor.x, vbutton.y);

		if (vbutton.x < -WINDOWLENGTH_X / 4) vbutton.x = -WINDOWLENGTH_X / 4;
		if (vbutton.x > WINDOWLENGTH_X / 4) vbutton.x = WINDOWLENGTH_X / 4;
	}

	if (AETestPointToRect(&center_cursor, &vbutton, barscalex, barscaley*5) && (AEInputCheckReleased(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_LBUTTON))) {
		AEVec2Set(&vbutton, center_cursor.x, vbutton.y);

		if (vbutton.x < -WINDOWLENGTH_X / 4) vbutton.x = -WINDOWLENGTH_X / 4;
		if (vbutton.x > WINDOWLENGTH_X / 4) vbutton.x = WINDOWLENGTH_X / 4;
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
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

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
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	//volume slider
	AEMtx33Scale(&scale, barscalex, barscaley);
	AEMtx33Trans(&translate, vbar.x, vbar.y);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(volume_bar, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	//volume button
	AEMtx33Scale(&scale, buttonscalex, buttonscaley);
	AEMtx33Trans(&translate, vbutton.x, vbutton.y);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(volume_button, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);



		AEGfxPrint(Albam_fontID, (s8*)"Full Screen", -0.2, -0.05, 0.95F, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"Back", -0.1, -0.3, 0.95F, 1, 1, 1);




}
void GameStateSettingsFree() {

}

void GameStateSettingsUnload(void) {

	//// FontID unload
	//AEGfxDestroyFont(Albam_fontID);

	// Button texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);
	
	// Volume texture unload
	AEGfxTextureUnload(volume_bar);
	AEGfxTextureUnload(volume_button);

	// Mesh free
	AEGfxMeshFree(square_mesh);
}