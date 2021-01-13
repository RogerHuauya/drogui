
CXX = g++
INC = -I/usr/local/include/opencv4 
PCFG= `pkg-config --cflags --libs opencv4` 

all: test.o 

%.o: %.cpp 
	$(CXX) -o $@ $< $(INC) $(PCFG)


clean:
	rm -f *.o
