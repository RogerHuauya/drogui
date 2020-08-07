CXX = g++

SOURCES :=  $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
HDRS := $(wildcard headers/*.h)
CFLAGS= -I headers -c

SRC_SERVER:= server.cpp
OBJ_SERVER:= server.o
TAR_SERVER = server

SRC_CLIENT:= client.cpp
OBJ_CLIENT:= client.o
TAR_CLIENT = client

all: $(TAR_SERVER).out $(TAR_CLIENT).out cleanObjects

.PHONY: server

server: $(TAR_SERVER).out cleanObjects

$(TAR_SERVER).out : $(OBJ_SERVER) $(OBJS)
	@echo Linking objects
	$(CXX) -o $(TAR_SERVER).out $(OBJ_SERVER) $(OBJS) `pkg-config --cflags --libs opencv`

$(OBJ_SERVER): $(SRC_SERVER) $(HDRS)
	@echo Creating object file for server $@
	$(CXX) $(CFLAGS) -o $@ $<


.PHONY: client

client: $(TAR_CLIENT).out cleanObjects

$(TAR_CLIENT).out : $(OBJ_CLIENT) $(OBJS)
	@echo Linking objects
	$(CXX) -o $(TAR_CLIENT).out $(OBJ_CLIENT) $(OBJS) `pkg-config --cflags --libs opencv`

$(OBJ_CLIENT): $(SRC_CLIENT) $(HDRS)
	@echo Creating object file for client $@
	$(CXX) $(CFLAGS) -o $@ $< 


src/%.o : src/%.cpp $(HDRS)
	@echo Creating object file $@
	$(CXX) $(CFLAGS) -o $@ $< 


.PHONY: clean
clean :
	@echo Cleaning all
	$(RM) *.o src/*.o *.out

.PHONY: cleanObjects
cleanObjects :
	@echo Cleaning all objects
	$(RM) *.o src/*.o