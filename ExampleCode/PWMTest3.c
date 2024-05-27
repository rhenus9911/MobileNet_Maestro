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
    printf("PWM Test Start\n");
    pinMode(GPIO_PIN, INPUT);
    pinMode(4, OUTPUT);
    softPwmCreate(4, 0, 100); // pwmRange
    int cnt = 0;
    for(int i = 0; i < 5; i++){
        softPwmWrite(4, i); // value
        int frequency = measureFrequency(4); // value / pwmRange = dutycylce
        printf("Measured Frequency: %d Hz\n", frequency);
        if(i == 0 && frequency != 0) continue;
        if(frequency != 0) cnt++;
        delay(1000);  // 1초 대기
        
    }
    if(cnt > 3) printf("Success\n");
    else printf("Failed\n");
    printf("PWM Finshed\n");
    //printf("%d", cnt);
  

    return 0;
}
