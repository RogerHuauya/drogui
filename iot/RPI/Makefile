CXX = g++
INC = 
PCFG = `pkg-config --cflags --libs ` -ljsoncpp -lz -lbcm2835 -lrt -lpthread

SOURCES :=  $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
HDRS := $(wildcard headers/*.h)
CFLAGS= -I headers -c 

SRC_SERVER:= rpi.cpp
OBJ_SERVER:= rpi.o
TAR_SERVER = rpi

.PHONY: server

server: $(TAR_SERVER) cleanObjects

$(TAR_SERVER) : $(OBJ_SERVER) $(OBJS)
	@echo Linking objects
	$(CXX) -o $(TAR_SERVER) $(OBJ_SERVER) $(OBJS) $(PCFG) $(INC)

$(OBJ_SERVER): $(SRC_SERVER) $(HDRS)
	@echo Creating object file for server $@
	$(CXX) $(CFLAGS) -o $@ $< $(INC)


src/%.o : src/%.cpp $(HDRS)
	@echo Creating object file $@
	$(CXX) $(CFLAGS) -o $@ $< $(INC)


.PHONY: clean
clean :
	@echo Cleaning all
	$(RM) *.o src/*.o *.out

.PHONY: cleanObjects
cleanObjects :
	@echo Cleaning all objects
	$(RM) *.o src/*.o
