/*
Claire Liu, Yu-Jing Wei
position.hpp

Path: include/position.hpp
Description: Header file for position utilities.
*/

#pragma once
#include <opencv2/opencv.hpp>
#include <string>

enum class Position
{
    WHOLE,
    UP,
    BOTTOM,
    CENTER
};

inline Position stringToPosition(const std::string &s)
{
    if (s == "up")
        return Position::UP;
    if (s == "bottom")
        return Position::BOTTOM;
    if (s == "center")
        return Position::CENTER;
    return Position::WHOLE; // default
}

inline std::string positionToString(Position p)
{
    switch (p)
    {
    case Position::WHOLE:
        return "whole";
    case Position::UP:
        return "up";
    case Position::BOTTOM:
        return "bottom";
    case Position::CENTER:
        return "center";
    }
    return "whole";
}

inline cv::Rect roiFor(Position p, int w, int h)
{
    if (p == Position::WHOLE)
        return {0, 0, w, h};
    if (p == Position::UP)
        return {0, 0, w, h / 2};
    if (p == Position::BOTTOM)
        return {0, h / 2, w, h - h / 2};
    if (p == Position::CENTER)
    {
        int y0 = h / 4;
        int hh = h / 2;
        return {0, y0, w, hh};
    }
    return {0, 0, w, h}; // whole
}
