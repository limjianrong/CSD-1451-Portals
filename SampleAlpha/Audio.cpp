#pragma once
#include "AEEngine.h"
#include "Audio.hpp"

AEAudio gameBGM, bossBGM;
AEAudio buttonAudio, portalAudio;
//AEAudio checkpointAudio;
AEAudio defeatAudio, victoryAudio;
AEAudio bossDamageAudio, bosslaserAudio;
AEAudio enemyDamageAudio, enemyDeathAudio;
//AEAudio walkAudio, levelUpAudio;

//AEAudioGroup soundGroup;

void audio_load() {
	bossBGM				= AEAudioLoadMusic("Assets/AUDIO/Boss battle BGM.mp3");
	gameBGM				= AEAudioLoadMusic("Assets/AUDIO/Game BGM.mp3");

	buttonAudio			= AEAudioLoadSound("Assets/AUDIO/Button click.mp3");
	//checkpointAudio		= AEAudioLoadSound("Assets/AUDIO/Checkpoint.mp3");
	//levelUpAudio		= AEAudioLoadSound("Assets/AUDIO/Player levelup.wav");
	portalAudio			= AEAudioLoadSound("Assets/AUDIO/Portals.m4a");
	defeatAudio			= AEAudioLoadSound("Assets/AUDIO/Defeat.wav");
	victoryAudio		= AEAudioLoadSound("Assets/AUDIO/Victory.wav");
	enemyDamageAudio	= AEAudioLoadSound("Assets/AUDIO/Enemy damage.mp3");
	enemyDeathAudio		= AEAudioLoadSound("Assets/AUDIO/Enemy dead.ogg");
	//walkAudio			= AEAudioLoadSound("Assets/AUDIO/Player walk.flac");


	//soundGroup = AEAudioCreateGroup();
}

void audio_init();
void audio_draw();
void audio_update() {
	//AEAudioPlay(victoryAudio, soundGroup, 1.f, 1.f, 0);
	//AEAudioPlay(walkAudio, soundGroup, 1.f, 1.f, 0);
	AEAudioUpdate();
}
void audio_unload();
void audio_free();