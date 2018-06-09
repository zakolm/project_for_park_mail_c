#include <stdio.h>
#include <math.h>

#define ABS(x) ((x) < 0 ? (-1 * x) : x)
#define MAX_MATR_SIZE 20000

typedef char MatrT[MAX_MATR_SIZE * 2 / 4]; // x2 to store one cell as 2 bits
static MatrT matrixes[4] = {0};

struct cell
{
    char *matr;
    int byteNum;
    int bitNum;
};

static char *getMatr(int x, int y)
{
    if (x < 0 && y < 0)
    {
        return matrixes[0];
    }
    if (x < 0 && y > 0)
    {
        return matrixes[1];
    }
    if (x > 0 && y < 0)
    {
        return matrixes[2];
    }

    return matrixes[3];
}

static struct cell getCell(int x, int y)
{
    char *m = getMatr(x, y);
    x = ABS(x);
    y = ABS(y);
 
    // 1 byte contain 4 cells
    // Find cell index
    int cell = x * 4 + y * 2;
 
    // Find a byte and bif for specific cell
    int byteNum = cell / 8;
    int bitNum = cell - byteNum * 8;
 
    struct cell c =
    {
        .matr = m,
        .byteNum = byteNum,
        .bitNum = bitNum,
    };

    return c;
}

#define EMPTY 0     // 00
#define FILLED 1    // 01
#define WALL 2      // 10
#define CELL_MASK 3 // 11
#define DELWALL -2

static void setCell(int x, int y, int value)
{
    struct cell c = getCell(x, y);
    c.matr[c.byteNum] |= value << c.bitNum;
    if (value == EMPTY)
        c.matr[c.byteNum] = 0;
}

static void setWall(int x, int y)
{
    setCell(x, y, WALL);
}

static void setFilled(int x, int y)
{
    setCell(x, y, FILLED);
}

static void setEmpty(int x, int y)
{
    setCell(x, y, EMPTY);
}

static int getValue(int x, int y)
{
    struct cell c = getCell(x, y);
    int value = c.matr[c.byteNum] & (CELL_MASK << c.bitNum);
    return value >> c.bitNum;
}

struct val
{
    int x;
    int y;
};

void print_all(struct val *tests, int count)
{
    int count_tmp = count / 10;
    for (int i = 0; i < count; ++i)
    {
        int item = getValue(tests[i].x, tests[i].y);
        printf("%d %d %d\n", item, tests[i].x,tests[i].y);
    }
    printf("\n");
}

int local_map(int x, int y, struct val *maps, int *size)
{
    printf("coordinats: %d %d\n", x, y);
    int rc;
    scanf("%d", &rc);
    printf("\n%d\n", rc);
    if (rc != FILLED)
    {
        if (rc == WALL)
        {
            setWall(x, y);
        }
        maps[*size].x = x; maps[*size].y = y;
        (*size)++;
    }

    return rc;
}

