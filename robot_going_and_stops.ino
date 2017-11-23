#include <iarduino_HC_SR04_int.h>                      //ultrasonic sensor
const uint8_t pinSens_TRIG = 2; 
const uint8_t pinSens_ECHO = 3; 
iarduino_HC_SR04_int hcsr(pinSens_TRIG, pinSens_ECHO); //initialisation of object hcsr (ultrasonic sensor)

const uint8_t pinShield_LE = 6;                        //initialaising pins for motors
const uint8_t pinShield_LH = 7;
const uint8_t pinShield_RE = 5;
const uint8_t pinShield_RH = 4;
const bool    arrRoute[2]  = {1, 1};                   //direction of motor's rotation
const uint8_t valSpeed = 255;                          // max speed 255
uint8_t       arrSpeed[2];             
const uint8_t valDistance  = 10;                       //the distance to the obstacle
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

void loop(void)
{
    tmrMillis = millis() - tmrMillis;                 //for timing
    if(tmrMillis > 10000)
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
    
}



