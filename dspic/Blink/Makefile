#the procesor
PROC = 33EP512MU810

#the c compiler
CC = xc16-gcc

#the hexfile maker
HX= xc16-bin2hex

#object dumper
OBJDMP= xc16-objdump

#the output target $(TARGET).hex
TARGET=out

#additional linker flags
LINKFLAGS=-Map=$(TARGET).map

#if we have specified a linker script add it
ifdef LINKSCRIPT
	LINKFLAGS:=--script=$(LINKSCRIPT),$(LINKFLAGS)
endif

SOURCES := $(wildcard *.c) $(wildcard src/*.c )
OBJS := $(patsubst %.c, %.o,$(SOURCES))
HDRS := $(wildcard headers/*.h)
CFLAGS=-g -O1 -x c -I headers 

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
	$(RM) *.map *.o *.elf *.dep        

# Link all the object files into an elf file.
$(TARGET).elf : $(OBJS)
	@echo Linking elf file
	$(CC) -mprocessor=$(PROC) -o $(TARGET).elf $(OBJS) -T p$(PROC).gld -Wl,$(LINKFLAGS)

# Create an object file for each C file.
%.o : %.c $(HDRS)
	@echo Creating object file $@
	$(CC) $(CFLAGS) -c -mcpu=$(PROC) -o $@ $< -T p$(PROC).gld

#path to harmony framework, harmony peripherals, harmony dsp library, harmony libq
.PHONY: clean

# Erase all hex, map, object, and elf files.
clean :
	$(RM) *.hex *.map *.o *.elf *.dep *.dis
	     