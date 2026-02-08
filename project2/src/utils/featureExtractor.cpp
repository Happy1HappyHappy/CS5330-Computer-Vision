/*
  Claire Liu, Yu-Jing Wei
  featureExtractor.cpp

  Path: project2/src/utils/featureExtractor.cpp
  Description: Extracts features from images.
*/

#include "featureExtractor.hpp"
#include "filters.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>

/*
BaselineExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
This method extracts a 7x7 patch from the center of the image, flattens it into a
feature vector, and normalizes the pixel values to the range [0, 1]. It uses OpenCV to
load the image and perform the necessary operations. The extracted feature vector is
stored in the provided pointer.
- @param imagePath The file path of the image to extract features from.
- @param featureVector A pointer to a vector where the extracted features will be stored.
- @return 0 on success, -1 on failure (e.g., if the image cannot be loaded or is too small).
*/
int BaselineExtractor::extractMat(
    const cv::Mat &image,
    std::vector<float> *featureVector) const
{
    // Ensure the image is large enough
    if (image.rows < 7 || image.cols < 7)
    {
        printf("Error: Image too small for 7x7 feature extraction\n");
        return -1;
    }

    // Extract the 7x7 middle patch
    int startRow = (image.rows - 7) / 2;
    int startCol = (image.cols - 7) / 2;

    // Create a continuous copy of the patch to ensure memory alignment
    cv::Mat patch = image(cv::Rect(startCol, startRow, 7, 7)).clone();

    // Reshape the patch to a single row with 1 channel (7*7*3 = 147 elements)
    // and convert the data type from 8-bit unsigned to 32-bit float and do normalization
    cv::Mat flat;
    patch.reshape(1, 1).convertTo(flat, CV_32F, 1.0 / 255.0);

    // Assign the reshaped data directly to the vector
    featureVector->assign((float *)flat.datastart, (float *)flat.dataend);

    return 0.0f; // Success
}

/*
RGColorHistExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
This method extracts a color histogram feature from the image. It computes a 2D histogram
for the red and green channels, normalizes it, and stores the histogram values in the provided
feature vector. It uses OpenCV to load the image and perform the necessary operations.
- @param imagePath The file path of the image to extract features from.
- @param featureVector A pointer to a vector where the extracted features will be stored.
- @return 0 on success, -1 on failure (e.g., if the image cannot be loaded).
*/
int RGColorHistExtractor::extractMat(
    const cv::Mat &image,
    std::vector<float> *featureVector) const
{
    // Create a 2D histogram for the red and green channels
    int histSize = 16;
    cv::Mat hist = cv::Mat::zeros(cv::Size(histSize, histSize), CV_32FC1);
    for (int i = 0; i < image.rows; i++)
    {
        const cv::Vec3b *ptr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++)
        {
            float B = ptr[j][0];
            float G = ptr[j][1];
            float R = ptr[j][2];

            float divisor = B + G + R;
            divisor = divisor > 0.0 ? divisor : 1.0;
            float r = R / divisor;
            float g = G / divisor;

            int rindex = (int)(r * (histSize - 1) + 0.5);
            int gindex = (int)(g * (histSize - 1) + 0.5);

            hist.at<float>(rindex, gindex)++;
        }
    }
    // Normalization
    hist /= (image.rows * image.cols);

    // Assign histogram values to feature vector
    featureVector->assign((float *)hist.datastart, (float *)hist.dataend);

    return 0; // Success
}

int RGBColorHistExtractor::extractMat(
    const cv::Mat &image,
    std::vector<float> *featureVector) const
{
    // Create a 3D histogram for the red and green channels
    int histSize = 8;
    int dims = 3;
    int sizes[] = {histSize, histSize, histSize};
    cv::Mat hist = cv::Mat::zeros(dims, sizes, CV_32FC1);

    float scale = (float)histSize / 256.0f;
    for (int i = 0; i < image.rows; i++)
    {
        const cv::Vec3b *ptr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++)
        {
            float B = ptr[j][0];
            float G = ptr[j][1];
            float R = ptr[j][2];

            int rindex = (int)(R * scale);
            int gindex = (int)(G * scale);
            int bindex = (int)(B * scale);

            hist.at<float>(rindex, gindex, bindex)++;
        }
    }
    // Normalization
    hist /= (image.rows * image.cols);

    // Assign histogram values to feature vector
    if (hist.isContinuous())
    {
        float *start = (float *)hist.ptr<float>(0);
        int totalElements = histSize * histSize * histSize;
        featureVector->assign(start, start + totalElements);
    }
    else
    {
        featureVector->clear();
        featureVector->reserve(histSize * histSize * histSize);

        for (int r = 0; r < histSize; ++r)
        {
            for (int g = 0; g < histSize; ++g)
            {
                for (int b = 0; b < histSize; ++b)
                {
                    featureVector->push_back(hist.at<float>(r, g, b));
                }
            }
        }
    }

    return 0; // Success
}

