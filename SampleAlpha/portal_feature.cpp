#include "AEEngine.h"
#include "portal_feature.hpp"
#include <iostream> //for std::cout


namespace portal {
	AEMtx33 rotation{};
	AEMtx33 translate{};
	AEMtx33 finalportal{};
	AEMtx33 rotation2{};
	AEMtx33 translate2{};
	AEMtx33 finalportal2{};
	f32 x{ -100.0 };
	f32 y{ 150.0 };
	f32 x2{ 200.0 };
	f32 y2{ -150.0 };
	AEVec2 center{ portal::x, portal::y };
	AEVec2 center2{ portal::x2, portal::y2 };

	f32 rotateby{ 0 };

}

AEGfxVertexList* portalmesh;
AEGfxVertexList* portalmesh2;
AEGfxVertexList* hexagonmesh;

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
	portalmesh2 = AEGfxMeshEnd();
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
void portal_feature(AEVec2* PlayerCenter, f32& playerx, f32& playery) {
	int teleporttoken{};
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEMtx33Rot(&portal::rotation, portal::rotateby);
	AEMtx33Trans(&portal::translate, -100.0f, 150.0f);
	AEMtx33Concat(&portal::finalportal, &portal::translate, &portal::rotation); //order is impt! translate -> rotation =/= rotation -> translate
	AEGfxSetTransform(portal::finalportal.m);
	AEGfxSetPosition(portal::x, portal::y); //careful! setposition affects the rotation of the object that is drawn to output
	AEGfxMeshDraw(portalmesh, AE_GFX_MDM_TRIANGLES);
	if (portal::rotateby <= 360.0) {
		portal::rotateby += 0.01f;
	}

	//portal2
	AEMtx33Rot(&portal::rotation2, portal::rotateby);
	AEMtx33Trans(&portal::translate2, -100.0f, 150.0f);
	AEMtx33Concat(&portal::finalportal2, &portal::translate2, &portal::rotation2); //order is impt! translate -> rotation =/= rotation -> translate
	AEGfxSetTransform(portal::finalportal2.m);
	AEGfxSetPosition(portal::x2, portal::y2); //careful! setposition affects the rotation of the object that is drawn to output
	AEGfxMeshDraw(portalmesh2, AE_GFX_MDM_TRIANGLES);
	
	if (AETestRectToRect(&portal::center, 60.0f, 60.0f, PlayerCenter, 50.0f, 50.0f)) {
		std::cout << "Collisionportal1" << std::endl;
		
			playerx = portal::x2;
			playery = portal::y2;
	}
	/*if (AETestRectToRect(&portal::center2, 60.0f, 60.0f, PlayerCenter, 50.0f, 50.0f)) {
		std::cout << "Collisionportal2" << std::endl;
		playerx = portal::x;
		playery = portal::y;
	}*/
	
}

void check_player_and_portal_collide(void) {
	

}