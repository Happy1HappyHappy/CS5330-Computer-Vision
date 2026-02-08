/*
Claire Liu, Yu-Jing Wei
featureExtractor.hpp

Path: include/featureExtractor.hpp
Description: Header file for featureExtractor.cpp to extract features from images.
*/

#pragma once

#include "IExtractor.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core.hpp>

struct BaselineExtractor : public IExtractor
{
    BaselineExtractor(FeatureType type) : IExtractor(type) {}
    int extractMat(const cv::Mat &image, std::vector<float> *featureVector) const override;
};

struct RGColorHistExtractor : public IExtractor
{
    RGColorHistExtractor(FeatureType type) : IExtractor(type) {}
    int extractMat(const cv::Mat &image, std::vector<float> *featureVector) const override;
};

struct RGBColorHistExtractor : public IExtractor
{
    RGBColorHistExtractor(FeatureType type) : IExtractor(type) {}
    int extractMat(const cv::Mat &image, std::vector<float> *featureVector) const override;
};

struct SobelMagnitudeExtractor : public IExtractor
{
    SobelMagnitudeExtractor(FeatureType type) : IExtractor(type) {}
    int extractMat(const cv::Mat &image, std::vector<float> *featureVector) const override;
};

struct CIELabHistExtractor : public IExtractor
{
    CIELabHistExtractor(FeatureType type) : IExtractor(type) {}
    int extractMat(const cv::Mat &image, std::vector<float> *featureVector) const override;
};

struct GaborHistExtractor : public IExtractor
{
    // Constructor to initialize the feature type and precompute Gabor filters
    GaborHistExtractor(FeatureType type) : IExtractor(type) {};
    // Override the extract function to implement the feature extraction logic for the texture Sobel extractor
    int extract(const char *imagePath, std::vector<float> *featureVector) const override;
    int GaborBankGenerator(std::vector<cv::Mat> *filters) const;
};

