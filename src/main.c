#include <stdio.h>
#include <raylib.h>

#include "simulation/variables.c"
#include "simulation/icons.c"

// Draw selected icon using rectangles pixel-by-pixel
void DrawIcon(int iconId, Vector2 position, int pixelSize, Color color){
    #define BIT_CHECK(a,b) ((a) & (1<<(b)))

    for (int i = 0, y = 0; i < RICONS_SIZE*RICONS_SIZE/32; i++)
    {
        for (int k = 0; k < 32; k++)
        {
            if (BIT_CHECK(RICONS[8*iconId + i], k)) 
            {
            #if !defined(RICONS_STANDALONE)
                DrawRectangle(position.x + (k%RICONS_SIZE)*pixelSize, position.y + y*pixelSize, pixelSize, pixelSize, color);
            #endif
            }
            
            if ((k == 15) || (k == 31)) y++;
        }
    }
}

int main(){
	InitWindow(1920, 1080, "test");

	const unsigned short number_cells_width = (SIMULATION_CANVA_WIDTH) / (SIMULATION_CELL);
	const unsigned short number_cells_height = (SIMULATION_CANVA_HEIGHT) / (SIMULATION_CELL);

	// Cells colors indexes
	int cells_colors[simulation_rectangles_size];
	cells_colors[0] = SIMULATION_BLUE; 		// GUI on the right for the "2" blue button
	cells_colors[162] = SIMULATION_GREEN;	// GUI on the right for the "3" green button
	// Cells inside the canva
	for(unsigned short i = 1; i < simulation_rectangles_size-1; ++i){
		cells_colors[i] = SIMULATION_GRAY;
	}

	// Arrows directions indexes
	unsigned char cells_arrows_directions[simulation_rectangles_size];
	for(unsigned i = simulation_rectangles_size; i != 0; --i){
		cells_arrows_directions[i] = simulation_arrows_directions[0];
	}

	// Save key pressed
	int key_pressed;
	
	// Temp char buffer for drawing text on screen
	char text_buffer[200];

	while(!WindowShouldClose()){
		BeginDrawing();
			ClearBackground(LIGHTGRAY);
			
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

					
			// Handle the keyboard
			{
				int current_key_pressed = GetKeyPressed();
				if(IsKeyDown(KEY_UP)){ current_key_pressed = KEY_UP; }
				if(IsKeyDown(KEY_DOWN)){ current_key_pressed = KEY_DOWN; }
				if(IsKeyDown(KEY_RIGHT)){ current_key_pressed = KEY_RIGHT; }
				if(IsKeyDown(KEY_LEFT)){ current_key_pressed = KEY_LEFT; }
				if(current_key_pressed != -1 ){
					key_pressed = current_key_pressed;	
				}
			}

			Vector2 mouse_position = GetMousePosition();
			sprintf(text_buffer, "Mouse position: %f %f", mouse_position.x, mouse_position.y);
			DrawText(text_buffer, 500, 50, 24, BLUE);
			
			const Rectangle canva_wall = { SIMULATION_CANVA_X, SIMULATION_CANVA_Y, SIMULATION_CANVA_WIDTH, SIMULATION_CANVA_HEIGHT };
			// Check if the mouse is overing the canva
			if(mouse_position.x >= canva_wall.x 
				&& mouse_position.x <= (canva_wall.x + canva_wall.width)
				&& mouse_position.y >= canva_wall.y
				&& mouse_position.y <= (canva_wall.y + canva_wall.height)
			){
				unsigned char overed_column_x = (mouse_position.x  - SIMULATION_CANVA_X) / SIMULATION_CELL;
				unsigned char overed_column_y = (mouse_position.y  - SIMULATION_CANVA_Y) / SIMULATION_CELL;

				unsigned short index_cell_canva_overed = ((overed_column_x) * number_cells_height) + overed_column_y + 1;
				switch(key_pressed){
					case 49: // Key 1 (for the 1 button gui)
						cells_colors[index_cell_canva_overed] = SIMULATION_GRAY;
						cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[0];	// Reinit the arrows for the cell
						break;
					case 50: // Key 2 (for the 2 button gui)
						cells_colors[index_cell_canva_overed] = SIMULATION_BLUE;
						cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[0];	// Reinit the arrows for the cell
						break;
					case 51: // Key 3 (for the 3 button gui)
						cells_colors[index_cell_canva_overed] = SIMULATION_GREEN;
						cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[0];	// Reinit the arrows for the cell
						break;
					case KEY_UP:
						// Check if the cell is green to add directional top arrow on it
						if(cells_colors[index_cell_canva_overed] == SIMULATION_GREEN){
							cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[4]; // arrow on top
						}
						break;
					case KEY_DOWN:
						// Check if the cell is green to add directional top arrow on it
						if(cells_colors[index_cell_canva_overed] == SIMULATION_GREEN){
							cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[3]; // arrow on bottom
						}
						break;
					case KEY_RIGHT:
						// Check if the cell is green to add directional top arrow on it
						if(cells_colors[index_cell_canva_overed] == SIMULATION_GREEN){
							cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[2]; // arrow on right
						}
						break;
					case KEY_LEFT:
						// Check if the cell is green to add directional top arrow on it
						if(cells_colors[index_cell_canva_overed] == SIMULATION_GREEN){
							cells_arrows_directions[index_cell_canva_overed] = simulation_arrows_directions[1]; // arrow on left
						}
						break;
					default:
						break;
				}
				// Reinit the key pressed
				key_pressed = -1;
			}


			for(unsigned short i = 0; i < simulation_rectangles_size ; ++i){
				/* Draw the entities */
				DrawRectangle(
					simulation_rectangles[i][0], 
					simulation_rectangles[i][1], 
					simulation_rectangles[i][2], 
					simulation_rectangles[i][3],
					GetColor((int)cells_colors[i])
				);
				/* Draw the directional arrows on cells */
				if(cells_arrows_directions[i] != 0){ // Check if there is a directional arrow for the cell
					DrawIcon(cells_arrows_directions[i], (Vector2) { simulation_rectangles[i][0] + 20, simulation_rectangles[i][1] + (0.25*(SIMULATION_CELL)) }, 2, ORANGE);
				}	
			}

			/* Draw numbers on the GUI buttons */
			DrawText("1", simulation_rectangles[161][0] + 31, simulation_rectangles[161][1]+ 12, 64, WHITE); // Default button (GRAY)
			DrawText("2", simulation_rectangles[0][0] + 24, simulation_rectangles[0][1]+ 12, 64, WHITE); // Wall button (BLUE)
			DrawText("3", simulation_rectangles[162][0] + 24, simulation_rectangles[162][1]+ 12, 60, WHITE); // Path button (GREEN)
			// Draw the directional arrows on the "3" green GUI buttons
			DrawIcon(RICON_ARROW_TOP_FILL, (Vector2) { simulation_rectangles[162][0] + 25, simulation_rectangles[162][1] - (0.1*(SIMULATION_CELL)) }, 2, ORANGE);
			DrawIcon(RICON_ARROW_LEFT_FILL, (Vector2) { simulation_rectangles[162][0] - 4, simulation_rectangles[162][1] + (0.28*(SIMULATION_CELL)) }, 2, ORANGE);
			DrawIcon(RICON_ARROW_RIGHT_FILL, (Vector2) { simulation_rectangles[162][0] + 52, simulation_rectangles[162][1] + (0.28*(SIMULATION_CELL)) }, 2, ORANGE);
			DrawIcon(RICON_ARROW_BOTTOM_FILL, (Vector2) { simulation_rectangles[162][0] + 25, simulation_rectangles[162][1] + (0.7*(SIMULATION_CELL)) }, 2, ORANGE);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}