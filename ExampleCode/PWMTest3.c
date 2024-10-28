#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PWM_PIN18 1    // BCM_GPIO 18, Physical Pin 12
#define INPUT_PIN 7  // BCM_GPIO 4, Physical Pin 7
#define PWM_PIN12 26 // BCM 12, wipi 26
#define PWM_PIN13 23 // 13, 23
#define PWM_PIN19 24 // 19, 24

void setup() {
    if (wiringPiSetup() == -1) {
        printf("wiringPi 초기화 실패\n");
        exit(1);
    }

    pinMode(PWM_PIN18, PWM_OUTPUT);
    pinMode(PWM_PIN12, PWM_OUTPUT);
    pinMode(PWM_PIN13, PWM_OUTPUT);
    pinMode(PWM_PIN19, PWM_OUTPUT);
    pinMode(INPUT_PIN, INPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(128);
    pwmSetClock(32);
}

void logPWMInput() {
    int value;
    int highCount = 0;
    int lowCount = 0;
    int totalCount = 128; // 측정할 샘플 수

    for (int i = 0; i < totalCount; i++) {
        value = digitalRead(INPUT_PIN);

        if (value == HIGH) {
            highCount++;
        } else {
            lowCount++;
        }

        delayMicroseconds(100); // 100us마다 샘플링
    }

    printf("HIGH Count: %d, LOW Count: %d\n", highCount, lowCount);
    printf("HIGH Percentage: %.2f%%, LOW Percentage: %.2f%%\n",
           (highCount / (float)totalCount) * 100,
           (lowCount / (float)totalCount) * 100);
}

int main(void) {
    setup();
    for (int i = 0; i < 128; i++) {
    pwmWrite(PWM_PIN13, i); // 50% duty cycle
    printf("input value : %d\n", i);
    
    logPWMInput();
    sleep(1); // 1초마다 로그 출력
}

    return 0;
}
