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
#include <opencv2/opencv.hpp>

#define HISTSIZE 16

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
ColorHistExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
This method extracts a color histogram feature from the image. It computes a 2D histogram
for the red and green channels, normalizes it, and stores the histogram values in the provided
feature vector. It uses OpenCV to load the image and perform the necessary operations.
- @param imagePath The file path of the image to extract features from.
- @param featureVector A pointer to a vector where the extracted features will be stored.
- @return 0 on success, -1 on failure (e.g., if the image cannot be loaded).
*/
int ColorHistExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{
    // Load the image
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1.0f;
    }
    // Create a 2D histogram for the red and green channels
    cv::Mat hist = cv::Mat::zeros(cv::Size(HISTSIZE, HISTSIZE), CV_32FC1);
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

            int rindex = (int)(r * (HISTSIZE - 1) + 0.5);
            int gindex = (int)(g * (HISTSIZE - 1) + 0.5);

            hist.at<float>(rindex, gindex)++;
        }
    }
    // Normalization
    hist /= (image.rows * image.cols);

    // Assign histogram values to feature vector
    featureVector->assign((float *)hist.datastart, (float *)hist.dataend);

    return 0; // Success
}

int TextureSobelExtractor::extract(const char *imagePath, std::vector<float> *featureVector) const
{

    return 0; // Success
}
