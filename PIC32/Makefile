#the procesor
PROC = 32MZ2048ECH144

#the c compiler
CC = xc32-gcc -std=gnu99

#the hexfile maker
HX= xc32-bin2hex

#object dumper
OBJDMP= xc32-objdump

#the output target $(TARGET).hex
TARGET=out

#additional linker flags
LINKFLAGS=-Map=$(TARGET).map,--defsym=_min_heap_size=0xF000


SOURCES := $(wildcard *.c) $(wildcard src/*.c ) $(wildcard test/*.c )
OBJS := $(patsubst %.c, %.o,$(SOURCES))
HDRS := $(wildcard headers/*.h)
CFLAGS=-g -O1 -x c -I headers 

#if on windows use a different RM
ifdef ComSpec
	RM = del /S /Q 
	REMOVE = *.map *.o *.elf *.dep *.dis
endif

ifndef ComSpec
	REMOVE = *.map *.o *.elf *.dep *.dis src/*.o test/*.o
endif



#what to do when make all
.PHONY : all 
all : $(TARGET).hex

# Turn the elf file into a hex file.
$(TARGET).hex : $(TARGET).elf
	@echo Creating hex file
	$(HX) $(TARGET).elf
	$(RM) $(REMOVE)

# Link all the object files into an elf file.
$(TARGET).elf : $(OBJS)
	@echo Linking elf file
	$(CC) -mprocessor=$(PROC) -o $(TARGET).elf $(OBJS)  -Wl,$(LINKFLAGS)

# Create an object file for each C file.
%.o : %.c $(HDRS)
	@echo Creating object file $@
	$(CC) $(CFLAGS) -c -mcpu=$(PROC) -o $@ $< -T p$(PROC).gld

#path to harmony framework, harmony peripherals, harmony dsp library, harmony libq
.PHONY: clean

# Erase all hex, map, object, and elf files.
clean :
	$(RM) $(REMOVE) *.hex 
	     