# 시스템 버스
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/cc5964e2-ec73-433c-b11d-b5c5a0c917d7)

- 주소 버스(Address Bus)
   - 메모리의 주소나 I/O Unit의 포트 번호를 전달
   - CPU와 Memory는 단방향으로 데이터 전달이 가능한 버스를 이용함으로써 주소 전달은 CPU에서 메모리로만 가능
   - CPU, Memory는 I/O Unit과 양방향으로 데이터 전달이 가능한 버스를 사용
     
- 데이터 버스(Data Bus)
   - 데이터 전달
   - 각 구성요소(CPU, Memory, I/O Unit)는 양방향으로 데이터 전달이 가능한 버스를 사용
   
- 제어 버스(Control Bus)
   - 제어 신호 전달
   - Read와 Write신호가 전달
   - 각 구성요소(CPU, Memory, I/O Unit)는 양방향으로 데이터 전달이 가능한 버스를 사용


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
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/f0a5955d-3159-43fe-92ec-c041130fffaa)

GPIO 12, 13, 18, 19를 사용하면 된다.
GPIO 12, 18은 PWM0이기 떄문에 base address가 0x7e20c000 13, 19은 PWM1이기 때문에 base address 0x7e20c800이다.

![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/f9e52183-21c2-4e20-8841-a6e652bf0db3)

![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/39ec7f4f-4459-458f-b09e-51b815a47520)

- CTL는 PWM Control이다. 오프셋도 32bit로 구성되어 있으며 15번째 비트를 통해 2가지 모드를 쓸 수 있다.
    - 0: PWM 알고리즘이 사용됩니다. 이 모드에서는 PWM 신호의 듀티 사이클이 비율로 조정되어, ON과 OFF 상태가 주기적으로 변환된다.
    - 1: M/S 전송이 사용됩니다. 이 모드에서는 듀티 사이클이 마크(활성) 시간과 스페이스(비활성) 시간으로 직접 지정된다. 

- STA
   - PWM의 현재 상태를 보여주는 레지스터입니다. 오류 상태나 FIFO의 상태를 확인할 수 있다.
      - FIFO가 왜 나오지?
         - PWM 모듈에서 FIFO는 일련의 PWM 값들을 버퍼링하는 데 사용됩니다. 이를 통해 연속적인 PWM 신호를 생성할 수 있다.  

 - DMAC
    - WM에 Direct Memory Access(DMA)를 설정하는 레지스터입니다. DMA를 통해 CPU의 개입 없이도 메모리에서 직접 데이터를 PWM FIFO로 전송할 수 있도록 설정
       - DMA이 뭐지?
          - Direct Memory Access로 말 그대로 다이렉트로 데이터를 메모리에 보낸다. - > CPU의 직접적인 관여 없이 메모리와 하드웨어 장치 간에 데이터를 직접 전송할 수 있게 해줌  

- RNG1
   - PWM 채널 1의 주기를 설정하는 레지스터입니다

- DAT1
   - PWM 채널 1의 듀티 사이클을 설정하는 레지스터 
 
   
