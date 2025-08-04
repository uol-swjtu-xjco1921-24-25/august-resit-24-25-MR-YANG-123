CC = gcc
CFLAGS = -Wall -Wextra -std=c99

treasure_hunt: main.c map_utils.c game_logic.c
	$(CC) $(CFLAGS) -o island main.c map_utils.c game_logic.c

clean:
	rm -f island