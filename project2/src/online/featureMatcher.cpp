/*
Claire Liu, Yu-Jing Wei
featureMatcher.cpp

Path: project2/src/online/featureMatcher.cpp
Description: Matches features from a query image to a database of feature vectors.
*/

#include "csvUtil.hpp"
#include "distanceMetrics.hpp"
#include "extractorFactory.hpp"
#include "featureExtractor.hpp"
#include "matchResult.hpp"
#include "metricFactory.hpp"
#include "opencv2/opencv.hpp"
#include "readFiles.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <filesystem>

/*
compareMatches is a helper function used to sort MatchResult objects
based on their distance values in ascending order.
- @param a: The first MatchResult object to compare.
- @param b: The second MatchResult object to compare.
- @return: true if the distance of a is less than the distance of b, false otherwise.
*/
bool compareMatches(const MatchResult &a, const MatchResult &b)
{
    return a.distance < b.distance;
}

int main(int argc, char *argv[])
{
    {
        // Check for sufficient arguments
        if (argc < 6)
        {
            printf("usage: %s <target image path> <database feature vectors file> \
            <feature type> <matcher type> <N>\n",
                   argv[0]);
            exit(-1);
        }

        // Parse command line arguments
        char *targetPath = argv[1];
        char *dbPath = argv[2];
        FeatureType featureType = ExtractorFactory::stringToFeatureType(argv[3]);
        MetricType metricType = MetricFactory::stringToMetricType(argv[4]);
        int topN = atoi(argv[5]);

        // Load database feature vectors and filenames from the CSV file
        std::vector<char *> dbFilenames;        // database to save image filenames
        std::vector<std::vector<float>> dbData; // database to save feature vectors
        ReadFiles::readFeaturesFromCSV(dbPath, dbFilenames, dbData);

        // Extract features from the target image using the specified feature extractor
        std::vector<float> targetFeatures;
        auto extractor = ExtractorFactory::create(featureType);
        extractor->extract(targetPath, &targetFeatures);

        // Create the appropriate distance metric based on the specified metric type
        // Compute distances between the target features and each database feature vector,
        // and store the results in a vector of MatchResult objects
        auto distanceMetric = MetricFactory::create(metricType);
        std::vector<MatchResult> results;
        for (size_t i = 0; i < dbData.size(); ++i)
        {
            // skip if the target image is the same as the database image
            namespace fs = std::filesystem;
            if (fs::path(dbFilenames[i]).filename() == fs::path(targetPath).filename())
                continue;
            float dist = distanceMetric->compute(targetFeatures, dbData[i]);

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
}
