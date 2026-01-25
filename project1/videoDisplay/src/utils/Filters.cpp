/*
Claire Liu, Yu-Jing Wei
Filters.cpp

Path: project1/videoDisplay/src/utils/Filters.cpp
Description: Filters implementation file defining image filtering functions.
*/

#include "project1/utils/Filters.hpp"
#include "project1/utils/TimeUtil.hpp"
#include "project1/utils/faceDetect.hpp"
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
    // convert to absolute values and 8U type
    cv::convertScaleAbs(dst, dst);

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

    // Convert to grayscale first
    cv::Mat grey;
    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);
    // Convolve
    Filters::convolve(grey, dst, kernelXH, kernelXV, 3, 0);
    // convert to absolute values and 8U type
    cv::convertScaleAbs(dst, dst);
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

    // greyscale first
    cv::Mat grey;
    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);
    // Convolve
    Filters::convolve(grey, dst, kernelYH, kernelYV, 3, 0);
    // convert to absolute values and 8U type
    cv::convertScaleAbs(dst, dst);

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
        cv::Vec3b *sxRow = sx.ptr<cv::Vec3b>(i);
        cv::Vec3b *syRow = sy.ptr<cv::Vec3b>(i);
        cv::Vec3b *dstRow = dst.ptr<cv::Vec3b>(i);

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

int Filters::faceDetect(cv::Mat &src, cv::Mat &dst, cv::Rect &last)
{
    // This function detects faces in the source image and draws rectangles around them in the destination image
    // src: source image
    // dst: destination image

    // check for empty source images
    if (src.empty())
        return -1;

    // copy src to dst
    src.copyTo(dst);

    std::vector<cv::Rect> faces;                 // vector to hold detected faces
    cv::Mat grey;                                // grayscale image
    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY); // convert to greyscale

    // detect faces
    detectFaces(grey, faces);

    // add a little smoothing by averaging the last two detections
    if (faces.size() > 0)
    {
        if (last.area() == 0)
        {
            last = faces[0]; // initialize last if it's the first detection
        }
        else
        {
            // smooth by averaging with last
            last.x = (faces[0].x + last.x) / 2;
            last.y = (faces[0].y + last.y) / 2;
            last.width = (faces[0].width + last.width) / 2;
            last.height = (faces[0].height + last.height) / 2;
        }
    }
    // draw boxes around the faces
    drawBoxes(dst, faces);

    // check if dst is empty
    if (dst.empty())
        return -1;

    return 0;
}

int Filters::blurOutsideFaces(cv::Mat &src, cv::Mat &dst, cv::Rect &last)
{
    // This function blurs the area outside detected faces in the source image
    // src: source image
    // dst: destination image

    // check for empty source images
    if (src.empty())
        return -1;

    // copy src to dst
    src.copyTo(dst);

    std::vector<cv::Rect> faces;                 // vector to hold detected faces
    cv::Mat grey;                                // grayscale image
    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY); // convert to greyscale

    // detect faces
    detectFaces(grey, faces);

    // add a little smoothing by averaging the last two detections
    if (faces.size() > 0)
    {
        last.x = (faces[0].x + last.x) / 2;
        last.y = (faces[0].y + last.y) / 2;
        last.width = (faces[0].width + last.width) / 2;
        last.height = (faces[0].height + last.height) / 2;
    }

    // create a mask for the face region
    cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);
    if (faces.size() > 0)
    {
        cv::rectangle(mask, last, cv::Scalar(255), cv::FILLED);
    }

    // blur the entire image
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(21, 21), 0);

    // copy the blurred areas outside the face region to dst
    blurred.copyTo(dst, 255 - mask);

    return 0;
}

