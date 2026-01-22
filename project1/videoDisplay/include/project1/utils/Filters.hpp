// Authors: Claire Liu, Yu-Jing Wei
// File: Filters.hpp
// Path: project1/include/utils/Filters.hpp
// Description: Declares image filtering functions (e.g., greyscale, blur, etc.).

#pragma once // Include guard

#include <opencv2/opencv.hpp>

class Filters
{
public:
    static int greyscale(cv::Mat &src, cv::Mat &dst);
    static int sepia(cv::Mat &src, cv::Mat &dst, bool vignetteFlag = false);
    static float vignette(float x, float y, float centerX, float centerY, float radius);
    static int blur5x5_1(cv::Mat &src, cv::Mat &dst, int times = 1);
    static int blur5x5_2(cv::Mat &src, cv::Mat &dst, int times = 1);
    static int sobelX3x3(cv::Mat &src, cv::Mat &dst);
    static int sobelY3x3(cv::Mat &src, cv::Mat &dst);
    static int convolve(cv::Mat &src, cv::Mat &dst, int *kernel1, int *kernel2, int kSize, int kSum);

private:
    static double getTime();
};
