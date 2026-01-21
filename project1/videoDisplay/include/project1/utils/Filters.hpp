#ifndef FILTERS_HPP
#define FILTERS_HPP
#include <opencv2/opencv.hpp>

class Filters
{
public:
    static int greyscale(cv::Mat &src, cv::Mat &dst);
    static int sepia(cv::Mat &src, cv::Mat &dst, bool vignetteFlag = false);
    static float vignette(float x, float y, float centerX, float centerY, float radius);
    static int blur5x5_1( cv::Mat &src, cv::Mat &dst );
    static int blur5x5_2( cv::Mat &src, cv::Mat &dst );
private:
    static double getTime();
};

#endif // FILTERS_HPP