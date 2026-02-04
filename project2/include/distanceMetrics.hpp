/*
Claire Liu, Yu-Jing Wei
distanceMetrics.hpp

Path: include/distanceMetrics.hpp
Description: Header file for distanceMetrics.cpp to compute distances between feature vectors.
*/

#pragma once // Include guard

#include <opencv2/opencv.hpp>

class DistanceMetrics
{
public:
    static float sumSquaredDistance(const std::vector<float> &v1, const std::vector<float> &v2);
};