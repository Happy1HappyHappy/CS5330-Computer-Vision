/*
Claire Liu, Yu-Jing Wei
IExtractor.hpp

Path: include/IExtractor.hpp
Description: Header file for IExtractor to define the interface for feature extractors.
*/

#pragma once

#include "extractorFactory.hpp"
#include <vector>

/*
IExtractor is an abstract base class that defines the interface for feature extractors used
to extract features from images.
- extract(
        const char *imagePath,
        std::vector<float> *featureVector) const:
    A pure virtual function that takes the file path of an image and a pointer to a vector to
    store the extracted feature vector. This function must be overridden by any concrete feature
    extractor class that inherits from IExtractor.
    It returns an integer status code (0 for success, -1 for failure) to indicate whether the
    feature extraction was successful.
- type() const: A virtual function that returns the FeatureType of the extractor. This allows
    users to identify which type of feature extractor is being used when extracting features from images.
*/
class IExtractor
{
public:
    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~IExtractor() = default;

    virtual int extract(const char *imagePath,
                        std::vector<float> *featureVector) const = 0;

    virtual FeatureType type() const { return type_; }

protected:
    // Protected member variable to store the type of the feature extractor
    FeatureType type_;
    // Constructor to initialize the feature type in derived classes
    explicit IExtractor(FeatureType type) : type_(type) {}
};