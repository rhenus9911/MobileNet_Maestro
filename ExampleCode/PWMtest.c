#include <wiringPi.h>
#include <stdio.h>

#define LED 1

int bright = 0;

int main(void){
    wiringPiSetup();
    
    pinMode(LED, PWM_OUTPUT);
    digitalWrite(LED, LOW);
    
    
    while(1){
        for(int i = 0; i < 100; i++){
            pwmWrite(LED, i);
            delay(50);
                
        }
        for(int i = 0; i < 100; i++){
            pwmWrite(LED, i);
            delay(50);
        
        }
    }

}
