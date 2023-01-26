#include "AEEngine.h"


AEGfxTexture* rect;
AEGfxVertexList* rectmesh;

void draw_level_init() {

	rect = AEGfxTextureLoad("Assets/rect.png");

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, 75.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		300.0f, 75.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		300.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		300.0f, 75.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	// Saving the mesh (list of triangles) in pMesh
	rectmesh = AEGfxMeshEnd();

}

void draw_level() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(rect, 0, 0);
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 1, 1);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 0);
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, -500.f, -200.f);
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

	AEMtx33Trans(&translate, -150.f, -100.f);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

	AEMtx33Trans(&translate, 175.f, 50.f);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

	//AEGfxMeshFree(rectmesh);
	//AEGfxTextureUnload(rect);
}
