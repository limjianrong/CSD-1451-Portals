#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"


extern AEGfxVertexList* button;
extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;

void GameStateSettingsLoad(void) {
	
}

void GameStateSettingsInit(void) {


}

void GameStateSettingsUpdate(void) {

	if (AEInputCheckReleased(AEVK_LBUTTON))
		gGameStateNext = GS_MainMenu;

}

void GameStateSettingsDraw(void) {

	AEGfxPrint(Albam_fontID, (s8*)"Click ANYWHERE", -0.5, 0.35, 2.0f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"to go back to", -0.5, 0, 2.0f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Main Menu!", -0.5, -0.38, 2.0f, 1, 1, 1);

}
void GameStateSettingsFree() {

}

void GameStateSettingsUnload(void) {
	
}