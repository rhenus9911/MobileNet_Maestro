#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <softPwm.h>
#define GPIO_PIN 18  // wiringPi 핀 번호 설정

int measureFrequency(int pin) {
    int previousState = digitalRead(pin);
    int eventCount = 0;
    struct timespec startTime, currentTime;

    // 현재 시간을 startTime에 저장합니다.
    clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);
    
    double elapsedTime = 0;
    
    // 1초 동안 상태 변화를 감지합니다.
    while (elapsedTime < 1.0) {
        int currentState = digitalRead(pin);
        if (currentState != previousState) {
            eventCount++;
            previousState = currentState;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &currentTime);
        elapsedTime = (currentTime.tv_sec - startTime.tv_sec) 
                    + (currentTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;
    }
    
    // 주파수 계산 (사이클은 상태가 2번 변경되어야 하므로 eventCount를 2로 나눕니다)
    return eventCount / 2;
}

int main(void) {
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi\n");
        return 1;
    }

    pinMode(GPIO_PIN, INPUT);
    pinMode(4, OUTPUT);
    softPwmCreate(4, 0, 100);
    int cnt = 0;
    for(int i = 0; i < 10; i++){
        softPwmWrite(4, i);
        int frequency = measureFrequency(4);
        //printf("Measured Frequency: %d Hz\n", frequency);
        if(frequency != 0) cnt++;
        delay(1000);  // 1초 대기
        
    }
    printf("%d", cnt);
  

    return 0;
}