int SobelMagnitudeExtractor::extractMat(
    const cv::Mat &image,
    std::vector<float> *featureVector) const
{
    cv::Mat src = image.clone();
    // Apply Sobel X and Y filters to get sX and sY, then apply magnitude filter
    cv::Mat sX, sY, dst;
    if (Filters::sobelX3x3(src, sX) != 0 || Filters::sobelY3x3(src, sY) != 0)
    {
        printf("Error: failed to extract target features by sobel X/Y filter\n");
        return -1;
    }
    if (Filters::magnitude(sX, sY, dst) != 0)
    {
        printf("Error: failed to extract target features by sobel magnitude filter\n");
        return -1;
    };
    // Make sure dst has only single channel
    if (dst.type() != CV_8UC1)
    {
        dst.convertTo(dst, CV_8UC1);
    }
    // Ceate 1D histogram
    int histSize = 256;
    std::vector<float> hist(histSize, 0.0f);

    // Iterates magnitude values at each pixel in dst
    float scale = (float)histSize / 256.0f;
    for (int i = 0; i < dst.rows; i++)
    {
        uchar *ptr = dst.ptr<uchar>(i);
        for (int j = 0; j < dst.cols; j++)
        {
            float value = ptr[j];
            int index = std::min((int)(value * scale), histSize - 1);

            hist[index]++;
        }
    }

    // Push normalized vectors back to histogram
    float totalPixels = (float)(image.rows * image.cols);
    featureVector->clear();
    featureVector->reserve(histSize);
    for (int i = 0; i < histSize; i++)
    {
        featureVector->push_back(hist[i] / totalPixels);
    }

    return 0; // Success
}

int CIELabHistExtractor::extractMat(
    const cv::Mat &image,
    std::vector<float> *featureVector) const
{
    // Create a 3D histogram for the red and green channels
    int L_histSize = 4;
    int a_histSize = 8;
    int b_histSize = 8;
    int dims = 3;
    int sizes[] = {L_histSize, a_histSize, b_histSize};
    cv::Mat hist = cv::Mat::zeros(dims, sizes, CV_32FC1);

    for (int i = 0; i < image.rows; i++)
    {
        const cv::Vec3b *ptr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++)
        {
            float b_norm = ptr[j][0] / 255.0f;
            float g_norm = ptr[j][1] / 255.0f;
            float r_norm = ptr[j][2] / 255.0f;

            // Inverse Gamma Correction
            float r_lin = (r_norm > 0.04045f) ? pow((r_norm + 0.055f) / 1.055f, 2.4f) : r_norm / 12.92f;
            float g_lin = (g_norm > 0.04045f) ? pow((g_norm + 0.055f) / 1.055f, 2.4f) : g_norm / 12.92f;
            float b_lin = (b_norm > 0.04045f) ? pow((b_norm + 0.055f) / 1.055f, 2.4f) : b_norm / 12.92f;

            // Convert to XYZ color space
            // X: response corresponding roughly to red-green perception
            // Y: luminance (perceived brightness)
            // Z: response corresponding roughly to blue perception
            float X = r_lin * 0.4124f + g_lin * 0.3576f + b_lin * 0.1805f;
            float Y = r_lin * 0.2126f + g_lin * 0.7152f + b_lin * 0.0722f;
            float Z = r_lin * 0.0193f + g_lin * 0.1192f + b_lin * 0.9505f;

            // Normalize XYZ by the reference white (D65)
            // Using D65 reference
            float x = X / 0.95047f;
            float y = Y / 1.00000f;
            float z = Z / 1.08883f;

            // Lab non-linear transform
            float fx = (x > 0.008856f) ? cbrt(x) : (7.787f * x + 16.0f / 116.0f);
            float fy = (y > 0.008856f) ? cbrt(y) : (7.787f * y + 16.0f / 116.0f);
            float fz = (z > 0.008856f) ? cbrt(z) : (7.787f * z + 16.0f / 116.0f);

            // Convert to Lab
            float L = 116.0f * fy - 16.0f;
            float a = 500.0f * (fx - fy);
            float b = 200.0f * (fy - fz);

            // L (0~100) -> project 0~15
            int Lindex = (int)(L / 100.0f * L_histSize);
            // a, b (-128~127) -> (+128) -> normalize(/255) -> project 0~15
            int aindex = (int)((a + 128.0f) / 255.0f * a_histSize);
            int bindex = (int)((b + 128.0f) / 255.0f * b_histSize);

            // Clamp
            Lindex = std::max(0, std::min(Lindex, L_histSize - 1));
            aindex = std::max(0, std::min(aindex, a_histSize - 1));
            bindex = std::max(0, std::min(bindex, b_histSize - 1));

            hist.at<float>(Lindex, aindex, bindex)++;
        }
    }

    // normalize histogram
    hist = hist / (float)(image.rows * image.cols);

    // Assign histogram values to feature vector
    if (hist.isContinuous())
    {
        float *start = (float *)hist.ptr<float>(0);
        int totalElements = L_histSize * a_histSize * b_histSize;
        featureVector->assign(start, start + totalElements);
    }
    else
    {
        featureVector->clear();
        featureVector->reserve(L_histSize * a_histSize * b_histSize);

        for (int l_idx = 0; l_idx < L_histSize; ++l_idx)
        {
            for (int a_idx = 0; a_idx < a_histSize; ++a_idx)
            {
                for (int b_idx = 0; b_idx < b_histSize; ++b_idx)
                {
                    featureVector->push_back(hist.at<float>(l_idx, a_idx, b_idx));
                }
            }
        }
    }

    return 0; // Success
}
