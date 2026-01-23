// Authors: Claire Liu, Yu-Jing Wei
// File: Filters.cpp
// Path: project1/videoDisplay/src/utils/Filters.cpp
// Description: Filters implementation file defining image filtering functions.

#include "project1/utils/Filters.hpp"
#include "project1/utils/TimeUtil.hpp"
#include <opencv2/opencv.hpp>

int Filters::greyscale(cv::Mat &src, cv::Mat &dst)
{
    // This function applies a greyscale filter to the source image
    // src: source image
    // dst: destination image

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
    // Apply sepia filter to the source image and store in destination image
    // src: source image
    // dst: destination image
    // vignetteFlag: whether to apply vignette effect

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
    // This function computes the vignette factor based on distance from center
    // x, y: pixel coordinates
    // centerX, centerY: center of the vignette
    // radius: radius of the vignette effect

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

int Filters::blur5x5_1(cv::Mat &src, cv::Mat &dst, int times)
{
    // This function applies a 5x5 Gaussian blur using direct convolution (version 1)
    // src: source image
    // dst: destination image
    // times: number of times to apply the blur

    // check for empty source images
    if (src.empty())
        return -1;

    // define the 5x5 Gaussian kernel
    const int kernel[5][5] = {
        {1, 2, 4, 2, 1},
        {2, 4, 8, 4, 2},
        {4, 8, 16, 8, 4},
        {2, 4, 8, 4, 2},
        {1, 2, 4, 2, 1}};

    const int kSize = 5;         // kernel size
    const int kHalf = kSize / 2; // 2
    const int kSum = 100;        // sum of kernel weights
    // set up the timing for version 1
    double startTime = TimeUtil::getTime();
    // intermediate image for processing
    cv::Mat tmp;
    src.copyTo(tmp);
    src.copyTo(dst);

    // apply the blur 'times' times
    for (int t = 0; t < times; t++)
    {
        // iterate each pixel in the source image starts from (2,2) to (rows-2, cols-2)
        for (int i = 0; i < tmp.rows; i++)
        {
            // pointer to the destination row
            cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);

            // iterate each column
            for (int j = 0; j < src.cols; j++)
            {
                // initialize sums for each channel
                int sumB = 0, sumG = 0, sumR = 0;

                // apply the kernel
                for (int ki = -kHalf; ki <= kHalf; ki++)
                {
                    // handle border pixels by
                    int row = i + ki;
                    if (row < 0)
                        row = 0;
                    if (row >= tmp.rows)
                        row = tmp.rows - 1;

                    // get pointer to the current row
                    cv::Vec3b *sptr = tmp.ptr<cv::Vec3b>(row);

                    // iterate each column of the kernel
                    for (int kj = -kHalf; kj <= kHalf; kj++)
                    {
                        // handle border pixels
                        int col = j + kj;
                        if (col < 0)
                            col = 0;
                        if (col >= tmp.cols)
                            col = tmp.cols - 1;
                        // get the kernel weight
                        int weight = kernel[ki + kHalf][kj + kHalf];

                        sumB += sptr[col][0] * weight;
                        sumG += sptr[col][1] * weight;
                        sumR += sptr[col][2] * weight;
                    }
                }
                // set the blurred pixel value in dst (normalize by sum of kernel weights = 100)
                dptr[j][0] = sumB / kSum;
                dptr[j][1] = sumG / kSum;
                dptr[j][2] = sumR / kSum;
            }
        }
        // prepare for next iteration
        if (t < times - 1)
        {
            // copy dst to tmp for next iteration
            dst.copyTo(tmp);
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

int Filters::blur5x5_2(cv::Mat &src, cv::Mat &dst, int times)
{
    // This function applies a 5x5 Gaussian blur using separable kernels (version 2)
    // src: source image
    // dst: destination image
    // times: number of times to apply the blur

    // check for empty source images
    if (src.empty())
        return -1;

    // define the 1x5 Gaussian kernel
    int kernel[5] = {1, 2, 4, 2, 1};
    int kSize = 5; // kernel size

    // set up the timing for version 2
    double startTime = TimeUtil::getTime();

    // Convoleve
    for (int t = 0; t < times; t++)
    {
        Filters::convolve(src, dst, kernel, kernel, kSize, 10);
    }

    // end the timing
    double endTime = TimeUtil::getTime();

    // compute the time per image
    double difference = (endTime - startTime) / times;
    // print the results
    printf("Time per image (2): %.4lf seconds\n", difference);

    return 0;
}

int Filters::sobelX3x3(cv::Mat &src, cv::Mat &dst)
{
    // This function applies the Sobel X filter to the source image
    // and stores the result in the destination image.
    // src: source image
    // dst: destination image

    // check for empty source images
    if (src.empty())
        return -1;
    // define the 1x3 Sobel separable kernels
    int kernelXH[3] = {-1, 0, 1}; // Horizontal kernel
    int kernelXV[3] = {1, 2, 1};  // Vertical kernel

    // Convolve
    int kSize = 3;
    Filters::convolve(src, dst, kernelXH, kernelXV, kSize, 0);

    return 0;
}

int Filters::sobelY3x3(cv::Mat &src, cv::Mat &dst)
{
    // This function applies the Sobel Y filter to the source image
    // and stores the result in the destination image.
    // src: source image
    // dst: destination image

    // check for empty source images
    if (src.empty())
        return -1;
    // define the 1x3 Sobel separable kernels
    int kernelYH[3] = {1, 2, 1};  // Horizontal kernel
    int kernelYV[3] = {-1, 0, 1}; // Vertical kernel

    // Convolve
    int kSize = 3;
    Filters::convolve(src, dst, kernelYH, kernelYV, kSize, 0);

    return 0;
}

int Filters::magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst)
{
    // This function computes the magnitude of two images sx and sy
    // and stores the result in dst.
    // sx: Sobel X image
    // sy: Sobel Y image
    // dst: output magnitude image

    // check for empty source images
    if (sx.empty() || sy.empty())
        return -1;
    // allocate dst if empty
    if (dst.empty())
        dst.create(sx.size(), sx.type());

    // compute the magnitude of the two images
    for (int i = 0; i < sx.rows; i++)
    {
        // Use pointers for faster access
        // Vec3s for possible negative values from Sobel
        cv::Vec3s *sxRow = sx.ptr<cv::Vec3s>(i);
        cv::Vec3s *syRow = sy.ptr<cv::Vec3s>(i);
        cv::Vec3s *dstRow = dst.ptr<cv::Vec3s>(i);

        // iterate each column
        for (int j = 0; j < sx.cols; j++)
        {
            // iterate each colorchannel
            for (int c = 0; c < 3; c++)
            {
                int sx_val = sxRow[j][c];
                int sy_val = syRow[j][c];

                // compute magnitude
                int magnitude = (int)sqrt(sx_val * sx_val + sy_val * sy_val);

                // clamp to [0,255] and assign to dst
                dstRow[j][c] = cv::saturate_cast<uchar>(magnitude);
            }
        }
    }

    return 0;
}

int Filters::blurQuantize(cv::Mat &src, cv::Mat &dst, int levels)
{
    // This function applies a blur and quantization effect to the source image
    // src: source image
    // dst: destination image
    // levels: number of quantization levels

    // validate levels
    if (levels <= 0)
        return -1;
    // check for empty source images
    if (src.empty())
        return -1;
    // allocate dst if empty
    if (dst.empty())
        dst.create(src.size(), src.type());

    // first apply a blur using the blur5x5_2 function
    cv::Mat blurred;
    Filters::blur5x5_2(src, blurred, 1);

    // then apply quantization
    int levelSize = 255 / levels;
    for (int i = 0; i < blurred.rows; i++)
    {
        // Use pointers for faster access
        cv::Vec3b *blurredRow = blurred.ptr<cv::Vec3b>(i);
        cv::Vec3b *dstRow = dst.ptr<cv::Vec3b>(i);

        // iterate each column
        for (int j = 0; j < blurred.cols; j++)
        {
            // iterate each colorchannel
            for (int c = 0; c < 3; c++)
            {
                // get the pixel value
                int x = blurredRow[j][c];
                // quantize the pixel value
                int xt = x / levelSize;
                int xf = xt * levelSize;
                // clamp to [0,255] and assign to dst
                dstRow[j][c] = cv::saturate_cast<uchar>(xf);
            }
        }
    }
    return 0;
}

int Filters::convolve(cv::Mat &src, cv::Mat &dst, int *kernel1, int *kernel2, int kSize, int kSum)
{
    // This is a function only for convolving an image with separable kernel
    // src: source image
    // dst: destination image
    // kernel1: horizontal 1D kernel
    // kernel2: vertical 1D kernel
    // kSize: size of the kernels (assumed to be odd)
    // kSum: sum of the kernel weights for normalization (0 if no normalization)

    // check for empty source images
    if (src.empty() || kernel1 == nullptr || kernel2 == nullptr)
        return -1;
    // allocate dst if empty
    if (dst.empty())
        dst.create(src.size(), src.type());

    // padding size
    const int kHalf = kSize / 2;

    // intermediate image for processing
    cv::Mat tmp;
    src.copyTo(tmp);
    // prev for last input, curr for horizontal output
    cv::Mat *prev = &src;
    cv::Mat *curr = &tmp;

    // horizontal pass: curr -> tmp
    for (int i = 0; i < src.rows; i++)
    {
        // get pointer to the current row in src and tmp
        cv::Vec3b *prevRow = prev->ptr<cv::Vec3b>(i);
        cv::Vec3b *currRow = curr->ptr<cv::Vec3b>(i);
        // iterate each column
        for (int j = 0; j < src.cols; j++)
        {
            // initialize sums for each channel
            int sumB = 0, sumG = 0, sumR = 0;

            // apply the kernel
            for (int k = -kHalf; k <= kHalf; k++)
            {
                // Check for border pixels
                int col = j + k;
                if (col < 0)
                    col = 0;
                if (col >= src.cols)
                    col = src.cols - 1;
                // get the kernel weight
                int weight = kernel1[k + kHalf];
                sumB += prevRow[col][0] * weight;
                sumG += prevRow[col][1] * weight;
                sumR += prevRow[col][2] * weight;
            }

            currRow[j][0] = (kSum == 0) ? sumB : (sumB / kSum);
            currRow[j][1] = (kSum == 0) ? sumG : (sumG / kSum);
            currRow[j][2] = (kSum == 0) ? sumR : (sumR / kSum);
        }
    }

    // vertical pass: tmp -> dst
    for (int i = 0; i < src.rows; i++)
    {
        // use prev to save the vertical output
        cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++)
        {
            // initialize sums for each channel
            int sumB = 0, sumG = 0, sumR = 0;
            // apply the kernel
            for (int k = -kHalf; k <= kHalf; k++)
            {
                // Check for border pixels
                int row = i + k;
                if (row < 0)
                    row = 0;
                if (row >= src.rows)
                    row = src.rows - 1;

                cv::Vec3b *currRow = curr->ptr<cv::Vec3b>(row);
                int weight = kernel2[k + kHalf];
                sumB += currRow[j][0] * weight;
                sumG += currRow[j][1] * weight;
                sumR += currRow[j][2] * weight;
            }

            dptr[j][0] = (kSum == 0) ? sumB : (sumB / kSum);
            dptr[j][1] = (kSum == 0) ? sumG : (sumG / kSum);
            dptr[j][2] = (kSum == 0) ? sumR : (sumR / kSum);
        }
    }

    return 0;
}