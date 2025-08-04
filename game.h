    #ifndef TREASURE_H
#define TREASURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 地图尺寸限制（5x5至100x100）{insert\_element\_0\_}
#define MIN_SIZE 5
#define MAX_SIZE 100

// 返回码定义{insert\_element\_1\_}
#define SUCCESS 0
#define ARG_ERROR 1
#define FILE_ERROR 2
#define DATA_ERROR 3

// 游戏状态结构体
typedef struct {
    char** map;         // 地图数据
    int size;           // 地图尺寸（边长）
    int player_x;       // 玩家X坐标
    int player_y;       // 玩家Y坐标
    int found_treasures;// 已找到宝藏数
    int total_treasures;// 总宝藏数（固定为3）
} GameState;

// 函数声明
int check_arguments(int argc, char* argv[], int* size);
int load_and_validate_map(const char* filename, int size, GameState* game);
void init_game(GameState* game);
void print_map(GameState* game);
int handle_input(GameState* game, char input);
void free_resources(GameState* game);

#endif