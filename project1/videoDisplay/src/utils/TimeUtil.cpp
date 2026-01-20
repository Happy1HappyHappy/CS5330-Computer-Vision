#include "project1/utils/TimeUtil.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Implements the TimeUtil class methods
// Returns a timestamp string in the format "YYYYMMDD_HHMMSSmmm"
std::string TimeUtil::getTimestamp() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);

    auto ms = duration_cast<milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y%m%d_%H%M%S")
       << std::setw(3) << std::setfill('0') << ms.count();

    return ss.str();
}