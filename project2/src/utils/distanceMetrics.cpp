/*
  Claire Liu, Yu-Jing Wei
  distanceMetrics.cpp

  Path: project2/src/utils/distanceMetrics.cpp
  Description: Implements distance metrics for feature vectors.
*/

#include "opencv2/opencv.hpp"
#include "distanceMetrics.hpp"
#include "csvUtil.hpp"
#include <cstdio>
#include <cstring>
#include <vector>

float DistanceMetrics::sumSquaredDistance(const std::vector<float> &v1, const std::vector<float> &v2)
{
    // Compute the sum of squared differences between two vectors
    if (v1.size() != v2.size())
    {
        printf("Error: Vectors must be of the same length to compute sum squared distance.\n");
        return -1.0f; // Return -1 to indicate error
    }

    float sum = 0.0f;
    for (size_t i = 0; i < v1.size(); ++i)
    {
        float diff = v1[i] - v2[i];
        sum += diff * diff;
    }
    return sum;
}
