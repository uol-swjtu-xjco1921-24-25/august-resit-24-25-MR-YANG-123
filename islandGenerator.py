import sys
import random
from collections import deque

def validate_arguments():
    """验证命令行参数：需包含文件名和尺寸（5-100）"""
    if len(sys.argv) != 3:
        print("Usage: ./islandGenerator <filename> <dimension>", file=sys.stderr)
        sys.exit(1)
    
    try:
        dim = int(sys.argv[2])
        if not (5 <= dim <= 100):
            raise ValueError
    except ValueError:
        print("Dimension must be an integer between 5 and 100", file=sys.stderr)
        sys.exit(1)
    
    return sys.argv[1], dim

def create_base_map(dim):
    """创建基础地图：边缘全为'w'，内部初始化为空"""
    map_data = [['w' for _ in range(dim)] for _ in range(dim)]
    # 填充内部区域（非边缘）
    for i in range(1, dim-1):
        for j in range(1, dim-1):
            map_data[i][j] = ' '  # 先默认全为陆地
    return map_data

def add_obstacles(map_data, dim):
    """添加棕榈树（'T'），使陆地占比约60%"""
    inner_size = (dim-2) * (dim-2)  # 内部格子总数（除去边缘）
    land_count = int(inner_size * 0.6)  # 60%为陆地
    current_land = inner_size  # 初始全为陆地
    obstacle_count = current_land - land_count  # 需要添加的障碍物数量

    # 随机添加障碍物
    obstacles_added = 0
    while obstacles_added < obstacle_count:
        i = random.randint(1, dim-2)
        j = random.randint(1, dim-2)
        if map_data[i][j] == ' ':  # 仅在陆地添加
            map_data[i][j] = 'T'
            obstacles_added += 1
    return map_data

def place_key_points(map_data, dim):
    """放置起点（'S'）和3个宝藏（'H'），确保在陆地上"""
    # 收集所有陆地位置（空格）
    land_positions = []
    for i in range(1, dim-2):  # 避开边缘内侧，留缓冲
        for j in range(1, dim-2):
            if map_data[i][j] == ' ':
                land_positions.append((i, j))
    
    if len(land_positions) < 4:  # 至少需要4个陆地位置（1S+3H）
        raise ValueError("Not enough land for key points")
    
    # 随机选择位置
    positions = random.sample(land_positions, 4)
    s_pos = positions[0]
    h_positions = positions[1:]

    # 放置起点和宝藏
    map_data[s_pos[0]][s_pos[1]] = 'S'
    for (i, j) in h_positions:
        map_data[i][j] = 'H'
    
    return map_data, s_pos, h_positions

def is_reachable(map_data, dim, start, target):
    """用BFS检查起点到目标是否可达（避开'w'和'T'）"""
    visited = [[False for _ in range(dim)] for _ in range(dim)]
    q = deque([start])
    visited[start[0]][start[1]] = True

    # 上下左右四个方向
    directions = [(-1,0), (1,0), (0,-1), (0,1)]
    
    while q:
        i, j = q.popleft()
        if (i, j) == target:
            return True
        for di, dj in directions:
            ni, nj = i + di, j + dj
            # 检查是否在内部区域且未访问、可通行（不是水或树）
            if 1 <= ni <= dim-2 and 1 <= nj <= dim-2:
                if not visited[ni][nj] and map_data[ni][nj] != 'w' and map_data[ni][nj] != 'T':
                    visited[ni][nj] = True
                    q.append((ni, nj))
    return False

def ensure_solvable(map_data, dim, s_pos, h_positions):
    """确保起点到所有宝藏都有路径，否则移除部分障碍物"""
    for h_pos in h_positions:
        if not is_reachable(map_data, dim, s_pos, h_pos):
            # 尝试移除路径上的障碍物（简单策略：随机移除附近的'T'）
            i, j = h_pos
            # 在宝藏周围3x3范围内找障碍物移除
            for di in [-1, 0, 1]:
                for dj in [-1, 0, 1]:
                    ni, nj = i + di, j + dj
                    if 1 <= ni <= dim-2 and 1 <= nj <= dim-2:
                        if map_data[ni][nj] == 'T':
                            map_data[ni][nj] = ' '  # 移除障碍物
            # 再次检查
            if not is_reachable(map_data, dim, s_pos, h_pos):
                # 若仍不可达，重新生成地图（简化处理）
                raise ValueError("Map unsolvable, regenerating...")
    return map_data

def save_map(map_data, dim, filename):
    """将地图写入文件"""
    with open(filename, 'w') as f:
        for row in map_data:
            f.write(''.join(row) + '\n')  # 每行末尾添加换行符

def generate_map(filename, dim):
    """生成完整地图的主函数"""
    max_attempts = 5  # 最大尝试次数（防止无限循环）
    for _ in range(max_attempts):
        try:
            map_data = create_base_map(dim)
            map_data = add_obstacles(map_data, dim)
            map_data, s_pos, h_positions = place_key_points(map_data, dim)
            map_data = ensure_solvable(map_data, dim, s_pos, h_positions)
            save_map(map_data, dim, filename)
            print(f"Successfully generated {dim}x{dim} map: {filename}")
            return
        except ValueError as e:
            print(e)
    print("Failed to generate a valid map after multiple attempts", file=sys.stderr)
    sys.exit(1)

if __name__ == "__main__":
    filename, dim = validate_arguments()
    generate_map(filename, dim)