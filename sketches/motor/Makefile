DEV := ARDUINO_AVR_PRO
NAME=motor
OBJ=motor.o ../../lib/Motor/Motor.o

INCLUDE=../../include
PCONF=../../avrdude.conf
SERIAL=/dev/ttyAMA0

ifeq ($(DEV),ARDUINO_AVR_PRO)
	CORE=../../core-pro3v.a
	MCU=atmega328p
	DF=8000000L
	VAR=$(INCLUDE)/eightanaloginputs
	BR=57600
endif

ifeq ($(DEV),ARDUINO_AVR_UNO)
	CORE=../../core-uno.a
	MCU=atmega328p
	DF=16000000L
	VAR=$(INCLUDE)/standard
	BR=115200
endif

CPLAG=-g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=$(MCU) -DF_CPU=$(DF) -DARDUINO=10605 -D$(DEV) -DARDUINO_ARCH_AVR

%.o: %.cpp
	avr-g++ -c $(CPLAG) -I$(INCLUDE) -I$(VAR) $<  -o $@

all:$(OBJ)
	avr-gcc -w -Os -Wl,--gc-sections -mmcu=$(MCU) -o $(NAME).elf $(OBJ) $(CORE) -L./ -lm 
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $(NAME).elf $(NAME).eep 
	avr-objcopy -O ihex -R .eeprom $(NAME).elf $(NAME).hex

program:
	avrdude -C$(PCONF) -v -p$(MCU) -carduino -P$(SERIAL) -b$(BR) -D -Uflash:w:$(NAME).hex:i
clean:
	rm *.hex *.o *.elf *.eep *.d





