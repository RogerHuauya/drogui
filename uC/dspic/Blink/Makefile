#the procesor
PROC = 33EP512MU810

#the c compiler
CC = xc16-gcc -std=c99

#the hexfile maker
HX= xc16-bin2hex

#object dumper
OBJDMP= xc16-objdump

#the output target $(TARGET).hex
TARGET=out

#additional linker flags
LINKFLAGS=-Map=$(TARGET).map,--heap=10480

#if we have specified a linker script add it
ifdef LINKSCRIPT
	LINKFLAGS:=--script=$(LINKSCRIPT),$(LINKFLAGS)
endif

SOURCES := $(wildcard *.c) $(wildcard src/*.c ) $(wildcard test/*.c) 
OBJS := $(patsubst %.c, %.o,$(SOURCES))
HDRS := $(wildcard headers/*.h)
CFLAGS=-g -O1 -x c -I headers  -mno-eds-warn

#if on windows use a different RM
ifdef ComSpec
	RM = del /S /Q 
endif

#what to do when make all
.PHONY : all 
all : $(TARGET).hex

# Turn the elf file into a hex file.
$(TARGET).hex : $(TARGET).elf
	@echo Creating hex file
	$(HX) $(TARGET).elf
	$(RM) *.map *.elf *.dep *.o    

# Link all the object files into an elf file.
$(TARGET).elf : $(OBJS)
	@echo Linking elf file
	$(CC) -mprocessor=$(PROC) -o $(TARGET).elf $(OBJS) -T p$(PROC).gld -ldsp -Wl,$(LINKFLAGS)

# Create an object file for each C file.
%.o : %.c $(HDRS)
	@echo Creating object file $@ from $<
	$(CC) $(CFLAGS) -c -mcpu=$(PROC) -o $@ $< -T p$(PROC).gld

#path to harmony framework, harmony peripherals, harmony dsp library, harmony libq
.PHONY: clean

# Erase all hex, map, object, and elf files.
clean :
	del *.hex *.map *.elf *.dep *.dis *.o
	     