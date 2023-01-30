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

#define WINDOWXLENGTH 1000
#define WINDOWYLENGTH 600
#define bottom_rectx -500.f
#define bottom_recty -200.f
#define middle_rectx -150.f
#define middle_recty -100.f
#define top_rectx 175.f
#define top_recty 50.f

AEGfxVertexList* create_Square_Mesh();
