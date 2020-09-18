#include <iostream>
#include <chrono>  // get time
#include <iomanip> //set precision
#include <sstream>


namespace utils
{
    class FPS{
    double fps,start_,end_;
    public:
        void start();
        std::string get();
        double get_time();
    };
}