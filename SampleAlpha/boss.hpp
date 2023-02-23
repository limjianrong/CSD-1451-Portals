#pragma once
#include "AEEngine.h"


enum { UP, DOWN };
struct boss {
	f32 x_pos{ }, y_pos{ };
	AEMtx33 scale{}, translate{}, matrix{};
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
	AEMtx33 scale{}, translate{}, rotate{}, matrix{};
	AEVec2 center{};
	AEGfxVertexList* mesh{};
	AEGfxTexture* picture{};
};



void initialize_boss();
void draw_boss();
void update_boss();
void boss_laser_beam();