MCU=atmega16
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

all: single_servo_timer.hex
single_servo_timer.hex : single_servo_timer.out 
	$(OBJCOPY) -R .eeprom -O ihex single_servo_timer.out single_servo_timer.hex 
single_servo_timer.out : single_servo_timer.o 
	$(CC) $(CFLAGS) -O0 -o single_servo_timer.out -Wl,-Map,single_servo_timer.map single_servo_timer.o 
single_servo_timer.o : single_servo_timer.c 
	$(CC) $(CFLAGS) -O0 -c single_servo_timer.c

# erase the AVR before it is programmed

clean:
	rm -f *.o *.map *.out *.hex

load:
	avrdude -c usbasp -p ATMEGA16  -U flash:w:single_servo_timer.hex


