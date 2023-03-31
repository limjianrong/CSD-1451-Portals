/*!**************************************************************************************************
\file     Utilities.hpp
\author   Lin ZhaoZhi
\par      DP email: z.lin@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file implements the functions used to initialize and bullet mechanics.

  The function includes:
  - create_Square_Mesh
	Creates a square mesh of width 1 and height 1
*****************************************************************************************************/
#pragma once
#include "AEEngine.h"

// Custom window size
#define WINDOWLENGTH_X 1280
#define WINDOWLENGTH_Y 730

//#define DEBUG		// DEBUG unit testing (Uncomment to debug)
//extern AEAudioGroup soundGroup;

struct GameObjects {

	s32 Hp, Max_Hp, Max_Hp_Reset;
	bool status;		// TRUE for alive, FALSE for dead
	AEVec2 center;		// x & y
	AEVec2 dimensions;	// width & height

	// --- Rendering ---
	void RenderObject();
	void Render_HealthBar();

};

AEGfxVertexList* create_Square_Mesh();
void drawMesh(AEVec2 scale, AEVec2 trans, f32 rot);
void mesh_load();
void variables_update();

// Collision between enemy1 & player
//bool CollisionIntersection_RectRect(Enemy1_stats& obj1, Player_stats& obj2);
