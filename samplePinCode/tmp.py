pinToGpio = [
  17, 18, 21, 22, 23, 24, 25, 4,
  0,  1,
  8,  7,
  10,  9, 11,
  14, 15,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  ]

gpioToGPFSEL = [
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,
  ]

gpioToShift = [
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  ]

gpioToGPCLR = [
  10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
  11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
  ]

gpioToGPSET = [
   7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   ]

gpioToGPLEV = [
  13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
  14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
  ]


def print_bin(data):
    for i in range(len(data)):
        if i % 4 == 0:
            print(' ', end="")
        print(data[i], end="")
    print("\n")

gpio_base = 0xfe200000
pin = 16

pin = pinToGpio[pin]

fSel = gpioToGPFSEL[pin]
shift = gpioToShift[pin]
gpio = 0xf7cbd000

gpio_data = 0xffffffff

print(f"fSel: {fSel}")
print(f"shift: {shift}")

print("[+] gpio_data")
print(hex(gpio_data))
print_bin(bin(gpio_data)[2:].ljust(32, '0'))

tmp = gpio_data & ~(7 << shift)

print("[+] pinMode INPUT")
print(hex(tmp))
print_bin(bin(tmp)[2:].ljust(32, '0'))

print("[+] pinMode OUTPUT")
tmp = tmp | (1<<shift)
print(hex(tmp))
print_bin(bin(tmp)[2:].ljust(32, '0'))


clr = 1 << (pin & 31)
print(f"[+] value: LOW *(gpio + {gpioToGPCLR[pin]})")
print_bin(bin(clr)[2:].ljust(32, '0'))

st = 1 << (pin & 31)
print(f"[+] value: HIGH *(gpio + {gpioToGPSET[pin]})")
print_bin(bin(st)[2:].ljust(32, '0'))

rd = 1 << (pin & 31)
print(f"[+] READ: *(gpio + {gpioToGPLEV[pin]})")
print_bin(bin(rd)[2:].ljust(32, '0'))
