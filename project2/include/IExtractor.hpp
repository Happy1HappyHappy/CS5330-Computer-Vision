/*
Claire Liu, Yu-Jing Wei
IExtractor.hpp

Path: include/IExtractor.hpp
Description: Declares the IExtractor interface for feature extraction.
*/

#pragma once

#include "extractorFactory.hpp"
#include "position.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

/*
IExtractor interface for feature extraction.
public:
    - extract(const char *imagePath, std::vector<float> *out, Position pos):
        Extracts features from an image at a given position.
    - extractMat(const cv::Mat &image, std::vector<float> *out):
        Extracts features from a cv::Mat image.
    - type(): Returns the feature type as a string.
protected:
    - type_: The feature type.
*/
class IExtractor
{
public:
    virtual ~IExtractor() = default;
    virtual int extract(const char *imagePath, std::vector<float> *out, Position pos) const
    {
        // Load the image from the given path
        cv::Mat img = cv::imread(imagePath);
        if (img.empty())
            return -1;
        // Compute the region of interest based on the given position
        cv::Rect r = roiFor(pos, img.cols, img.rows);
        // Extract features from the region of interest
        cv::Mat roi = img(r).clone();
        fprintf(stderr, "[IExtractor::extract] %s at {%d, %d, %d, %d}\n", imagePath, r.x, r.y, r.width, r.height);
        fflush(stderr);
        return extractMat(roi, out);
    }

    virtual int extractMat(const cv::Mat &image, std::vector<float> *out) const = 0;

    virtual std::string type() const { return ExtractorFactory::featureTypeToString(type_); }

protected:
    FeatureType type_;
    explicit IExtractor(FeatureType type) : type_(type) {}
};