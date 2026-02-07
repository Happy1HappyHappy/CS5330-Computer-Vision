/*
  Claire Liu, Yu-Jing Wei
  featureExtractor.cpp

  Path: project2/src/utils/featureExtractor.cpp
  Description: Extracts features from images.
*/

#include "featureExtractor.hpp"
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
int BaselineExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{
    // Load the image
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1.0f;
    }

    // Ensure the image is large enough
    if (image.rows < 7 || image.cols < 7)
    {
        printf("Error: Image %s is too small for 7x7 feature extraction\n", imagePath);
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
int RGColorHistExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{
    // Load the image
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1.0f;
    }
    // Create a 2D histogram for the red and green channels
    int histSize = 16;
    cv::Mat hist = cv::Mat::zeros(cv::Size(histSize, histSize), CV_32FC1);
    for (int i = 0; i < image.rows; i++)
    {
        cv::Vec3b *ptr = image.ptr<cv::Vec3b>(i);
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

int RGBColorHistExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{
    // Load the image
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1.0f;
    }
    // Create a 3D histogram for the red and green channels
    int histSize = 8;
    int dims = 3;
    int sizes[] = {histSize, histSize, histSize};
    cv::Mat hist = cv::Mat::zeros(dims, sizes, CV_32FC1);

    float scale = (float)histSize / 256.0f;
    for (int i = 0; i < image.rows; i++)
    {
        cv::Vec3b *ptr = image.ptr<cv::Vec3b>(i);
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

int TextureSobelExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{

    return 0; // Success
}

int CIELabHistExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{
    // Load the image
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1;
    }
    // Create a 3D histogram for the red and green channels
    int histSize = 16;
    int dims = 3;
    int sizes[] = {histSize, histSize, histSize};
    cv::Mat hist = cv::Mat::zeros(dims, sizes, CV_32FC1);

    for (int i = 0; i < image.rows; i++)
    {
        cv::Vec3b *ptr = image.ptr<cv::Vec3b>(i);
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
            float Lab_L = 116.0f * fy - 16.0f;
            float Lab_a = 500.0f * (fx - fy);
            float Lab_b = 200.0f * (fy - fz);
            
            // L (0~100) -> project 0~15
            int Lindex = (int)(L / 100.0f * histSize);
            // a, b (-128~127) -> (+128) -> normalize(/255) -> project 0~15
            int aindex = (int)((a + 128.0f) / 255.0f * histSize);
            int bindex = (int)((b + 128.0f) / 255.0f * histSize);
            
            // Clamp
            Lindex = std::max(0, std::min(Lindex, histSize - 1));
            aindex = std::max(0, std::min(aindex, histSize - 1));
            bindex = std::max(0, std::min(bindex, histSize - 1));

            hist.at<float>(Lindex, aindex, bindex)++;
        }
    }

    // normalize histogram
    hist = hist / (float)(image.rows * image.cols);

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

        for (int l_idx = 0; l_idx < histSize; ++l_idx)
        {
            for (int a_idx = 0; a_idx < histSize; ++a_idx)
            {
                for (int b_idx = 0; b_idx < histSize; ++b_idx)
                {
                    featureVector->push_back(hist.at<float>(l_idx, a_idx, b_idx));
                }
            }
        }
    }

    return 0; // Success
}
