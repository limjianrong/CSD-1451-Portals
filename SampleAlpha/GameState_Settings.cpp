#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"


extern AEGfxVertexList* button;
extern AEMtx33 scale, rotate, translate, transform;

void GameStateSettingsLoad(void) {
	
}

void GameStateSettingsInit(void) {


}

void GameStateSettingsUpdate(void) {
	if (AEInputCheckTriggered(AEVK_L))
		gGameStateNext = GS_MainMenu;

}

void GameStateSettingsDraw(void) {

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

}
void GameStateSettingsFree() {

}

void GameStateSettingsUnload(void) {
	
}