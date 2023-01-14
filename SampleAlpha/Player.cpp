#include "AEEngine.h"

int playersize{ 200 };
float rotation{ 0 };
float x{  }, y{  };
AEGfxVertexList* pMesh;
AEGfxVertexList* trianglemesh;


void initialize_player(int playersize) { //PLAYERSIZE is not used for now

	
	// Pointer to Mesh
	//pMesh = 0;
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();
	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	trianglemesh = AEGfxMeshEnd();

}

void draw_player(int playersize) {
	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AEGfxTexture* yellowball = AEGfxTextureLoad("Assets/ball9.png");

	// Create a scale matrix that scales by 100 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 50.f, 50.f);

	// Create a rotation matrix that rotates by 45 degrees
	AEMtx33 rotate = { 0 };

	AEMtx33Rot(&rotate, rotation);
	// Create a translation matrix that translates by
	// 100 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, x, y);
	// Concat the matrices (TRS)
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to use
	AEGfxSetTransform(transform.m);

	//if user presses 'A'
	if (AEInputCheckCurr(0x41)) {
		x -= 1;
		rotation += 0.1;
	}

	//if user presses 'D'
	if (AEInputCheckCurr(0x44)) {
		x += 1;
		rotation -= 0.1;
	}
	// Set the texture to pTex
	AEGfxTextureSet(pTex, 0, 0);
	
	// Actually drawing the mesh 
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	AEMtx33 scale2 {};
	AEMtx33Scale(&scale2, 100.0f, 100.0f);
	AEMtx33 translate2{};
	AEMtx33Trans(&translate2, 100.0f, 100.0f);
	AEMtx33 finalform{};
	AEMtx33Concat(&finalform, &scale2, &translate2);
	AEGfxSetTransform(finalform.m);
	
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(yellowball, 0, 0);
	AEGfxMeshDraw(trianglemesh, AE_GFX_MDM_TRIANGLES);

	
}

void player_movement(void) {
	
}