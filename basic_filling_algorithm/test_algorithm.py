# Заполняет 2 область, ограниченную 1
# Добавить затравочный пиксель в стек;
# 3 образно помечен старт
def print_map(local_map):
    for i in range(len(local_map)):
        for j in range(len(local_map[i])):
            print(local_map[i][j], end = ' ')
        print()

def main():
    '''
    local_map = [[0,1,1,1,1,0,0,1,0,0],
                 [1,0,0,0,0,1,1,3,1,0],
                 [1,0,0,0,0,1,0,0,0,1],
                 [0,1,0,0,0,0,0,0,0,1],
                 [0,0,1,0,0,0,0,0,1,0],
                 [0,0,0,1,1,1,1,1,0,0]]
    '''
    local_map = [[3,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0],
                 [0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0],
                 [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                 [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                 [0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0],
                 [0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0],
                 [0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0],
                 [0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0],
                 [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]]

    # map_stack = [[1,7]]
    map_stack = [[0,0]]
    print_map(local_map)
    while(len(map_stack)):
        # Извлечение пикселя из стека
        robot_pos = map_stack[0]; map_stack.pop(0)
        
        x_min = robot_pos[0]
        while((x_min > 0) and (local_map[x_min - 1][robot_pos[1]] != 1)):
            x_min -= 1
            
        x_max = robot_pos[0]
        while((x_max < len(local_map) - 1) and (local_map[x_max + 1][robot_pos[1]] != 1)):
            x_max += 1

        # Закрасить отрезок (X_min,Y)--(X_max,Y) цветом A;
        for x in range(x_min, x_max + 1):
            local_map[x][robot_pos[1]] = 2
            
        # Обработка строки сверху
        flag_color = True
        for x in range(x_min, x_max + 1):
            if (local_map[x][robot_pos[1] - 1] != 1 and local_map[x][robot_pos[1] - 1] == 0):
                if (flag_color):
                    map_stack.append([x, robot_pos[1] - 1])
                    flag_color = False
            else:
                flag_color = True
                
        # Обработка строки снизу
        flag_color = True
        for x in range(x_min, x_max + 1):
            if (local_map[x][robot_pos[1] + 1] != 1 and local_map[x][robot_pos[1] + 1] == 0):
                if (flag_color):
                    map_stack.append([x, robot_pos[1] + 1])
                    flag_color = False
            else:
                flag_color = True
                
        # print()
        # print_map(local_map)
        # print()

    print()
    print_map(local_map)
    print()

if __name__ == '__main__':
    main()
