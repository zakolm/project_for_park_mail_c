#include <stdio.h>
#include <math.h>
#include <iarduino_HC_SR04_int.h>                      //ultrasonic sensor

#define ABS(x) ((x) < 0 ? (-1 * x) : x)
#define MAX_MATR_SIZE 10
#define EMPTY 0     // 00
#define FILLED 1    // 01
#define WALL 2      // 10
#define CELL_MASK 3 // 11
#define DELWALL -2

const uint8_t pinSens_TRIG = 12; 
const uint8_t pinSens_ECHO = 11; 
iarduino_HC_SR04_int hcsr(pinSens_TRIG, pinSens_ECHO); //initialisation of object hcsr (ultrasonic sensor)

const uint8_t pinShield_LE = 6;                        //initialaising pins for motors
const uint8_t pinShield_LH = 7;
const uint8_t pinShield_RE = 5;
const uint8_t pinShield_RH = 4;
const bool    arrRoute[2]  = {0, 1};                   //direction of motor's rotation
const uint8_t valSpeed = 100;                          // max speed 255
uint8_t       arrSpeed[2];             
const uint8_t valDistance  = 10;                       //the distance to the obstacle
uint32_t      tmrMillis;                               //record time

typedef char MatrT[MAX_MATR_SIZE]; // x2 to store one cell as 2 bits
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

int local_map(int x, int y, struct val *maps, int *size_r)
{
    //printf("coordinats: %d %d\n", x, y);
    int rc = EMPTY;
    //scanf("%d", &rc);
    //printf("\n%d\n", rc);
    if (rc != FILLED)
    {
        if (rc == WALL)
        {
            setWall(x, y);
        }
        maps[*size_r].x = x; maps[*size_r].y = y;
        (*size_r)++;
    }

    return rc;
}

void pop(struct val map_stack[], int *size_r)
{
    // pop
    map_stack[0].x = map_stack[1].x; map_stack[0].y = map_stack[1].y;
    for (int i = 1; i < 99; ++i)
    {
        map_stack[i].x = map_stack[i + 1].x; map_stack[i].y = map_stack[i + 1].y;
    }
    map_stack[99].x = 0; map_stack[99].y = 0;
    (*size_r)--;
}

void streight(void);
void left(void);
void right(void);
void stop_motor(void);
void com_back(void);

void setup(void)
{
    arrSpeed[1]=valSpeed;                              //speed of motors
    arrSpeed[0]=valSpeed;
    
    pinMode(pinShield_LE, OUTPUT);                     //5V output
    pinMode(pinShield_LH, OUTPUT);
    pinMode(pinShield_RE, OUTPUT);
    pinMode(pinShield_RH, OUTPUT); 
    
    digitalWrite(pinShield_LH, arrRoute[1]);
    digitalWrite(pinShield_RH, arrRoute[1]);
    tmrMillis = millis();
}

void loop(void)
{
   struct val map_stack[100] = {0};
    map_stack[0].x = 1; map_stack[0].y = 1; int size_r = 1;
    struct val maps[100] = {0};
    maps[0].x = 1; maps[0].y = 1; int size_map = 1;
    while (size_r)
    {
        int robot_pos[2];
        robot_pos[0] = map_stack[0].x; robot_pos[1] = map_stack[0].y;

        // pop
        pop(map_stack, &size_r);

        int x_min = robot_pos[0];
        int time_r = 0;
        while (time_r < 3 &&/*getValue(x_min - 1, robot_pos[1]) == EMPTY && */local_map(x_min - 1, robot_pos[1], maps, &size_map) != WALL)//(local_map[x_min - 1][robot_pos[1]] != 1))
        {
          time_r++;
          streight();
          delay(1000);
          stop_motor();
            x_min--;
        }
        right();
        stop_motor();
        com_back();
        stop_motor();
        left();
        stop_motor();
        int x_max = robot_pos[0];
        time_r = 0;
        while (time_r < 6 && /*getValue(x_max + 1, robot_pos[1]) == EMPTY && */local_map(x_max + 1, robot_pos[1], maps, &size_map) != WALL)//(local_map[x_max + 1][robot_pos[1]] != 1))
        {
            x_max++;
            time_r++;
            streight();
            delay(1000);
            stop_motor();
            x_min--;
            right();
            stop_motor();
            com_back();
            stop_motor();
            right();
            stop_motor();
        }

    }
    exit(1);
}

void com_back(void)
{
  analogWrite(pinShield_LE, 0);          //signal-to-speed convertion
  analogWrite(pinShield_RE, 250);//arrSpeed[1]);
  delay(800);
}
void streight()
{
  analogWrite(pinShield_LE, arrSpeed[1]);//signal-to-speed convertion
  analogWrite(pinShield_RE, arrSpeed[1]);
  delay(550);
}

void left()
{
  analogWrite(pinShield_LE, 0);          //signal-to-speed convertion
  analogWrite(pinShield_RE, 250);//arrSpeed[1]);
  delay(400);
  stop_motor();
}
void right()
{
  analogWrite(pinShield_RE, 0);          //signal-to-speed convertion
  analogWrite(pinShield_LE, 250);//175);
  delay(400);
  stop_motor();
}
void stop_motor()
{
  analogWrite(pinShield_LE, 0);          //signal-to-speed convertion
  analogWrite(pinShield_RE, 0);
  delay(2000);
}

