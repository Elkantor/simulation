#include <stdio.h>
#include <raylib.h>

#include "simulation/variables.c"

int main(){
	InitWindow(1920, 1080, "test");

	const unsigned short number_cells_width = (SIMULATION_CANVA_WIDTH) / (SIMULATION_CELL);
	const unsigned short number_cells_height = (SIMULATION_CANVA_HEIGHT) / (SIMULATION_CELL);

	while(!WindowShouldClose()){
		printf("%d\n", ColorToInt(GRAY));
		BeginDrawing();
			ClearBackground(LIGHTGRAY);
			// DrawRectangle(
			// 	SIMULATION_CANVA_X, 
			// 	SIMULATION_CANVA_Y, 
			// 	SIMULATION_CANVA_WIDTH,
			// 	SIMULATION_CANVA_HEIGHT,
			// 	GRAY
			// );			
			
			/* Draw the grid inside the canva */
			// Draw the X lines
			for(unsigned short i = number_cells_height; i != 0; --i){
				DrawLine(
					SIMULATION_CANVA_X, 
					(i*SIMULATION_CELL) + (SIMULATION_CANVA_Y), 
					(SIMULATION_CANVA_X) + (SIMULATION_CANVA_WIDTH), 
					(i*SIMULATION_CELL) + (SIMULATION_CANVA_Y), 
					GREEN
				);
			}
			// Draw the Y lines
			for(unsigned short i = number_cells_width; i != 0; --i){
				DrawLine(
					(i*SIMULATION_CELL) + (SIMULATION_CANVA_X),
					SIMULATION_CANVA_Y,
					(i*SIMULATION_CELL) + (SIMULATION_CANVA_X), 
					(SIMULATION_CANVA_Y) + (SIMULATION_CANVA_HEIGHT), 
					GREEN
				);
			}

			/* Draw the entities */
			for(unsigned short i = 0; i < simulation_rectangles_size ; ++i){
				DrawRectangle(
					simulation_rectangles[i][0], 
					simulation_rectangles[i][1], 
					simulation_rectangles[i][2], 
					simulation_rectangles[i][3],
					GetColor((int)simulation_rectangles[i][4])
				);
			}
			Vector2 mouse_position = GetMousePosition();
			const Rectangle gui_wall = { simulation_rectangles[0][0], simulation_rectangles[0][1], simulation_rectangles[0][2], simulation_rectangles[0][3] };
			if(mouse_position.x >= gui_wall.x){
				if(mouse_position.x <= (gui_wall.x + gui_wall.width)){
					DrawText("INSIDE !", 300, 300, 18, BLUE);
				}
			}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}