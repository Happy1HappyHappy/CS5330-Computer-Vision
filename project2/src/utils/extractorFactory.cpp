/*
  Claire Liu, Yu-Jing Wei
  extractorFactory.cpp

  Path: project2/src/utils/extractorFactory.cpp
  Description: Implements the factory method for creating feature extractor instances.
*/

#include "extractorFactory.hpp"
#include "featureExtractor.hpp"
#include <memory>
#include <unordered_map>

/*
ExtractorFactory::create(FeatureType type)
This static method creates and returns a shared pointer to an IExtractor instance based
on the specified FeatureType. It uses a switch statement to determine which type of
extractor to create:
- BASELINE, it creates and returns a shared pointer to a BaselineExtractor instance.
- RG_COLOR_HIST, it creates and returns a shared pointer to a RGColorHistExtractor instance.
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
    case RG_HIST_2D:
        return std::make_shared<RGColorHistExtractor>(type);
    case RGB_HIST_3D:
        return std::make_shared<RGBColorHistExtractor>(type);
    case TEXTURE_SOBEL:
        return std::make_shared<TextureSobelExtractor>(type);
    case CIELAB_HIST:
        return std::make_shared<CIELabHistExtractor>(type);
    default:
        return nullptr;
    }
}

/*
ExtractorFactory::stringToFeatureType(const char *typeStr)
This static method converts a string representation of a feature type to the corresponding
FeatureType enum value. It compares the input string to known feature type strings:
- "baseline" returns BASELINE
- "colorhist" returns RG_COLOR_HIST
- "texturesobel" returns TEXTURE_SOBEL
If the input string does not match any known feature type, it returns UNKNOWN_FEATURE.
*/
FeatureType ExtractorFactory::stringToFeatureType(const char *typeStr)
{
    static const std::unordered_map<std::string, FeatureType> typeMap = {
        {"baseline", BASELINE},
        {"rghist2d", RG_HIST_2D},
        {"rgbhist3d", RGB_HIST_3D},
        {"texturesobel", TEXTURE_SOBEL},
        {"cielab", CIELAB_HIST}
        };


    auto it = typeMap.find(typeStr);
    return (it != typeMap.end()) ? it->second : UNKNOWN_FEATURE;
}

/*
ExtractorFactory::featureTypeToString(FeatureType type)
This static method converts a FeatureType enum value back to its string representation for
display purposes. It uses a switch statement to return the corresponding string for each
FeatureType:
- BASELINE returns "baseline"
- RG_COLOR_HIST returns "colorhist"
- TEXTURE_SOBEL returns "texturesobel"
If the type is unrecognized, it returns "Unknown".
*/
std::string ExtractorFactory::featureTypeToString(FeatureType type)
{
    static const std::unordered_map<FeatureType, std::string> reverseMap = {
        {BASELINE, "baseline"},
        {RG_HIST_2D, "rghist2d"},
        {RGB_HIST_3D, "rgbhist3d"},
        {TEXTURE_SOBEL, "texturesobel"},
        {CIELAB_HIST, "cielab"}};

    auto it = reverseMap.find(type);
    return (it != reverseMap.end()) ? it->second : "Unknown";
}