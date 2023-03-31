#pragma once
#include "AEEngine.h"
#include "Audio.hpp"


AEAudio levelupAudio, checkpointAudio;
AEAudioGroup soundGroup;

void audio_load() {
	levelupAudio = AEAudioLoadSound("Assets/AUDIO/Player levelup.wav");
	AEAudioIsValidAudio(levelupAudio);
	checkpointAudio = AEAudioLoadSound("Assets/AUDIO/Checkpoint.mp3");
	AEAudioIsValidAudio(checkpointAudio);

	soundGroup = AEAudioCreateGroup();
}

void audio_init();
void audio_draw();
void audio_update() {
	AEAudioPlay(levelupAudio, soundGroup, 1.f, 1.f, 0);
	AEAudioUpdate();
}
void audio_unload();
void audio_free();