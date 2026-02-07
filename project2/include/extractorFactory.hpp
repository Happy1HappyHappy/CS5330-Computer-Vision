/*
Claire Liu, Yu-Jing Wei
extractorFactory.hpp

Path: include/extractorFactory.hpp
Description: Header file for extractorFactory.cpp to
             create feature extractor instances based on feature type.
*/

#pragma once // Include guard

#include <memory>
#include <vector>

class IExtractor;

/*
Enumeration for different feature types that can be extracted from images.
- BASELINE: A simple baseline feature extractor that extracts a 7x7 patch from the
            center of the image and flattens it into a feature vector.
- RG_COLOR_HIST: A color histogram feature extractor that computes a histogram of pixel
            intensities for each color channel.
- TEXTURE_SOBEL: A texture feature extractor that uses the Sobel operator to capture
            edge information in the image.
- UNKNOWN_FEATURE: A default value for unrecognized feature types.
*/
enum FeatureType
{
    BASELINE,
    RG_COLOR_HIST,
    RGB_COLOR_HIST,
    TEXTURE_SOBEL,
    CIELAB_HIST,
    UNKNOWN_FEATURE
};

/*
ExtractorFactory class that provides a static method to create instances of IExtractor
based on the specified FeatureType.
- create(FeatureType type): A factory method that takes a FeatureType and returns a
                    shared pointer to an IExtractor instance corresponding to that type.
                    If the type is unrecognized, it returns nullptr.
- stringToFeatureType(const char *typeStr): A utility method that converts a string
                    representation of a feature type (e.g., "baseline", "colorhist") to
                    the corresponding FeatureType enum value. If the string does not match
                    any known feature type, it returns UNKNOWN_FEATURE.
- featureTypeToString(FeatureType type): A utility method that converts a FeatureType enum
                    value back to its string representation for display purposes. If the
                    type is unrecognized, it returns "Unknown".
*/
class ExtractorFactory
{
public:
    static std::shared_ptr<IExtractor> create(FeatureType type);
    static FeatureType stringToFeatureType(const char *typeStr);
    static std::string featureTypeToString(FeatureType type);
};