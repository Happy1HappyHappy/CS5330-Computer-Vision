#include "project1/utils/Filters.hpp"
#include "project1/utils/TimeUtil.hpp"
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

int Filters::sepia(cv::Mat &src, cv::Mat &dst, bool vignetteFlag)
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

            // apply sepia transformation matrix
            int newBlue = static_cast<int>(0.272 * red + 0.534 * green + 0.131 * blue);
            int newGreen = static_cast<int>(0.349 * red + 0.686 * green + 0.168 * blue);
            int newRed = static_cast<int>(0.393 * red + 0.769 * green + 0.189 * blue);

            // clamp values to [0,255]
            newBlue = std::min(255, std::max(0, newBlue));
            newGreen = std::min(255, std::max(0, newGreen));
            newRed = std::min(255, std::max(0, newRed));

            // apply vignette effect if flag is set
            float centerX = src.cols / 2.0f;
            float centerY = src.rows / 2.0f;
            float radius = std::max(src.cols, src.rows) / 2.0f;
            if (vignetteFlag)
            {
                // Call the vignette function to get the factor
                float vignetteFactor = Filters::vignette(static_cast<float>(j), static_cast<float>(i), centerX, centerY, radius);
                // Update the new color values
                newBlue = static_cast<int>(newBlue * vignetteFactor);
                newGreen = static_cast<int>(newGreen * vignetteFactor);
                newRed = static_cast<int>(newRed * vignetteFactor);
            }

            // set the new BGR values to dst
            dstRow[j][0] = newBlue;
            dstRow[j][1] = newGreen;
            dstRow[j][2] = newRed;
        }
    }
    return 0; // Success
}

float Filters::vignette(float x, float y, float centerX, float centerY, float radius)
{
    float dx = x - centerX;
    float dy = y - centerY;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance >= radius)
    {
        return 0.0f; // Outside the vignette radius
    }
    else
    {
        return 1.0f - (distance / radius); // Linear falloff
    }
}

int Filters::blur5x5_1( cv::Mat &src, cv::Mat &dst, int times)
{
    cv::Mat kernel = (cv::Mat_<int>(5, 5) <<
    1, 2, 4, 2, 1,
    2, 4, 8, 4, 2,
    4, 8, 16, 8, 4,
    2, 4, 8, 4, 2,
    1, 2, 4, 2, 1
    );

    // set up the timing for version 1
    double startTime = TimeUtil::getTime();

     for (int t = 0; t < times; t++) {
        for (int i = 2; i < src.rows - 2; i++)
    {
        for (int j = 2; j < src.cols - 2; j++)
        {
            int sumB = 0, sumG = 0, sumR = 0;

            for (int ki = -2; ki <= 2; ki++)
            {
                for (int kj = -2; kj <= 2; kj++)
                {
                    cv::Vec3b pixel = src.at<cv::Vec3b>(i + ki, j + kj);
                    int weight = kernel.at<int>(ki + 2, kj + 2);

                    sumB += pixel[0] * weight;
                    sumG += pixel[1] * weight;
                    sumR += pixel[2] * weight;
                }
            }

            dst.at<cv::Vec3b>(i, j)[0] = sumB / 100;
            dst.at<cv::Vec3b>(i, j)[1] = sumG / 100;
            dst.at<cv::Vec3b>(i, j)[2] = sumR / 100;
        }
    }

    }
    
    // end the timing
    double endTime = TimeUtil::getTime();

    // compute the time per image
    double difference = (endTime - startTime) / times;
    // print the results
    printf("Time per image (1): %.4lf seconds\n", difference);
    
    return 0;
}

int Filters::blur5x5_2( cv::Mat &src, cv::Mat &dst, int times)
{   
    // set up the timing for version 2
    double startTime = TimeUtil::getTime();

    // for (i = 0; i < times; i++) {
    //     // execute the file on the original image
    // }

    // end the timing
    double endTime = TimeUtil::getTime();

    // compute the time per image
    double difference = (endTime - startTime) / times;
      // print the results
    printf("Time per image (2): %.4lf seconds\n", difference );
    return 0;
}
