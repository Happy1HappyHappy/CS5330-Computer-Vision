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
#include "matchUtil.hpp"
#include "metricFactory.hpp"
#include "opencv2/opencv.hpp"
#include "readFiles.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

/*
featureMatcher is the main program that matches features from a query image to
a database of feature vectors. The program takes five command line arguments:
the path to the target image, the path to the database CSV file, the feature type,
the distance metric type, and the number of top matches to display.

- @param argc The number of command line arguments.
- @param argv An array of character pointers representing the command line arguments.
- @return 0 on success, non-zero value on error.
*/
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
        std::vector<std::string> dbFilenames;   // database to save image filenames
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
            // Skip the target image if in the database to avoid matching it with itself
            if (ReadFiles::isTargetImageInDatabase(targetPath, dbFilenames[i].c_str()))
                continue;

            MatchResult res;               // store filename and distance
            res.filename = dbFilenames[i]; // database image filename
            res.distance = distanceMetric->compute(
                targetFeatures, dbData[i]); // computed distance
            results.push_back(res);         // add to results
        }

        // Sort results by distance in ascending order
        std::sort(results.begin(), results.end(), MatchUtil::compareMatches);
        // Get and print the top N matches
        std::vector<MatchResult> topMatches = MatchUtil::getTopNMatches(results, topN);

        return (0); // Success
    }
}
