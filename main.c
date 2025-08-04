#include "treasure.h"

int main(int argc, char* argv[]) {
    GameState game = {0};
    int size;

    // 验证命令行参数
    if (check_arguments(argc, argv, &size) != SUCCESS) {
        fprintf(stderr, "Argument error\n");
        return ARG_ERROR;
    }

    // 加载并验证地图
    int load_result = load_and_validate_map(argv[1], size, &game);
    if (load_result != SUCCESS) {
        fprintf(stderr, load_result == FILE_ERROR ? "File error\n" : "Data error\n");
        return load_result;
    }

    // 初始化游戏
    init_game(&game);
    printf("Treasure hunt game started! Use WASD to move, M to show map.\n");

    // 游戏主循环
    char input;
    while (1) {
        printf("Enter move: ");
        if (scanf(" %c", &input) != 1) { // 读取输入（忽略前导空格）
            break;
        }

        // 处理输入并检查游戏是否结束
        if (handle_input(&game, input) == 1) {
            printf("Congratulations! You found all treasures!\n");
            break;
        }
    }

    // 释放资源
    free_resources(&game);
    return SUCCESS;
}