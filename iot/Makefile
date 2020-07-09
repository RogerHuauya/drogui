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


<<<<<<< HEAD
# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/roger/Github/drogui/iot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roger/Github/drogui/iot

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/roger/Github/drogui/iot/CMakeFiles /home/roger/Github/drogui/iot/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/roger/Github/drogui/iot/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named iot

# Build rule for target.
iot: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 iot
.PHONY : iot

# fast build rule for target.
iot/fast:
	$(MAKE) -f CMakeFiles/iot.dir/build.make CMakeFiles/iot.dir/build
.PHONY : iot/fast

receiveCamera.o: receiveCamera.cpp.o

.PHONY : receiveCamera.o

# target to build an object file
receiveCamera.cpp.o:
	$(MAKE) -f CMakeFiles/iot.dir/build.make CMakeFiles/iot.dir/receiveCamera.cpp.o
.PHONY : receiveCamera.cpp.o

receiveCamera.i: receiveCamera.cpp.i

.PHONY : receiveCamera.i

# target to preprocess a source file
receiveCamera.cpp.i:
	$(MAKE) -f CMakeFiles/iot.dir/build.make CMakeFiles/iot.dir/receiveCamera.cpp.i
.PHONY : receiveCamera.cpp.i

receiveCamera.s: receiveCamera.cpp.s

.PHONY : receiveCamera.s

# target to generate assembly for a file
receiveCamera.cpp.s:
	$(MAKE) -f CMakeFiles/iot.dir/build.make CMakeFiles/iot.dir/receiveCamera.cpp.s
.PHONY : receiveCamera.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... iot"
	@echo "... edit_cache"
	@echo "... receiveCamera.o"
	@echo "... receiveCamera.i"
	@echo "... receiveCamera.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
=======
.PHONY: clean
clean :
	@echo Cleaning all
	$(RM) *.o src/*.o *.out
>>>>>>> origin/inglis

.PHONY: cleanObjects
cleanObjects :
	@echo Cleaning all objects
	$(RM) *.o src/*.o