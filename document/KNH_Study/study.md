# 주소가 매핑되는 과정
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/fb660d10-0e15-447f-96fc-06b6ccc37d13)


![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/07b2d6c1-67e9-4049-ade4-f359a42b2e4c)

35비트 주소쳬계를 쓰지만 c언어로 접근하기 위해서는 32비트를 써야한다.

1. ARM 코어에 의해 가상 주소로 발행된 다음
2. ARM MMU에 의해 물리적 주소로 매핑된 다음
3. RAM에서 적절한 주변 장치 또는 위치를 선택하는 데 사용된다.
   

# 어떻게 접근?
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/94a5e388-5e1b-48e0-98fa-d6e13715638a)


- GPLSEL :GPIO의 입출력을 결정하는거 해당 비트에 000을 주면 input 001 를 주면 아웃풋
    - GPLSEL0 : 0번핀부터 9번핀까지 오프셋: 0x00
        - FSEL9: 해당 비트 29:27
        - FSEL8: 해당 비트 26:24
        - FSEL7: 해당 비트 23:21
        - FSEL0: 해당 비트 2:0
    - GPLSEL1: 10번핀부터 19번핀까지 오프셋:0x04
        - FSEL19: 해당 비트 29:27
        - FSEL10: 해당 비트 2:0
    - GPLSEL2 : 20번핀부터 29번핀까지 오프셋:0x08
        - FSEL29: 해당 비트 29:27
        - FSEL20: 해당 비트 2:0
    - GPLSEL3: 30번핀부터 39번핀까지 오프셋:0x0c
        - FSEL39: 해당 비트 29:27
        - FSEL30: 해당 비트 2:0
    - GPLSEL4: 40번핀부터 49번핀까지 오프셋:0x10
    - GPLSEL5: 50번핀부터 59번핀까지 오프셋:0x14
 
- GPLSET: HIGH설정
    - GPLSET0 : 0번핀부터 31번핀까지 설정
        - 3번 비트를 1로 주면 3번 핀이 HIGH 4번 비트를 1로 주면 4번 핀이 HIGH
    - GPLSET1: 32번핀부터 57번핀까지 설정
        - 1번 비트를 1로 주면 32번 핀이 HIGH 3번 비트를 1로 주면 34번 핀이 HIGH

- GPCLR: LOW 설정
    - GPCLR0: 0번핀부터 31번핀까지 설정
        - 3번 비트를 1로 주면 3번 핀이 LOW 4번 비트를 1로 주면 4번 핀이 LOW
    - GPCLR1: 32번핀부터 57번핀까지 설정
        - 1번 비트를 1로 주면 32번 핀이 LOW 4번 비트를 1로 주면 35번 핀이 LOW
     

# PWM
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/70706b63-20af-4a80-9a56-27bb6806d393)

Dulty Cycle

![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/45ac8167-b3d3-42b5-ad5e-25e00c8906d6)

- PWM이란?
   - Pulse Width Modulation
     - High 상태와 Low 상태 파형의 비율을 듀티 사이클
     - 원래는 통신용으로 개발된 기술이었으나 전류, 전압 제어용 사용
     - LED를 예로 들면 아날로그 신호는 Sin파이기때문에 밝기까지 조절 가능 디지털 신호는 불가능 PWm는 가능 이걸 하는 것이 듀티 사이클

- 라즈베리 파이에서의 PWM는?
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/895f0ed4-ae45-420b-933d-3248f438db64)

GPIO 12, 13, 18, 19를 사용하면 된다

   
