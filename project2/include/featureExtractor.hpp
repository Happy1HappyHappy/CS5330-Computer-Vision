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
