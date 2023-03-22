#pragma once

const int asset_count{ 3 };
const int asset2_count{ 2 };

void GameStateTutorialLoad(void);
void GameStateTutorialInit(void);
void GameStateTutorialUpdate(void);
void GameStateTutorialDraw(void);
void GameStateTutorialFree(void);
void GameStateTutorialUnload(void);

static enum asset {
	_player,
	portals,
	range
};

static enum asset2 {
	enemy1,
	enemy2
};


