void initialize_player(int playersize);
void draw_player(int playersize);
void player_movement(AEVec2 PlayerCenter);
void initialize_endpoint(void);
void check_endpoint(f32 playerx, f32 playery, AEGfxVertexList* endpoint_rectangle,
	AEGfxVertexList* endpoint_triangle, AEVec2* PlayerCenter, AEVec2& endpoint_center);


