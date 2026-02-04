/*
Claire Liu, Yu-Jing Wei
featureExtractor.hpp

Path: include/featureExtractor.hpp
Description: Header file for featureExtractor.cpp to extract features from images.
*/

#pragma once // Include guard

#include <opencv2/opencv.hpp>

enum FeatureType
{
    BASELINE,
    COLOR_HIST,
    TEXTURE_SOBEL,
    UNKNOWN
};

class FeatureExtractor
{
public:
    static int extractFeatures(const char *imagePath, const char *featureType, std::vector<float> *featureVector);

private:
    static FeatureType stringToFeatureType(const char *typeStr);
    static int middle7x7(const char *imagePath, std::vector<float> *featureVector);
};