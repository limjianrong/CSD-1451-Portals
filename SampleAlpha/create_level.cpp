#include "AEEngine.h"
#include <iostream>

AEGfxVertexList* groundplatform{};

void initialize_platform(void) {
	
	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, 50.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		600.0f, 50.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		600.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		600.0f, 50.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	groundplatform = AEGfxMeshEnd();
	
}

void draw_platform(void) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEMtx33 translate{};
	AEMtx33Trans(&translate, -AEGetWindowWidth()/2, -AEGetWindowHeight()/2);
	AEGfxSetTransform(translate.m);
	AEGfxMeshDraw(groundplatform, AE_GFX_MDM_TRIANGLES);
}

