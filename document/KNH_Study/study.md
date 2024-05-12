주소가 매핑되는 과정은?
![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/fb660d10-0e15-447f-96fc-06b6ccc37d13)


![image](https://github.com/rhenus9911/MobileNet_Maestro/assets/100738276/07b2d6c1-67e9-4049-ade4-f359a42b2e4c)

35비트 주소쳬계를 쓰지만 c언어로 접근하기 위해서는 32비트를 써야한다.


# 어떻게 접근?

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
