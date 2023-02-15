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
#include "AEEngine.h"

// Custom window size
#define WINDOWXLENGTH 1000
#define WINDOWYLENGTH 600

AEGfxVertexList* create_Square_Mesh();
s8 create_font();
AEVec2 get_cursor_center_position();
