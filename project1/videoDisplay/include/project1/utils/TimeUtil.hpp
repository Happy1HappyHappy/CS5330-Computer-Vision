#ifndef TIME_UTIL_HPP
#define TIME_UTIL_HPP
#include <string>

// TimeUtil class Encapsulation
class TimeUtil
{
public:
    static std::string getTimestamp();
    static double getTime();
    static int getNTime();
};

#endif