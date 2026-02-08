/*
Claire Liu, Yu-Jing Wei
IExtractor.hpp
Path: include/IExtractor.hpp
*/

#pragma once

#include "extractorFactory.hpp"
#include "position.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class IExtractor
{
public:
    virtual ~IExtractor() = default;
    virtual int extract(const char *imagePath, std::vector<float> *out, Position pos) const
    {
        cv::Mat img = cv::imread(imagePath);
        if (img.empty())
            return -1;

        cv::Rect r = roiFor(pos, img.cols, img.rows);

        cv::Mat roi = img(r).clone();
        // debug
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