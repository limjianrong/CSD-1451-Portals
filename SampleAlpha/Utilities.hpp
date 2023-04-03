/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Utilities.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This header file contains definitions of functions for global use in every source file.
  Contains only variable and functions that are commonly used in most source file.
  Defined base struct class. player, enemy1, enemy2, boss uses this base struct to derive 
==================================================================================*/

#pragma once
#include "AEEngine.h"

// Custom window size
#define WINDOWLENGTH_X 1600
#define WINDOWLENGTH_Y 900

//#define DEBUG		// DEBUG unit testing (Uncomment to debug)

struct GameObjects {

	s32 Hp, Max_Hp, Max_Hp_Reset;
	bool status;		// TRUE for alive, FALSE for dead
	AEVec2 center;		// x & y
	AEVec2 dimensions;	// width & height

	// --- Rendering ---
	void RenderObject();
	void Render_HealthBar();

};

void drawMesh(AEVec2 scale, AEVec2 trans, f32 rot);
void mesh_load();
void variables_update();
bool check_window_out_of_focus();

