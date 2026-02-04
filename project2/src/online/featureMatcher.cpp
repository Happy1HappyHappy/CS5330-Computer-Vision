/*
Claire Liu, Yu-Jing Wei
featureMatcher.cpp

Path: project2/src/online/featureMatcher.cpp
Description: Matches features from a query image to a database of feature vectors.
*/

#include "distanceMetrics.hpp"
#include "featureExtractor.hpp"
#include "opencv2/opencv.hpp"
#include "csvUtil.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

struct MatchResult
{
    std::string filename;
    float distance;
};

bool compareMatches(const MatchResult &a, const MatchResult &b)
{
    return a.distance < b.distance;
}

int main(int argc, char *argv[])
{
    // argv[1]: target image file path
    // argv[2]: database feature vectors file path
    // argv[3]: N most similar images to retrieve

    if (argc < 4)
    {
        printf("usage: %s <target image path> <feature db file> <N>\n", argv[0]);
        exit(-1);
    }

    char *targetPath = argv[1]; // target image file path
    char *dbPath = argv[2];     // database feature vectors file path
    int topN = atoi(argv[3]);   // number of top matches to retrieve

    std::vector<char *> dbFilename;         // database to save image filenames
    std::vector<std::vector<float>> dbData; // database to save feature vectors
    // read database feature vectors from CSV file
    if (csvUtil::read_image_data_csv(dbPath, dbFilename, dbData, 0) != 0)
    {
        printf("Error reading CSV file.\n");
        return -1;
    }

    std::vector<float> targetFeatures; // feature vector for target image
    // extract features for target imagedatabase
    FeatureExtractor::extractFeatures(targetPath, "baseline", &targetFeatures);

    std::vector<MatchResult> results; // to store matching results
    for (size_t i = 0; i < dbData.size(); ++i)
    {
        // skip if the target image is the same as the database image
        if (strstr(dbFilename[i], targetPath) != NULL || strstr(targetPath, dbFilename[i]) != NULL)
            continue;
        // compute distance between target features and database features
        float dist = DistanceMetrics::sumSquaredDistance(targetFeatures, dbData[i]);

        MatchResult res;              // store filename and distance
        res.filename = dbFilename[i]; // database image filename
        res.distance = dist;          // computed distance
        results.push_back(res);       // add to results
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
    for (auto p : dbFilename)
        delete[] p;

    return (0);
}
