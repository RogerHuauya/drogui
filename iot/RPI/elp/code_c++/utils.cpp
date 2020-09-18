#include "utils.h"

namespace utils
{
    double FPS::get_time() {
        auto current_time = std::chrono::system_clock::now();
        auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());
        return duration_in_seconds.count();
    }
    void FPS::start () {
        start_ = get_time();
    }
    std::string FPS::get() {
        fps = get_time()-start_;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << 1/fps;
        return stream.str();
    }
}