int main(void)
{
    setbuf(stdout, NULL);
    struct val map_stack[100] = {0};
    map_stack[0].x = 1; map_stack[0].y = 1; int size = 1;
    struct val maps[100] = {0};
    maps[0].x = 1; maps[0].y = 1;
    int size_map = 1;
    while (size)
    {
        int robot_pos[2];
        robot_pos[0] = map_stack[0].x; robot_pos[1] = map_stack[0].y;

        // pop
        map_stack[0].x = map_stack[1].x; map_stack[0].y = map_stack[1].y;
        for (int i = 1; i < 99; ++i)
        {
            map_stack[i].x = map_stack[i + 1].x; map_stack[i].y = map_stack[i + 1].y;
        }
        map_stack[99].x = 0; map_stack[99].y = 0;
        size--;

        int x_min = robot_pos[0];
        while (/*getValue(x_min - 1, robot_pos[1]) == EMPTY && */local_map(x_min - 1, robot_pos[1], maps, &size_map) != WALL)//(local_map[x_min - 1][robot_pos[1]] != 1))
        {
            x_min--;
        }

        int x_max = robot_pos[0];
        while (/*getValue(x_max + 1, robot_pos[1]) == EMPTY && */local_map(x_max + 1, robot_pos[1], maps, &size_map) != WALL)//(local_map[x_max + 1][robot_pos[1]] != 1))
        {
            x_max++;
        }

        print_all(maps, size_map);
        for (int x = x_min; x <= x_max; ++x)
        {
            //printf("check %d %d\n", map_stack[x].x, map_stack[x].y);
            //int item = getValue(x, robot_pos[1]);
            //if (item == WALL)
            //    setEmpty(x, robot_pos[1]);
            //printf("check %d %d %d\n", item, x, robot_pos[1]);
            setFilled(x, robot_pos[1]);//map_stack[x].x, map_stack[x].y);
            //maps[size_map].x = x_max; maps[size_map].x = robot_pos[1];
            //size_map++;
        }
        //printf("%d %d %d\n", size, x_min, x_max);
        //printf("\n");
        
        int flag_color = 1;
        for (int x = x_min; x <= x_max; ++x)
        {
            int item = getValue(x, robot_pos[1] - 1);
            printf("supercheck%d\n", item);
            if (item == EMPTY && local_map(x, robot_pos[1] - 1, maps, &size_map) == EMPTY)//local_map[x][robot_pos[1] - 1] != 1 && local_map[x][robot_pos[1] - 1] == 0)
            {
                if (flag_color)
                {
                    //printf("check %d %d\n", x, robot_pos[1]);
                    map_stack[size].x = x; map_stack[size].y = robot_pos[1] - 1;
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
            if (getValue(x, robot_pos[1] + 1) == EMPTY && local_map(x, robot_pos[1] + 1, maps, &size_map) == 0)//local_map[x][robot_pos[1] + 1] != 1 && local_map[x][robot_pos[1] + 1] == 0)
            {
                if (flag_color)
                {
                    //printf("check1 %d %d\n", x, robot_pos[1]);
                    map_stack[size].x = x; map_stack[size].y = robot_pos[1] + 1;
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
    }

    /*for (int i = 0; i < sizeof(maps)/sizeof(*maps); ++i)
    {
        printf("%d %d %d\n", maps[i].x, maps[i].y, size_map);
    }*/
    print_all(maps, size_map);

    /*
    struct val tests[] = {
        {  0,     0    },
        {  -10,   0    },
        {  0,     -10  },
        {  10,    0    },
        {  0,     10   },
        {  1,     1    },
        {  10,    -1   },
        {  -10,   23   },
        {  -47,   -53  },
        {  1,     1    },
        {  2,     2    },
        {  3,     3    },
        {  4,     4    },
        {  5,     5    },
        {  6,     6    },
        {  7,     7    },
        {  8,     8    },
        {  9,     9    },
        {  10,    10   },
        {  11,    11   },
        {  12,    12   },
        {  13,    13   },
        {  14,    14   },
        {  15,    15   },
        {  16,    16   },
        {  17,    17   },
        {  18,    18   },
        {  19,    19   },
        {  20,    20   },
        {  21,    21   },
        {  22,    22   },
        {  23,    23   },
        {  24,    24   },
        {  25,    25   },
        {  26,    26   },
        {  27,    27   },
        {  28,    28   },
        {  29,    29   },
        {  30,    30   },
        {  31,    31   },
        {  32,    32   },
        {  33,    33   },
        {  34,    34   },
        {  35,    35   },
        {  36,    36   },
        {  37,    37   },
        {  38,    38   },
        {  39,    39   },
        {  40,    40   },
        {  -41,   41   },
        {  -42,   42   },
        {  -43,   43   },
        {  -44,   44   },
        {  -45,   45   },
        {  -46,   46   },
        {  -47,   47   },
        {  -48,   48   },
        {  -49,   49   },
        {  -50,   50   },
        {  -51,   51   },
        {  -52,   52   },
        {  -53,   53   },
        {  -54,   54   },
        {  -55,   55   },
        {  -56,   56   },
        {  -57,   57   },
        {  -58,   58   },
        {  -59,   59   },
        {  -60,   60   },
        {  -61,   61   },
        {  -62,   62   },
        {  -63,   63   },
        {  -64,   64   },
        {  -65,   65   },
        {  -66,   66   },
        {  -67,   67   },
        {  -68,   68   },
        {  -69,   69   },
        {  -70,   70   },
        {  -71,   71   },
        {  -72,   72   },
        {  -73,   73   },
        {  -74,   74   },
        {  -75,   75   },
        {  -76,   76   },
        {  -77,   77   },
        {  -78,   78   },
        {  -79,   79   },
        {  -80,   80   },
        {  -81,   81   },
        {  -82,   82   },
        {  -83,   83   },
        {  -84,   84   },
        {  -85,   85   },
        {  -86,   86   },
        {  -87,   87   },
        {  -88,   88   },
        {  -89,   89   },
        {  -90,   90   },
        {  -91,   91   },
        {  -92,   92   },
        {  -93,   93   },
        {  -94,   94   },
        {  -95,   95   },
        {  -96,   96   },
        {  -97,   97   },
        {  -98,   98   },
        {  -99,   99   },
        {  -100,  100  },
    };

    printf("\n\nFILLING\n\n");
 
    for (int i = 0; i < sizeof(tests) / sizeof(*tests) - 1; ++i) {
        if (i % 2) {
            printf("% 4d x % 4d is filled\n", tests[i].x, tests[i].y);
            setFilled(tests[i].x, tests[i].y);
        } else {
            printf("% 4d x % 4d is a wall\n", tests[i].x, tests[i].y);
            setWall(tests[i].x, tests[i].y);
        }
    }

    printf("\n\nTESTING\n\n");
 
    for (int i = 0; i < sizeof(tests) / sizeof(*tests); ++i) {
        int value = getValue(tests[i].x, tests[i].y);

        const char *type = "UNKNOWN";
        if (value == WALL)
            type = "a wall";
        else if (value == FILLED)
            type = "filled";
        else if (value == EMPTY)
            type = "empty";

        printf("% 4d x % 4d is %s\n", tests[i].x, tests[i].y, type);
    }
    
    print_all(tests, sizeof(tests) / sizeof(*tests));
    */

    return 0;
}