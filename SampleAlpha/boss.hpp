#pragma once
#include "AEEngine.h"


enum { UP, DOWN };

struct Boss{
	f32 x_pos{ }, y_pos{ };
	f32 width{ 200 };
	f32 height{ 200 };
	f32 velocity{ 100 };
	int direction{ UP };
	AEVec2 center{ 0,0 };

	// ---- Mesh & Texture ----
	AEMtx33 scale{}, translate{}, matrix{};
	AEGfxVertexList* mesh{};
	AEGfxTexture* picture{};
};

struct Laser_beam{
	int status{};
	f32 x{}, y{};
	f32 width{300}, height{50};
	AEVec2 center{};

	// ---- Mesh & Texture ----
	AEMtx33 scale{}, translate{}, rotate{}, matrix{};
	AEGfxVertexList* mesh{};
	AEGfxTexture* picture{};
};



void initialize_boss();
void draw_boss();
void update_boss();
void boss_laser_beam();