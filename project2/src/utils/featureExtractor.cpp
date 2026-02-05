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

FeatureType FeatureExtractor::stringToFeatureType(const char *typeStr)
{
    if (strcmp(typeStr, "baseline") == 0)
        return BASELINE;
    if (strcmp(typeStr, "colorhist") == 0)
        return COLOR_HIST;
    return UNKNOWN;
}

std::string FeatureExtractor::featureTypeToString(FeatureType type)
{
    switch (type)
    {
    case BASELINE:
        return "Baseline";
    case COLOR_HIST:
        return "Color Histogram";
    case TEXTURE_SOBEL:
        return "Texture Sobel";
    default:
        return "Unknown";
    }
}

int FeatureExtractor::extractFeatures(const char *imagePath, FeatureType featureType, std::vector<float> *featureVector)
{
    // This is a function to extract features from an image given its path and feature type.
    // imagePath: path to the image file
    // featureType: type of feature to extract (e.g., "SIFT", "
    // featureVector: output array to hold the extracted features

    printf("Extracting %s features from image: %s\n", featureTypeToString(featureType).c_str(), imagePath);
    switch (featureType)
    {
    case BASELINE:
        FeatureExtractor::middle7x7(imagePath, featureVector);
        break;

    case COLOR_HIST:
        FeatureExtractor::rgColorHistogram(imagePath, featureVector);
        break;

    default:
        break;
    }

    return 0; // Return 0 to indicate success
}

int FeatureExtractor::middle7x7(const char *imagePath, std::vector<float> *featureVector)
{
    // Load the image using OpenCV
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1;
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
    // and convert the data type from 8-bit unsigned to 32-bit float
    cv::Mat flat;
    patch.reshape(1, 1).convertTo(flat, CV_32F);

    // Assign the reshaped data directly to the vector
    // This is much faster than manual loops as it utilizes block memory copying
    featureVector->assign((float *)flat.datastart, (float *)flat.dataend);

    return 0; // Success
}

int FeatureExtractor::rgColorHistogram(const char *imagePath, std::vector<float> *featureVector)
{
    // Load the image using OpenCV
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        printf("Error: Unable to load image %s\n", imagePath);
        return -1;
    }
    int histsize = 16;
    cv::Mat hist = cv::Mat::zeros(cv::Size(histsize, histsize), CV_32FC1);

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

            int rindex = (int)(r * (histsize - 1) + 0.5);
            int gindex = (int)(g * (histsize - 1) + 0.5);

            hist.at<float>(rindex, gindex)++;
        }
    }

    hist /= (image.rows * image.cols);

    // Assign histogram values to feature vector
    featureVector->assign((float *)hist.datastart, (float *)hist.dataend);

    return 0; // Success
}