
main:
	gcc -Wall src/main.c -L./libs -I./include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
