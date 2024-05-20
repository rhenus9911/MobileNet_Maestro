### 라즈베리파이4 커널 세팅

1. 종속성 설치

```bash
$	sudo apt install git bc bison flex libssl-dev make
```

2. 커널 소스코드 다운

```bash
cd /usr/src
sudo git clone --depth=1 https://github.com/raspberrypi/linux
```

3. Config 파일 생성

```bash
cd linux
KERNEL=kernel8 # raspberry pi4 64bit settting
sudo make bcm2711_defconfig
```

4. Build

```bash
sudo make -j4 Image.gz modules dtbs # core가 4개이므로 j4, 64bit이므로 Image.gz
sudo make modules_install
sudo cp arch/arm64/boot/dts/broadcom/*.dtb /boot/firmware
sudo cp arch/arm64/boot/dts/overlays/*.dtb* /boot/firmware/overlays/
sudo cp arch/arm64/boot/dts/overlays/README /boot/firmware/overlays/
sudo cp arch/arm64/boot/Image.gz /boot/firmware/$KERNEL.img
```

5. kernel version

```bash
uname -r
```

6. reboot

```bash
sudo reboot 
uname -r
```
