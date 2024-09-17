# Makefile


CC = gcc
CFLAGS = -Wall -g

TARGET = main
OBJ = main.o test.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) -lbluetooth -lwiringPi

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
