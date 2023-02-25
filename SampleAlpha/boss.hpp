#pragma once
#include "AEEngine.h"


enum { UP, DOWN };

struct Boss{
	f32 x_pos{ 1500.0f }, y_pos{ 0.0f };
	f32 width{ 200 }, height{ 200 }, velocity{ 100 };
	f32 range_x{ 100 + 200 }, range_y{ 100 + 500 };
	s32 Hp{ 5 };
	s32 direction{ UP };
	f32 charge_velocity{ 500 };

	// ---- Mesh & Texture ----
	AEMtx33 scale{}, translate{}, matrix{};
	AEGfxVertexList* mesh{};
	AEGfxTexture* standTex, * deadTex;


	AEVec2 center{0,0};
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
void boss_charge();