int Filters::remainYellowInGrey(cv::Mat &src, cv::Mat &dst)
{
    // This filter preserves yellow and keep everything else as grey
    // src: source image
    // dst: destination image

    // check for empty source images
    if (src.empty())
        return -1;

    cv::Mat grey;
    Filters::greyscale(src, grey);

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // define yellow in HSV
    cv::Scalar lowerYellow = cv::Scalar(15, 50, 50);
    cv::Scalar upperYellow = cv::Scalar(45, 255, 255);

    // a mask show pure white in each pixel if it is a defined yellow
    cv::Mat mask;
    cv::inRange(hsv, lowerYellow, upperYellow, mask);

    dst.create(src.size(), src.type());

    for (int i = 0; i < src.rows; i++)
    {
        const cv::Vec3b *srcPtr = src.ptr<cv::Vec3b>(i);
        const cv::Vec3b *greyPtr = grey.ptr<cv::Vec3b>(i);
        const uchar *maskPtr = mask.ptr<uchar>(i);
        cv::Vec3b *dstPtr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++)
        {
            // if mask is 255, which means it is defined yellow, then show the original pixel
            if (maskPtr[j] > 0)
            {
                dstPtr[j] = srcPtr[j];
            }
            else
            {
                // if not, show grey
                dstPtr[j] = greyPtr[j];
            }
        }
    }
    return 0;
}

int Filters::makeFaceColorful(cv::Mat &src, cv::Mat &dst, cv::Rect &last)
{
    // This filter makes the detected face region colorful while the rest is grey
    // src: source image
    // dst: destination image

    // create a copy of the original color image
    cv::Mat colorCopy = src.clone();

    // check for empty source images
    if (src.empty())
        return -1;

    // create a grey image as the background image
    Filters::greyscale(src, dst);

    // call the detectFace function
    std::vector<cv::Rect> faces;
    cv::Mat greyForDetect;
    cv::cvtColor(src, greyForDetect, cv::COLOR_BGR2GRAY);
    detectFaces(greyForDetect, faces);

    // smooth the face rectangle position with last
    if (!faces.empty())
    {
        if (last.area() == 0)
        {
            last = faces[0];
        }
        else
        {
            last.x = (faces[0].x + last.x) / 2;
            last.y = (faces[0].y + last.y) / 2;
            last.width = (faces[0].width + last.width) / 2;
            last.height = (faces[0].height + last.height) / 2;
        }
    }

    // add the color back to face region from the original image
    if (last.area() > 0)
    {
        // define bounds of the image
        cv::Rect bounds(0, 0, src.cols, src.rows);
        // ensure the rectangle is within image bounds
        cv::Rect safeRect = last & bounds;

        // only copy if the rectangle is valid
        if (safeRect.area() > 0)
        {
            colorCopy(safeRect).copyTo(dst(safeRect));
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

    // ensure src is of type CV_8UC1(greyscale) or CV_8UC3(3 color channels)
    CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3);

    int channels = src.channels(); // number of color channels
    int rows = src.rows;           // image row dimensions
    int cols = src.cols;           // image col dimensions
    const int kHalf = kSize / 2;   // padding size

    // intermediate buffer (signed!)
    cv::Mat tmp(rows, cols, CV_MAKETYPE(CV_16S, channels));

    // output (signed, keep direction info)
    dst.create(rows, cols, CV_MAKETYPE(CV_16S, channels));

    /*  Horizontal Pass*/
    if (channels == 1) // greyscale
    {
        for (int i = 0; i < rows; i++)
        {
            const uchar *srcRow = src.ptr<uchar>(i);
            short *tmpRow = tmp.ptr<short>(i);

            for (int j = 0; j < cols; j++)
            {
                int sum = 0;
                for (int k = -kHalf; k <= kHalf; k++)
                {
                    int col = std::clamp(j + k, 0, cols - 1);
                    sum += srcRow[col] * kernel1[k + kHalf];
                }
                if (kSum)
                    sum /= kSum;
                tmpRow[j] = sum;
            }
        }
    }
    else // 3 channels
    {
        for (int i = 0; i < rows; i++)
        {
            const cv::Vec3b *srcRow = src.ptr<cv::Vec3b>(i);
            cv::Vec3s *tmpRow = tmp.ptr<cv::Vec3s>(i);

            for (int j = 0; j < cols; j++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int sum = 0;
                    for (int k = -kHalf; k <= kHalf; k++)
                    {
                        int col = std::clamp(j + k, 0, cols - 1);
                        sum += srcRow[col][c] * kernel1[k + kHalf];
                    }
                    if (kSum)
                        sum /= kSum;
                    tmpRow[j][c] = sum;
                }
            }
        }
    }

    /* Vertical Pass */
    if (channels == 1) // greyscale
    {
        for (int i = 0; i < rows; i++)
        {
            short *dstRow = dst.ptr<short>(i);

            for (int j = 0; j < cols; j++)
            {
                int sum = 0;
                for (int k = -kHalf; k <= kHalf; k++)
                {
                    int row = std::clamp(i + k, 0, rows - 1);
                    sum += tmp.ptr<short>(row)[j] * kernel2[k + kHalf];
                }
                if (kSum)
                    sum /= kSum;
                dstRow[j] = sum;
            }
        }
    }
    else // 3 channels
    {
        for (int i = 0; i < rows; i++)
        {
            cv::Vec3s *dstRow = dst.ptr<cv::Vec3s>(i);

            for (int j = 0; j < cols; j++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int sum = 0;
                    for (int k = -kHalf; k <= kHalf; k++)
                    {
                        int row = std::clamp(i + k, 0, rows - 1);
                        sum += tmp.ptr<cv::Vec3s>(row)[j][c] * kernel2[k + kHalf];
                    }
                    if (kSum)
                        sum /= kSum;
                    dstRow[j][c] = sum;
                }
            }
        }
    }
    return 0;
}

