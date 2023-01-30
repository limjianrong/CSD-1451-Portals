#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"
#include <string>

s8 fontID;
s8 string_title[] = "Portals";
s8 string_startgame[] = "Start Game";
s8 string_quitgame[] = "Quit Game";
s8* mainmenu_title;
s8* mainmenu_startgame;
s8* mainmenu_quitgame;
AEGfxVertexList* play_button;

void GameStateMainmenuLoad(void) {
	fontID = AEGfxCreateFont("Assets/Roboto-Regular.ttf",50);
	play_button = create_Square_Mesh();
}

void GameStateMainmenuInit(void) {

	mainmenu_title = string_title;
	mainmenu_startgame = string_startgame;
	mainmenu_quitgame = string_quitgame;
}

void GameStateMainmenuUpdate(void) {

	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		gGameStateNext = GS_Platformer;
	}
}

void GameStateMainmenuDraw(void) {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// draw rectangle box
	AEMtx33 weapon_scale = { 0 };
	AEMtx33Scale(&weapon_scale, 270.0f, 80.0f); // scaling it up
	AEMtx33 weapon_translate = { 0 };
	AEMtx33Trans(&weapon_translate, -10.0f, 10.0f); // shifts along x & y axis
	AEMtx33 weapon_rotate = { 0 };
	AEMtx33Rot(&weapon_rotate, 0); // rotation
	AEMtx33 weapon_transform = { 0 };
	AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
	AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);
	AEGfxSetTransform(weapon_transform.m);
	AEGfxMeshDraw(play_button, AE_GFX_MDM_TRIANGLES);


	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(fontID, mainmenu_title, -0.20, 0.4, 1.0F, 1, 1, 1);
	AEGfxPrint(fontID, mainmenu_startgame, -0.28, 0, 1.0F, 1, 1, 1);
	AEGfxPrint(fontID, mainmenu_quitgame, -0.26, -0.3, 1.0F, 1, 1, 1);
}
void GameStateMainmenuFree() {
	
}

void GameStateMainmenuUnload(void) {
	AEGfxDestroyFont(fontID);
}
