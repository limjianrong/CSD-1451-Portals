/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Audio.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  Source file for all the audio functions, only init and load
==================================================================================*/

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

void audio_load() {
	
	// BGM 
	gameBGM = AEAudioLoadSound("Assets/AUDIO/Game BGM.mp3");
	bossBGM = AEAudioLoadSound("Assets/AUDIO/Boss battle BGM.mp3");

	// Win/Lose
	defeatAudio			= AEAudioLoadSound("Assets/AUDIO/Defeat.mp3");
	victoryAudio		= AEAudioLoadSound("Assets/AUDIO/Victory.mp3");

	// Button
	buttonClickedAudio	= AEAudioLoadSound("Assets/AUDIO/Button_Click.mp3");
	buttonHoverAudio	= AEAudioLoadSound("Assets/AUDIO/Button_Hover.mp3");

	// Player
	checkpointAudio		= AEAudioLoadSound("Assets/AUDIO/Checkpoint.mp3");
	walkAudio			= AEAudioLoadSound("Assets/AUDIO/Player_Movement.mp3");
	playerDeathAudio	= AEAudioLoadSound("Assets/AUDIO/Player_Death.mp3");
	playerDamageAudio	= AEAudioLoadSound("Assets/AUDIO/Player_Dmg.mp3");

	// Enemy 1 & 2
	deathAudio			= AEAudioLoadSound("Assets/AUDIO/Enemy&Boss_Death");
	bulletAudio			= AEAudioLoadSound("Assets/AUDIO/Bullet.mp3");
	damageAudio			= AEAudioLoadSound("Assets/AUDIO/Enemy&Boss_Dmg.mp3");

	// Enemy 3
	zoomAudio			= AEAudioLoadSound("Assets/AUDIO/Enemy3_Zoom.mp3");

	// Boss
	laserAudio			= AEAudioLoadSound("Assets/AUDIO/Boss_Laser.mp3");

	// Portal
	portalAudio			= AEAudioLoadSound("Assets/AUDIO/Portals.mp3");

	// Upgrades
	levelUpAudio = AEAudioLoadSound("Assets/AUDIO/Player_LevelUp.mp3");
	receivedUpgradeAudio = AEAudioLoadSound("Assets/AUDIO/UpgradeTimeOver.mp3");



}

void audio_init() {

	soundGroup = AEAudioCreateGroup();
	musicGroup = AEAudioCreateGroup();
}