#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h> // for sleep

#define BMP280_ADDRESS 0x76 // BMP280 안되면 0x77

// BMP280 레지스터 정의 20비트로 온도값 받아옴
#define BMP280_REG_TEMP_XLSB 0xFC // 하위 4비트
#define BMP280_REG_TEMP_LSB  0xFB // 하위 8비트
#define BMP280_REG_TEMP_MSB  0xFA // 상위 8비트
#define BMP280_REG_CONTROL   0xF4
#define BMP280_REG_CONFIG    0xF5
#define BMP280_REG_ID        0xD0

// BMP280 제어 레지스터 값
#define BMP280_COMMAND_TEMPERATURE 0x2E

// 보정값을 저장할 변수
unsigned short dig_T1;
short dig_T2, dig_T3;

// 보정값 읽기 함수
void read_calibration_data(int fd) {
    dig_T1 = wiringPiI2CReadReg16(fd, 0x88);
    dig_T2 = wiringPiI2CReadReg16(fd, 0x8A);
    dig_T3 = wiringPiI2CReadReg16(fd, 0x8C);
}

// 온도를 읽고 보정하는 함수
int32_t compensate_temperature(int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    T = var1 + var2; 
    return (T * 5 + 128) >> 8;  // 온도 보정
}

// BMP280에서 온도 값을 읽는 함수
float read_temperature(int fd) {
    // 온도 데이터를 3바이트 읽기 (MSB, LSB, XLSB)
    int32_t msb = wiringPiI2CReadReg8(fd, BMP280_REG_TEMP_MSB);
    int32_t lsb = wiringPiI2CReadReg8(fd, BMP280_REG_TEMP_LSB);
    int32_t xlsb = wiringPiI2CReadReg8(fd, BMP280_REG_TEMP_XLSB);

    // 20비트 온도 데이터 합성
    int32_t rawTemp = (msb << 12) | (lsb << 4) | (xlsb >> 4);

    // 온도를 보정
    int32_t temp = compensate_temperature(rawTemp);

    // 섭씨로 변환 (소수점을 포함한 값을 위해 100으로 나눔)
    return temp / 100.0;
}

int main(void) {
    int fd;

    // BMP280 I2C 장치에 연결
    if ((fd = wiringPiI2CSetup(BMP280_ADDRESS)) < 0) {
        perror("I2C 장치 연결 실패");
        return 1;
    }

    // 장치 ID 확인 (BMP280 ID: 0x58)
    int id = wiringPiI2CReadReg8(fd, BMP280_REG_ID);
    if (id != 0x58) {
        printf("BMP280 ID 확인 실패! ID: 0x%x\n", id);
        return 1;
    }

    // 보정값 읽기
    read_calibration_data(fd);

    // 측정을 위해 BMP280 설정
    wiringPiI2CWriteReg8(fd, BMP280_REG_CONTROL, 0x27);

    // 온도 읽기
    while (1) {
        float temperature = read_temperature(fd);
        printf("온도: %.2f°C\n", temperature);
        sleep(1);  // 1초 대기 후 반복
    }

    return 0;
}
