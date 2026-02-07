/*
Claire Liu, Yu-Jing Wei
featureExtractor.hpp

Path: include/featureExtractor.hpp
Description: Header file for featureExtractor.cpp to extract features from images.
*/

#pragma once // Include guard

#include "IExtractor.hpp"
#include <vector>

/*
BaselineExtractor class that implements the IExtractor interface to extract a simple
baseline feature from an image. The feature is a 7x7 patch from the center of the image,
flattened into a feature vector.
*/
struct BaselineExtractor : public IExtractor
{
    // Constructor to initialize the feature type
    BaselineExtractor(FeatureType type) : IExtractor(type) {}
    // Override the extract function to implement the feature extraction logic for the baseline extractor
    int extract(const char *imagePath, std::vector<float> *featureVector) const override;
};

struct RGColorHistExtractor : public IExtractor
{
    // Constructor to initialize the feature type
    RGColorHistExtractor(FeatureType type) : IExtractor(type) {}
    // Override the extract function to implement the feature extraction logic for the color histogram extractor
    int extract(const char *imagePath, std::vector<float> *featureVector) const override;
};

struct RGBColorHistExtractor : public IExtractor
{
    // Constructor to initialize the feature type
    RGBColorHistExtractor(FeatureType type) : IExtractor(type) {}
    // Override the extract function to implement the feature extraction logic for the color histogram extractor
    int extract(const char *imagePath, std::vector<float> *featureVector) const override;
};

struct TextureSobelExtractor : public IExtractor
{
    // Constructor to initialize the feature type
    TextureSobelExtractor(FeatureType type) : IExtractor(type) {}
    // Override the extract function to implement the feature extraction logic for the texture Sobel extractor
    int extract(const char *imagePath, std::vector<float> *featureVector) const override;
};
