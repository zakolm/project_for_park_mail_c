#include <stdio.h>

/*void pop(int ***array)
{
	//int **tmp_array = *array;
	int **a = *array;

	//int tmp_array[6][10];
	//int *b = (int*) tmp_array;
	//b[0] = a[1];
	a[0] = a[1];
	for (int i = 1; i < 5; ++i)
	{
		//for (int j = 0; j < 10; )
		a[i] = a[i + 1];
	}

	//*array = tmp_array;
}*/
void print_list(int local_map[6][10])
{
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 10; ++j)
			printf("%d ", local_map[i][j]);
	}
	printf("\n");
}
void main(void)
{
	int local_map[6][10] =
	{
		{ 0, 1, 1, 1, 0, 0, 1, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 1, 3, 1, 1, 0 },
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 }
	};
	int map_stack[60][2];
	map_stack[0][0] = 2; map_stack[0][1] = 6;
	int size = 1;

	while (map_stack[0][0] != 0 && map_stack[0][1] != 0)
	{
		int robot_pos[2];
		robot_pos[0] = map_stack[0][0]; robot_pos[1] = map_stack[0][1];
		// pop
		map_stack[0][0] = map_stack[1][0]; map_stack[0][1] = map_stack[1][1];
		for (int i = 1; i < 59; ++i)
		{
			map_stack[i][0] = map_stack[i + 1][0]; map_stack[i][1] = map_stack[i + 1][1];
		}
		map_stack[59][0] = 0; map_stack[59][1] = 0;

		int x_min = robot_pos[0];
		while ((x_min > 0) && (local_map[x_min - 1][robot_pos[1]] != 1))
			x_min--;

		int x_max = robot_pos[0];
		while ((x_max < 5) && (local_map[x_max - 1][robot_pos[1]] != 1))
			x_max++;

		for (int x = x_min; x <= x_max; ++x)
		{
			local_map[x][robot_pos[1]] = 2;
		}

		int flag_color = 1;
		for (int x = x_min; x <= x_max; ++x)
		{
			if (local_map[x][robot_pos[1] - 1] != 1 && local_map[x][robot_pos[1] - 1] == 0)
			{
				if (flag_color)
				{
					map_stack[size][0] = x; map_stack[size][1] = robot_pos[1] - 1;
					size++;
					flag_color = 0;
					//forward();
					//delay(1000);
					//stop();
				}
			}
			else
			{
				flag_color = 1;
			}
		}

		flag_color = 1;
		for (int x = x_min; x <= x_max; ++x)
		{
			if (local_map[x][robot_pos[1] + 1] != 1 && local_map[x][robot_pos[1] + 1] == 0)
			{
				if (flag_color)
				{
					map_stack[size][0] = x; map_stack[size][1] = robot_pos[1] + 1;
					size++;
					flag_color = 0;
					//forward();
					//delay(1000);
					//stop();
				}
			}
			else
			{
				flag_color = 1;
			}
		}
		print_list(local_map);
	}
}