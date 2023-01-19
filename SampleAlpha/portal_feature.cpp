#include "AEEngine.h"
#include "portal_feature.hpp"
#include <iostream> //for std::cout
namespace portal {
	AEMtx33 rotation{};
	AEMtx33 translate{};
	AEMtx33 finalportal{};
	f32 x{ -100.0 };
	f32 y{ 150.0 };
	AEVec2 center{ portal::x, portal::y };
	AEVec2* pointertoportalcenter{ &center };
	f32 rotateby{ 0 };

}

AEGfxVertexList* portalmesh;
AEGfxVertexList* hexagonmesh;
//AEVec2 player_center{ playerx{}, playery{} };
AEVec2* pointertoplayercenter{ };

void initialize_player_portal(void) {
	AEGfxMeshStart();
	AEGfxTriAdd(
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	/*AEGfxTriAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);*/
	AEGfxVertexAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	portalmesh = AEGfxMeshEnd();


	//drawing a hexagon mesh
	/*AEGfxMeshStart();
	AEGfxVertexAdd(10.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, 10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, -10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(10.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-10.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, -10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, 10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-10.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);

	hexagonmesh = AEGfxMeshEnd();*/


}
void player_portal(void) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEMtx33Rot(&portal::rotation, portal::rotateby);
	//AEMtx33Trans(&portal::translate, -100.0f, 150.0f);
	//AEMtx33Concat(&portal::finalportal, &portal::translate, &portal::rotation); //order is impt! translate -> rotation =/= rotation -> translate
	//AEGfxSetTransform(portal::finalportal.m);
	AEGfxSetPosition(portal::x, portal::y); //careful! setposition affects the rotation of the object that is drawn to output
	AEGfxMeshDraw(portalmesh, AE_GFX_MDM_TRIANGLES);
	if (portal::rotateby <= 360.0) {
		portal::rotateby += 0.01f;
	}

	/*if (AETestRectToRect(portal::pointertoportalcenter, 60.0f, 60.0f, pointertoplayercenter, 50.0f, 50.0f)) {
		portal::x = 500.0f;
		std::cout << "collision detected" << std::endl;
	}*/

	//AEGfxMeshDraw(hexagonmesh, AE_GFX_MDM_LINES_STRIP);
	/*if (AEInputCheckCurr(AEVK_M)) {
		rotation += 10;
	}*/
}

void check_player_and_portal_collide(void) {
	AETestRectToRect(portal::pointertoportalcenter, 60.0f, 60.0f, pointertoplayercenter, 50.0f, 50.0f);

}