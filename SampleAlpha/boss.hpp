#pragma once
#include "AEEngine.h"


enum { UP, DOWN,STOP, BTM_RIGHT, BTM_LEFT, TOP_RIGHT, TOP_LEFT };

struct Boss{
	f32 x_pos{ 050.0f }, y_pos{ 200.0f };//3050
	f32 width{ 200 }, height{ 200 }, velocity{ 100 };
	f32 range_x{ 100 + 200 }, range_y{ 100 + 500 };
	s32 Hp{ 5 };
	s32 previous_direction{};
	s32 direction{ UP };
	f32 charge_velocity{ 5 };
	f32 charge_angle{};
	s32 charge_towards{};
	s32 return_to_position{};
	f32 charge_cooldown{1000.0f};
	// ---- Mesh & Texture ----
	AEMtx33 scale{}, translate{}, matrix{};
	AEGfxVertexList* mesh{};
	AEGfxTexture* standTex, * deadTex;


	AEVec2 original_position{ 0,0 };	// SAME AS center?
	AEVec2 center{0,0};					// SAME AS original_position?
	AEVec2 charge_direction{ 0,0 };
	AEVec2 charge_endpoint{ 0,0 };
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