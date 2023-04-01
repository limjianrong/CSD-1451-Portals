#pragma once
#include "AEEngine.h"
#include "Audio.hpp"

// BGM
AEAudio gameBGM, bossBGM;
// Win / Lose
AEAudio defeatAudio, victoryAudio;
// Button
AEAudio buttonClickedAudio, buttonHoverAudio;

// Player
AEAudio checkpointAudio, walkAudio, playerDeathAudio, playerDamageAudio;

// Enemy & Boss
AEAudio damageAudio, deathAudio, bulletAudio;
// Boss
AEAudio laserAudio;
// Enemy 3
AEAudio zoomAudio;

// Portals
AEAudio portalAudio;

// Upgrade
AEAudio levelUpAudio, receivedUpgradeAudio;



// Group
AEAudioGroup soundGroup, musicGroup;

//AEAudioGroup soundGroup;

void audio_load() {
	
	// BGM 
	gameBGM = AEAudioLoadSound("Assets/AUDIO/Game BGM.mp3");
	bossBGM = AEAudioLoadSound("Assets/AUDIO/Boss battle BGM.mp3");

	// Win/Lose				DONE (Repeated everytime you press restart)
	defeatAudio			= AEAudioLoadSound("Assets/AUDIO/Defeat.mp3");
	victoryAudio		= AEAudioLoadSound("Assets/AUDIO/Victory.mp3");

	// Button				DONE
	buttonClickedAudio	= AEAudioLoadSound("Assets/AUDIO/Button_Click.mp3");
	buttonHoverAudio	= AEAudioLoadSound("Assets/AUDIO/Button_Hover.mp3");

	// Player				DONE
	checkpointAudio		= AEAudioLoadSound("Assets/AUDIO/Checkpoint.mp3");
	walkAudio			= AEAudioLoadSound("Assets/AUDIO/Player_Movement.mp3");
	playerDeathAudio	= AEAudioLoadSound("Assets/AUDIO/Player_Death.mp3");
	playerDamageAudio	= AEAudioLoadSound("Assets/AUDIO/Player_Dmg.mp3");

	// Enemy 1 & 2			DONE
	deathAudio			= AEAudioLoadSound("Assets/AUDIO/Enemy&Boss_Death");
	bulletAudio			= AEAudioLoadSound("Assets/AUDIO/Bullet.mp3");
	damageAudio			= AEAudioLoadSound("Assets/AUDIO/Enemy&Boss_Dmg.mp3");

	// Enemy 3 (WTF IS THAT CODE)
	zoomAudio			= AEAudioLoadSound("Assets/AUDIO/Enemy3_Zoom.mp3");

	// Boss					DONE (No bullets)
	//damageAudio		= AEAudioLoadSound("Assets/AUDIO/Enemy&Boss_Dmg.mp3");
	//deathAudio		= AEAudioLoadSound("Assets/AUDIO/Enemy&Boss_Death");
	//bulletAudio		= AEAudioLoadSound("Assets/AUDIO/Bullet.mp3");
	laserAudio			= AEAudioLoadSound("Assets/AUDIO/Boss_Laser.mp3");

	// Portal				DONE
	portalAudio			= AEAudioLoadSound("Assets/AUDIO/Portals.mp3");

	// Upgrades				DONE
	levelUpAudio = AEAudioLoadSound("Assets/AUDIO/Player_LevelUp.mp3");
	receivedUpgradeAudio = AEAudioLoadSound("Assets/AUDIO/UpgradeTimeOver.mp3");



}

void audio_init() {

	soundGroup = AEAudioCreateGroup();
	musicGroup = AEAudioCreateGroup();
}
void audio_draw();
void audio_update() {

	//AEAudioPlay(levelUpAudio, soundGroup, 1.f, 1.f, 0);
	//AEAudioUpdate();
}
void audio_unload();
void audio_free();