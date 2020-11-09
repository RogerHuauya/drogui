#include <iostream>
#include <iomanip>
#include <ctime>


int main()
{
    for(int i=0;i<100000;i++){
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);
      std::cout << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << std::endl;

    }
}