// Filter that preserve yellow and keep everything else as grey
int Filters::remainYellowInGrey(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
        return -1;

    cv::Mat grey;
    Filters::greyscale(src, grey);

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // define yellow in HSV
    cv::Scalar lowerYellow = cv::Scalar(15, 50, 50);
    cv::Scalar upperYellow = cv::Scalar(45, 255, 255);

    // a mask show pure white in each pixel if it is a defined yellow
    cv::Mat mask;
    cv::inRange(hsv, lowerYellow, upperYellow, mask);

    dst.create(src.size(), src.type());

    for (int i = 0; i < src.rows; i++)
    {
        const cv::Vec3b *srcPtr = src.ptr<cv::Vec3b>(i);
        const cv::Vec3b *greyPtr = grey.ptr<cv::Vec3b>(i);
        const uchar *maskPtr = mask.ptr<uchar>(i);
        cv::Vec3b *dstPtr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++)
        {
            // if mask is 255, which means it is defined yellow, then show the original pixel
            if (maskPtr[j] > 0)
            {
                dstPtr[j] = srcPtr[j];
            }
            else
            {
                // if not, show grey
                dstPtr[j] = greyPtr[j];
            }
        }
    }
    return 0;
}

int Filters::makeFaceColorful(cv::Mat &src, cv::Mat &dst, cv::Rect &last)
{
    // create a grey image as the background image
    cv::Mat grey;
    Filters::greyscale(src, grey);
    grey.copyTo(dst);

    // call the detectFace function
    std::vector<cv::Rect> faces;
    cv::Mat greyForDetect;
    cv::cvtColor(src, greyForDetect, cv::COLOR_BGR2GRAY);
    detectFaces(greyForDetect, faces);

    if (faces.size() > 0)
    {
        last.x = (faces[0].x + last.x) / 2;
        last.y = (faces[0].y + last.y) / 2;
        last.width = (faces[0].width + last.width) / 2;
        last.height = (faces[0].height + last.height) / 2;
    }

    // add the color back to face region from the original image
    if (last.area() > 0)
    {
        cv::Rect bounds(0, 0, src.cols, src.rows);
        cv::Rect safeRect = last & bounds;

        printf("last: x=%d y=%d w=%d h=%d area=%d | safeRect area=%d\n",
               last.x, last.y, last.width, last.height,
               last.area(), safeRect.area());
        if (safeRect.area() > 0)
        {
            src(safeRect).copyTo(dst(safeRect));
        }
    }
    return 0;
}