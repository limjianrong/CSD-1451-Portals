#include "AEEngine.h"
#include <cmath> //for square root function
#include "portal_feature.hpp"
#include <iostream> //for std::cout
#define PORTAL_WIDTH 60.0f
#define PORTAL_HEIGHT 60.0f
#define TRUE 1
#define FALSE 0

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

struct {
	s32 x{}, y{};
	int alive{};
	AEGfxVertexList* mesh{};
	AEVec2 center{2000, 2000 };

}portal_1,portal_2;

AEGfxVertexList* portalmesh;
AEGfxVertexList* portalmesh2;
AEGfxVertexList* hexagonmesh;
AEGfxTexture* greencircle;
AEVec2* pointertoplayercenter{ };
AEGfxVertexList* greencirclemesh;

int drawportal{ 0 };
int drawtoken{};


void initialize_portal(void) {
	greencircle = AEGfxTextureLoad("Assets/greencircle1.png");
	
	AEGfxMeshStart();
	AEGfxTriAdd(
		50.0f, -50.0f, 0xFF00FF00, 0.0f, 0.0f,
		50.0f, 50.0f, 0xFF00FF00, 0.0f, 0.0f,
		-50.0f, 50.0f, 0xFF00FF00, 0.0f, 0.0f);
	/*AEGfxTriAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);*/
	AEGfxVertexAdd(-50.0f, 50.0f, 0xFF00FF00, 0.0f, 0.0f);
	AEGfxVertexAdd(-50.0f, -50.0f, 0xFF00FF00, 0.0f, 0.0f);
	AEGfxVertexAdd(50.0f, -50.0f, 0xFF00FF00, 0.0f, 0.0f);
	greencirclemesh = AEGfxMeshEnd();

	AEGfxMeshStart();
	AEGfxTriAdd(
		30.0f, -30.0f, 0xFF00FF00, 0.0f, 0.0f,
		30.0f, 30.0f, 0xFF00FF00, 0.0f, 0.0f,
		-30.0f, 30.0f, 0xFF00FF00, 0.0f, 0.0f);
	/*AEGfxTriAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);*/
	AEGfxVertexAdd(-30.0f, 30.0f, 0xFF00FF00, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, -30.0f, 0xFF00FF00, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, -30.0f, 0xFF00FF00, 0.0f, 0.0f);
	portal_1.mesh = portalmesh = AEGfxMeshEnd();
	
	AEGfxMeshStart();
	AEGfxTriAdd(
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	
	portal_2.mesh = AEGfxMeshEnd();

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


}
void portal_feature(AEVec2* PlayerCenter, f32 &playerx, f32 &playery) {
	
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEMtx33Rot(&portal::rotation, portal::rotateby);
	//AEMtx33Trans(&portal::translate, -100.0f, 150.0f);
	//AEMtx33Concat(&portal::finalportal, &portal::translate, &portal::rotation); //order is impt! translate -> rotation =/= rotation -> translate
	//AEGfxSetTransform(portal::finalportal.m);
	//AEGfxSetPosition(portal::x, portal::y); //careful! setposition affects the rotation of the object that is drawn to output
	//AEGfxMeshDraw(portalmesh, AE_GFX_MDM_TRIANGLES);
	//if (portal::rotateby <= 360.0) {
	//	portal::rotateby += 0.01f;
	//}

	////portal2
	//AEMtx33Rot(&portal::rotation2, portal::rotateby);
	//AEMtx33Trans(&portal::translate2, -100.0f, 150.0f);
	//AEMtx33Concat(&portal::finalportal2, &portal::translate2, &portal::rotation2); //order is impt! translate -> rotation =/= rotation -> translate
	//AEGfxSetTransform(portal::finalportal2.m);
	//AEGfxSetPosition(portal::x2, portal::y2); //careful! setposition affects the rotation of the object that is drawn to output
	//AEGfxMeshDraw(portalmesh2, AE_GFX_MDM_TRIANGLES);
	//
	/*if (AETestRectToRect(&portal::center, 60.0f, 60.0f, PlayerCenter, 50.0f, 50.0f)) {
		std::cout << "Collisionportal1" << std::endl;
		
			playerx = portal::x2;
			playery = portal::y2;
	}*/
	/*if (AETestRectToRect(&portal::center2, 60.0f, 60.0f, PlayerCenter, 50.0f, 50.0f)) {
		std::cout << "Collisionportal2" << std::endl;
		playerx = portal::x;
		playery = portal::y;
	}*/
	
}



void draw_a_portal(AEVec2* PlayerCenter, f32& playerx, f32& playery) {
	
	
	AEMtx33 drawportal1{0};
	AEMtx33 drawportal2{};
	
	if (AEInputCheckTriggered(VK_RBUTTON)) {
		if (drawportal == 0) {
			drawportal = 1;
			AEInputGetCursorPosition(&portal_1.x, &portal_1.y);
			
			if (portal_1.y <= AEGetWindowHeight() / 2) {
				portal_1.y = AEGetWindowHeight() / 2 - portal_1.y;
			}
			else {
				portal_1.y = -(portal_1.y - AEGetWindowHeight() / 2);
			}
			AEVec2Set(&portal_1.center, static_cast<f32>(portal_1.x) - AEGetWindowWidth() / 2, static_cast<f32>(portal_1.y));
			if (sqrt(AEVec2SquareDistance(PlayerCenter, &portal_1.center))<100 || sqrt(AEVec2SquareDistance(PlayerCenter, &portal_1.center)) > 200) {
				std::cout << "portal 1 selection is out of range";
				drawportal = 0;
				drawtoken = 0;
			}
			
		}
		else if (drawportal == 1) {
			AEInputGetCursorPosition(&portal_2.x, &portal_2.y);
			if (portal_2.y <= AEGetWindowHeight() / 2) {
				portal_2.y = AEGetWindowHeight() / 2 - portal_2.y;
			}
			else {
				portal_2.y = -(portal_2.y - AEGetWindowHeight() / 2);
			}
			AEVec2Set(&portal_2.center, static_cast<f32>(portal_2.x) - AEGetWindowWidth() / 2, static_cast<f32>(portal_2.y));
			drawtoken = 1;
			if (sqrt(AEVec2SquareDistance(PlayerCenter, &portal_2.center)) < 100 || sqrt(AEVec2SquareDistance(PlayerCenter, &portal_2.center)) > 400) {
				std::cout<<"portal 2 selection is out of range";
				drawtoken = 0;
			}
			//drawportal = 0;
			
		}
	}

	if (drawtoken == 1) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEMtx33Trans(&drawportal1, static_cast<f32>(portal_1.x) -AEGetWindowWidth()/2 , static_cast<f32>(portal_1.y));
		AEGfxSetTransform(drawportal1.m);
		AEGfxMeshDraw(portal_1.mesh, AE_GFX_MDM_TRIANGLES);

		AEMtx33Trans(&drawportal2, static_cast<f32>(portal_2.x) - AEGetWindowWidth() / 2, static_cast<f32>(portal_2.y));
		AEGfxSetTransform(drawportal2.m);
		AEGfxMeshDraw(portal_2.mesh, AE_GFX_MDM_TRIANGLES);

		

		if (AETestRectToRect(&(portal_1.center), 60.0f, 60.0f, PlayerCenter, 50.0f, 50.0f)) {
			std::cout << "Collided with portal_1" << std::endl;
			playerx = portal_2.center.x;
			playery = portal_2.center.y;
			drawportal = 0;
			drawtoken = 0;
		}
	}
	/*AEGfxSetTransparency(1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(PlayerCenter->x, PlayerCenter->y);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(greencircle, 0, 0);
	AEGfxMeshDraw(greencirclemesh, AE_GFX_MDM_TRIANGLES);*/
}