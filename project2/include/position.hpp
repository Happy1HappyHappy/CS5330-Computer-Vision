/*
Claire Liu, Yu-Jing Wei
position.hpp

Path: include/position.hpp
Description: Header file for position utilities.
*/

#pragma once
#include <opencv2/opencv.hpp>
#include <string>

/*
Enumeration for different positions within an image.
- WHOLE: The entire image.
- UP: The upper half of the image.
- BOTTOM: The lower half of the image.
- CENTER: The central region of the image.
*/
enum class Position
{
  WHOLE,
  UP,
  BOTTOM,
  CENTER
};

/*
Converts a string representation of a position to the corresponding Position
enum value.
- "up": Upper half of the image.
- "bottom": Lower half of the image.
- "center": Central region of the image.
- Any other value: Whole image.
*/
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

/*
Converts a Position enum value to its string representation.
- Position::WHOLE: "whole"
- Position::UP: "up"
- Position::BOTTOM: "bottom"
- Position::CENTER: "center"
*/
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

/*
Returns the region of interest (ROI) for a given position within an image.
- Position::WHOLE: The entire image.
- Position::UP: The upper half of the image.
- Position::BOTTOM: The lower half of the image.
- Position::CENTER: The central region of the image.
*/
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
