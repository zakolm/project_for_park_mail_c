#include <iarduino_HC_SR04_int.h>                      //ultrasonic sensor
const uint8_t pinSens_TRIG = 2; 
const uint8_t pinSens_ECHO = 3; 
iarduino_HC_SR04_int hcsr(pinSens_TRIG, pinSens_ECHO); //initialisation of object hcsr (ultrasonic sensor)

const uint8_t pinShield_LE = 6;                        //initialaising pins for motors
const uint8_t pinShield_LH = 7;
const uint8_t pinShield_RE = 5;
const uint8_t pinShield_RH = 4;
const bool    arrRoute[2]  = {1, 1};                   //direction of motor's rotation
const uint8_t valSpeed = 200;                          // max speed 255
uint8_t       arrSpeed[2];             
const uint8_t valDistance  = 20;                       //the distance to the obstacle
uint32_t      tmrMillis;                               //record time

void setup(void)
{
    arrSpeed[1]=valSpeed;                              //speed of motors
    arrSpeed[0]=valSpeed;
    
    pinMode(pinShield_LE, OUTPUT);                     //5V output
    pinMode(pinShield_LH, OUTPUT);
    pinMode(pinShield_RE, OUTPUT);
    pinMode(pinShield_RH, OUTPUT); 
    
    digitalWrite(pinShield_LH, arrRoute[1]);
    digitalWrite(pinShield_RH, arrRoute[0]);
    tmrMillis = millis();
}

void pop(int ***array)
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
		a[i] = a[i+1];
	}
	
	//*array = tmp_array;
}

void forward(void)
{
	
	analogWrite(pinShield_LE, 155);          //signal-to-speed convertion
    analogWrite(pinShield_RE, 155);
}

void stop(void)
{
	analogWrite(pinShield_LE, 0);          //signal-to-speed convertion
    analogWrite(pinShield_RE, 0);
}

void loop(void)
{
  analogWrite(pinShield_LE, 155);          //signal-to-speed convertion
  analogWrite(pinShield_RE, 155);
  delay(5000);
  analogWrite(pinShield_LE, 0);          //signal-to-speed convertion
  analogWrite(pinShield_RE, 0);
  int local_map[6][10] =
				{
				{0, 1, 1, 1, 0, 0, 1, 0, 0, 1},
				{1, 0, 0, 0, 1, 1, 3, 1, 1, 0},
				{1, 0, 0, 0, 0, 1, 0, 0, 0,1},
				{0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
				{0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
				{0, 0, 0, 1, 1, 1, 1, 1, 0, 0}
				};
	int map_stack[60][2];
	map_stack[0][0] = 1; map_stack[0][1] = 6;
	int size = 1;
	
	while (map_stack[0][0] != 0 && map_stack[0][1] != 0)
	{
		int robot_pos[2];
   robot_pos[0] = map_stack[0][0]; robot_pos[1] = map_stack[0][1];
		// pop
    int **a = map_stack[0][0];
		pop(&a);
		
		int x_min = robot_pos[0];
		while ((x_min > 0) && (local_map[x_min - 1][robot_pos[1]] != 1))
			x_min--;
		
		int x_max = robot_pos[0];
		while ((x_max < 6) && (local_map[x_min - 1][robot_pos[1]] != 1))
			x_max++;
		
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
					forward();
					delay(1000);
					stop();
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
					forward();
					delay(1000);
					stop();
				}
			}
			else
			{
				flag_color = 1;
			}
		}
	}
    //tmrMillis = millis() - tmrMillis;                 //for timing
    /*if(tmrMillis > 10000)
    {
        arrSpeed[1]=0;                    
        arrSpeed[0]=0;
         
    }
    
    if (hcsr.distance()<valDistance )                 //stop if it's obstacle
    {
         arrSpeed[1]=0;                    
         arrSpeed[0]=0;
    }else
    { //продолжаем движение
            arrSpeed[1]=valSpeed;                     //go 
            arrSpeed[0]=valSpeed;
    }
    
    analogWrite(pinShield_LE, arrSpeed[1]);          //signal-to-speed convertion
    analogWrite(pinShield_RE, arrSpeed[0]);
    */
}



