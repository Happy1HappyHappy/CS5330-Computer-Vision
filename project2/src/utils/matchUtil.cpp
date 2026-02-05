/*
Claire Liu, Yu-Jing Wei
matchUtil.cpp

Path: project2/src/utils/matchUtil.cpp
Description: Matching utilities.
*/

#include "matchResult.hpp"
#include "distanceMetrics.hpp"
#include "featureExtractor.hpp"
#include "opencv2/opencv.hpp"
#include "csvUtil.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

bool compareMatches(const MatchResult &a, const MatchResult &b)
{
    return a.distance < b.distance;
}

int matchTopN(char *dbPath, std::vector<char *> dbFilenames, std::vector<std::vector<float>> dbData, std::vector<MatchResult> results, int topN)
{
    // dbPath: target image file path
    // dbFilenames: database feature vectors file path
    // dbData: feature vectors database
    // results:
    // argv[4]: N most similar images to retrieve

    std::vector<float> targetFeatures; // feature vector for target image
    // extract features for target imagedatabase
    FeatureExtractor::extractFeatures(targetPath, "baseline", &targetFeatures);

    std::vector<MatchResult> results; // to store matching results
    for (size_t i = 0; i < dbData.size(); ++i)
    {
        // skip if the target image is the same as the database image
        if (strstr(dbFilenames[i], targetPath) != NULL || strstr(targetPath, dbFilenames[i]) != NULL)
            continue;
        // compute distance between target features and database features
        float dist = DistanceMetrics::sumSquaredDistance(targetFeatures, dbData[i]);

        MatchResult res;               // store filename and distance
        res.filename = dbFilenames[i]; // database image filename
        res.distance = dist;           // computed distance
        results.push_back(res);        // add to results
    }

    // Sort results by distance in ascending order
    std::sort(results.begin(), results.end(), compareMatches);

    // Print top N matches
    printf("Top %d matches:\n", topN);
    for (int i = 0; i < topN && i < (int)results.size(); ++i)
    {
        printf("%s %f\n", results[i].filename.c_str(), results[i].distance);
    }

    // Clean up allocated memory for filenames
    for (auto p : dbFilenames)
        delete[] p;

    return (0);
}
