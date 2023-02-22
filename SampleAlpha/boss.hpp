#pragma once
#include "AEEngine.h"


enum { UP, DOWN };
struct boss {
	f32 x{};
	f32 y{};
	AEMtx33 scale{};
	AEMtx33 translate{};
	AEMtx33 matrix{};
	AEGfxVertexList* mesh{};
	AEGfxTexture* picture{};
	const int width{ 200 };
	const int height{ 200 };
	const int velocity{ 100 };
	int direction{ UP };
	AEVec2 center{0,0};
};

struct laser_beam {
	int status{};
	f32 x{};
	f32 y{};
	f32 width{300};
	f32 height{50};
	AEMtx33 scale{};
	AEMtx33 translate{};
	AEMtx33 rotate{};
	AEMtx33 matrix{};
	AEVec2 center{};
	AEGfxVertexList* mesh{};
};



void initialize_boss();
void draw_boss();
void update_boss();
void boss_laser_beam();