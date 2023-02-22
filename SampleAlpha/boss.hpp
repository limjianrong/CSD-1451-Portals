#pragma once
#include "AEEngine.h"

struct boss {
	f32 x{};
	f32 y{};
	AEMtx33 scale{};
	AEMtx33 translate{};
	AEMtx33 matrix{};
	//AEVec2 center{0,0};
};

enum { UP, DOWN };

void initialize_boss();
void draw_boss();
void update_boss();
