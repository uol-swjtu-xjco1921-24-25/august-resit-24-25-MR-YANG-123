#include "treasure.h"

// 验证命令行参数合法性{insert\_element\_2\_}
int check_arguments(int argc, char* argv[], int* size) {
    // 检查参数数量
    if (argc != 3) {
        return ARG_ERROR;
    }

    // 解析并验证尺寸
    char* endptr;
    *size = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || *size < MIN_SIZE || *size > MAX_SIZE) {
        return ARG_ERROR;
    }

    return SUCCESS;
}

// 加载地图文件并验证合法性{insert\_element\_3\_}
int load_and_validate_map(const char* filename, int size, GameState* game) {
    // 打开文件
    FILE* file = fopen(filename, "r");
    if (!file) {
        return FILE_ERROR;
    }

    // 分配地图内存
    game->map = (char**)malloc(size * sizeof(char*));
    if (!game->map) {
        fclose(file);
        return -1;
    }
    for (int i = 0; i < size; i++) {
        game->map[i] = (char*)malloc(size + 1); // +1 用于存储换行符
        if (!game->map[i]) {
            // 内存分配失败时释放已分配空间
            for (int j = 0; j < i; j++) free(game->map[j]);
            free(game->map);
            fclose(file);
            return -1;
        }
    }

    // 读取地图内容
    for (int i = 0; i < size; i++) {
        if (!fgets(game->map[i], size + 1, file)) {
            // 读取失败（行数不足）
            free_resources(game);
            fclose(file);
            return DATA_ERROR;
        }
        // 移除换行符
        game->map[i][strcspn(game->map[i], "\n")] = '\0';
        // 检查行长度
        if (strlen(game->map[i]) != size) {
            free_resources(game);
            fclose(file);
            return DATA_ERROR;
        }
    }
    fclose(file);

    // 验证地图内容
    int start_count = 0;
    int treasure_count = 0;
    game->size = size;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            char c = game->map[i][j];

            // 验证边缘是否为水{insert\_element\_4\_}
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1) {
                if (c != 'w') {
                    free_resources(game);
                    return DATA_ERROR;
                }
            }

            // 验证字符合法性{insert\_element\_5\_}
            if (c != ' ' && c != 'w' && c != 'T' && c != 'H' && c != 'S') {
                free_resources(game);
                return DATA_ERROR;
            }

            // 统计起点和宝藏{insert\_element\_6\_}
            if (c == 'S') {
                start_count++;
                game->player_x = j;
                game->player_y = i;
            } else if (c == 'H') {
                treasure_count++;
            }
        }
    }

    // 验证起点和宝藏数量
    if (start_count != 1 || treasure_count != 3) {
        free_resources(game);
        return DATA_ERROR;
    }

    game->total_treasures = 3;
    game->found_treasures = 0;
    return SUCCESS;
}

// 释放动态分配的资源
void free_resources(GameState* game) {
    for (int i = 0; i < game->size; i++) {
        free(game->map[i]);
    }
    free(game->map);
}