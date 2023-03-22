#pragma once

const int asset3_count{ 3 };

void GameStateTutorial2Load(void);
void GameStateTutorial2Init(void);
void GameStateTutorial2Update(void);
void GameStateTutorial2Draw(void);
void GameStateTutorial2Free(void);
void GameStateTutorial2Unload(void);

static enum asset3 {
	enemy3,
	enemy3_warning,
	_boss
};