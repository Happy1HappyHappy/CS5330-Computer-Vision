// Authors: Claire Liu, Yu-Jing Wei
// File: TimeUtil.hpp
// Path: project1/videoDisplay/include/project1/utils/TimeUtil.hpp
// Description: Provides utility functions for time-related operations.

#ifndef TIME_UTIL_HPP
#define TIME_UTIL_HPP
#include <string>

// TimeUtil class Encapsulation
class TimeUtil
{
public:
    static std::string getTimestamp();
    static double getTime();
};

#endif