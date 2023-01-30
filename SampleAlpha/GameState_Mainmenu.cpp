#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "GameStateManager.hpp"
#include "utilities.hpp"
#include <string>

s8 fontID;
s8 string_title[] = "Portals";
s8 string_startgame[] = "Start Game";
s8 string_quitgame[] = "Quit Game";
s8* mainmenu_title;
s8* mainmenu_startgame;
s8* mainmenu_quitgame;
void GameStateMainmenuLoad(void) {
	fontID = AEGfxCreateFont("Assets/Roboto-Regular.ttf",50);

}

void GameStateMainmenuInit(void) {
	
	mainmenu_title = string_title;
	mainmenu_startgame = string_startgame;
	mainmenu_quitgame = string_quitgame;
}

void GameStateMainmenuUpdate(void) {
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(fontID, mainmenu_title, -0.2, 0.4, 1.0F, 1, 1, 1);
	AEGfxPrint(fontID, mainmenu_startgame, -0.28, 0, 1.0F, 1, 1, 1);
	AEGfxPrint(fontID, mainmenu_quitgame, -0.26, -0.3, 1.0F, 1, 1, 1);
	if (AEInputCheckReleased(VK_RBUTTON)) {
		//gGameStateNext = GS_Platformer;
	}
}

void GameStateMainmenuDraw(void) {

}
void GameStateMainmenuFree() {
	AEGfxDestroyFont(fontID);
}

void GameStateMainmenuUnload(void) {

}
