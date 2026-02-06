/*
  Claire Liu, Yu-Jing Wei
  extractorFactory.cpp

  Path: project2/src/utils/extractorFactory.cpp
  Description: Implements the factory method for creating feature extractor instances.
*/

#include "extractorFactory.hpp"
#include "featureExtractor.hpp"
#include <memory>

/*
ExtractorFactory::create(FeatureType type)
This static method creates and returns a shared pointer to an IExtractor instance based
on the specified FeatureType. It uses a switch statement to determine which type of
extractor to create:
- BASELINE, it creates and returns a shared pointer to a BaselineExtractor instance.
- COLOR_HIST, it creates and returns a shared pointer to a ColorHistExtractor instance.
- TEXTURE_SOBEL, it creates and returns a shared pointer to a TextureSobelExtractor instance.
- UNKNOWN_FEATURE or any unrecognized type, it returns nullptr to indicate that no valid
    extractor could be created.
*/
std::shared_ptr<IExtractor> ExtractorFactory::create(FeatureType type)
{
    switch (type)
    {
    case BASELINE:
        return std::make_shared<BaselineExtractor>(type);
    case COLOR_HIST:
        return std::make_shared<ColorHistExtractor>(type);
    case TEXTURE_SOBEL:
        return std::make_shared<TextureSobelExtractor>(type);
    default:
        return nullptr;
    }
}

/*
ExtractorFactory::stringToFeatureType(const char *typeStr)
This static method converts a string representation of a feature type to the corresponding
FeatureType enum value. It compares the input string to known feature type strings:
- "baseline" returns BASELINE
- "colorhist" returns COLOR_HIST
- "texturesobel" returns TEXTURE_SOBEL
If the input string does not match any known feature type, it returns UNKNOWN_FEATURE.
*/
FeatureType ExtractorFactory::stringToFeatureType(const char *typeStr)
{
    if (strcmp(typeStr, "baseline") == 0)
        return BASELINE;
    if (strcmp(typeStr, "colorhist") == 0)
        return COLOR_HIST;
    return UNKNOWN_FEATURE;
}

/*
ExtractorFactory::featureTypeToString(FeatureType type)
This static method converts a FeatureType enum value back to its string representation for
display purposes. It uses a switch statement to return the corresponding string for each
FeatureType:
- BASELINE returns "baseline"
- COLOR_HIST returns "colorhist"
- TEXTURE_SOBEL returns "texturesobel"
If the type is unrecognized, it returns "Unknown".
*/
std::string ExtractorFactory::featureTypeToString(FeatureType type)
{
    switch (type)
    {
    case BASELINE:
        return "baseline";
    case COLOR_HIST:
        return "colorhist";
    case TEXTURE_SOBEL:
        return "texturesobel";
    default:
        return "Unknown";
    }
}