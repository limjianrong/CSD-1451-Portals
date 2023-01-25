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
AEGfxVertexList* drawamesh1;
AEGfxVertexList* drawamesh2;
int drawtoken{};
AEVec2* pointertoplayercenter{ };
int drawportal{ 0 };
int keyreleased{ 0 };
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
	drawamesh2 = drawamesh1 = portalmesh = AEGfxMeshEnd();
	

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



void draw_a_portal(f32 playerx, f32 playery) {
	
	s32 drawportal1x{};
	s32 drawportal1y{};
	s32 drawportal2x{};
	s32 drawportal2y{};
	AEMtx33 drawportal1{};
	AEMtx33 translatedrawportal1{};
	AEMtx33 drawportal2{};
	AEMtx33 translateddrawportal2{};

	if (AEInputCheckTriggered(VK_RBUTTON)) {
		if (drawportal == 0) {
			drawportal = 1;
			AEInputGetCursorPosition(&drawportal1x, &drawportal1y);
			AEMtx33Trans(&drawportal1, drawportal1x, drawportal1y);
			std::cout << "1\n";
		}
		else if (drawportal == 1) {
			AEInputGetCursorPosition(&drawportal2x, &drawportal2y);
			AEMtx33Trans(&drawportal2, drawportal2x, drawportal2y);
			drawtoken = 1;
			std::cout << "2\n";
		}
	}

	if (drawtoken == 1) {
		AEGfxSetPosition(200, 200);
		AEGfxMeshDraw(drawamesh1, AE_GFX_MDM_TRIANGLES);
		AEGfxSetPosition(100, 100);
		AEGfxMeshDraw(drawamesh1, AE_GFX_MDM_TRIANGLES);
		std::cout << "3\n";
	}
}