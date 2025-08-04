#include "treasure.h"

// 初始化游戏状态（隐藏起点）{insert\_element\_7\_}
void init_game(GameState* game) {
    game->map[game->player_y][game->player_x] = ' '; // 起点替换为陆地（隐藏）
}

// 打印地图（隐藏宝藏，显示玩家位置）{insert\_element\_8\_}
void print_map(GameState* game) {
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            if (i == game->player_y && j == game->player_x) {
                printf("X"); // 玩家位置标记
            } else {
                // 隐藏宝藏，显示其他元素
                char c = game->map[i][j];
                printf("%c", (c == 'H') ? ' ' : c);
            }
        }
        printf("\n");
    }
}

// 处理玩家输入（移动或显示地图）{insert\_element\_9\_}
int handle_input(GameState* game, char input) {
    int new_x = game->player_x;
    int new_y = game->player_y;

    // 解析输入（忽略大小写）
    switch (toupper(input)) {
        case 'W': new_y--; break; // 上移
        case 'S': new_y++; break; // 下移
        case 'A': new_x--; break; // 左移
        case 'D': new_x++; break; // 右移
        case 'M': print_map(game); return 0; // 显示地图
        default: return 0; // 无效输入
    }

    // 检查移动合法性（边界、障碍物）
    if (new_x < 0 || new_x >= game->size || new_y < 0 || new_y >= game->size) {
        return 0; // 超出边界
    }
    char target = game->map[new_y][new_x];
    if (target == 'w' || target == 'T') {
        return 0; // 水或棕榈树不可通过
    }

    // 执行移动
    game->player_x = new_x;
    game->player_y = new_y;

    // 检查是否找到宝藏{insert\_element\_10\_}
    if (target == 'H') {
        game->found_treasures++;
        game->map[new_y][new_x] = ' '; // 移除宝藏（避免重复找到）
        printf("You have found %d out of 3 hidden treasures.\n", game->found_treasures);

        // 检查是否找到所有宝藏（游戏结束）{insert\_element\_11\_}
        if (game->found_treasures == 3) {
            return 1; // 游戏胜利
        }
    }

    return 0;
}