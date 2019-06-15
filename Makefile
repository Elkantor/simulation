
main_linux:
	gcc -Wall src/main.c -L./libs -I./include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

main_windows:
	gcc -Wall src/main.c -L./libs/windows -I./include -lraylib -lglfw3 -lopengl32 -lgdi32