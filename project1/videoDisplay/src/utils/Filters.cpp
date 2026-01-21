#include "project1/utils/Filters.hpp"
#include <opencv2/opencv.hpp>

int Filters::greyscale(cv::Mat &src, cv::Mat &dst)
{
    // check for empty source image
    if (src.empty())
    {
        return -1; // Error: empty source image
    }
    // copy the src to the dst to allocate the dst
    src.copyTo(dst);

    // iterate each row
    for (int i = 1; i < src.rows - 1; i++)
    {
        // get pointer to the current row
        const cv::Vec3b *srcRow = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *dstRow = dst.ptr<cv::Vec3b>(i);
        // iterate each column
        for (int j = 1; j < src.cols - 1; j++)
        {

            // get BGR values from src
            int blue = srcRow[j][0];
            int green = srcRow[j][1];
            int red = srcRow[j][2];

            // apply weighted formula to compute greyscale value
            int greyValue = static_cast<int>(0.04 * blue + 0.64 * green + 0.32 * red);

            // set the grey value to all three channels in dst
            dstRow[j][0] = greyValue;
            dstRow[j][1] = greyValue;
            dstRow[j][2] = greyValue;
        }
    }

    return 0; // Success
}