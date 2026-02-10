/*
Claire Liu, Yu-Jing Wei
featureMatcher.cpp

Path: project2/src/online/featureMatcher.cpp
Description: Matches features from a query image to a database of feature
vectors.
*/

#include "distanceMetrics.hpp"
#include "extractorFactory.hpp"
#include "featureExtractor.hpp"
#include "featureMatcherCLI.hpp"
#include "matchResult.hpp"
#include "matchUtil.hpp"
#include "metricFactory.hpp"
#include "readFiles.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <vector>

/*
featureMatcher is the main program that matches features from a query image to
a database of feature vectors.
- @param argc The number of command line arguments.
- @param argv An array of character pointers representing the command line
arguments.
- @return 0 on success, non-zero value on error.
*/
int main(int argc, char *argv[]) {

  // Parse command line arguments
  auto args = FeatureMatcherCLI::parse(argc, argv);
  if (args.showHelp) {
    FeatureMatcherCLI::printUsage(argv[0]);
    return 0;
  }
  if (args.targetPath.empty() || args.dbs.empty() || args.topN <= 0) {
    printf("Error: missing required arguments.\n\n");
    FeatureMatcherCLI::printUsage(argv[0]);
    return -1;
  }

  // Initialize data structures to accumulate distances and track seen images
  std::unordered_map<std::string, float> totalDistance;
  std::unordered_map<std::string, bool> seenAny;

  // Iterate over each database entry
  for (const auto &dbEntry : args.dbs) {
    // Load database feature vectors and filenames from the CSV file
    std::vector<std::string> dbFilenames;   // database to save image filenames
    std::vector<std::vector<float>> dbData; // database to save feature vectors
    ReadFiles::readFeaturesFromCSV(dbEntry.dbPath.c_str(), dbFilenames, dbData);

    if (dbData.empty()) {
      printf("Warning: DB is empty: %s\n", dbEntry.dbPath.c_str());
      continue;
    }
    // Determine the metric type to use for this database entry
    MetricType metricType =
        dbEntry.hasMetric ? dbEntry.metricType : args.metricType;
    // Create the appropriate distance metric based on the specified metric type
    auto distanceMetric = MetricFactory::create(metricType);
    if (!distanceMetric) {
      printf("Error: invalid metric for db entry. db='%s'\n\n",
             dbEntry.dbPath.c_str());
      FeatureMatcherCLI::printUsage(argv[0]);
      return -1;
    }

    // Extract features from the target image using the specified feature
    // extractor
    std::vector<float> targetFeatures;
    bool targetFromDb = false;

    // Check if target image exists in DB CSV
    for (size_t i = 0; i < dbFilenames.size(); ++i) {
      if (ReadFiles::isTargetImageInDatabase(args.targetPath.c_str(),
                                             dbFilenames[i].c_str())) {
        // Target image found in DB: reuse its feature vector
        targetFeatures = dbData[i];
        targetFromDb = true;

        printf(
            "Info: target image '%s' found in DB '%s', reuse feature vector.\n",
            args.targetPath.c_str(), dbEntry.dbPath.c_str());
        break;
      }
    }

    // If target not in DB, extract features from image
    if (!targetFromDb) {
      printf("Info: target image '%s' not found in DB '%s', extract feature "
             "vector.\n",
             args.targetPath.c_str(), dbEntry.dbPath.c_str());
      printf("Extract by feature type: %s; Position: %s\n",
             dbEntry.featureName.c_str(),
             positionToString(dbEntry.position).c_str());
      auto extractor = ExtractorFactory::create(dbEntry.featureType);
      if (!extractor) {
        printf("Error: extractor nullptr for feature=%s\n",
               dbEntry.featureName.c_str());
        return -1;
      }
      int rc = extractor->extract(args.targetPath.c_str(), &targetFeatures,
                                  dbEntry.position);
      if (rc != 0) {
        printf("Error: failed to extract target features for feature=%s\n",
               dbEntry.featureName.c_str());
        return -1;
      }
    }

    // Create the appropriate distance metric based on the specified metric type
    printf("Distance metric: %s\n",
           MetricFactory::metricTypeToString(dbEntry.metricType).c_str());
    printf("Weight: %.3f\n", dbEntry.weight);
    printf("--------------------\n");
    // Compute distances between the target features and each database feature
    // vector
    for (size_t i = 0; i < dbData.size(); ++i) {
      // Skip the target image if in the database to avoid matching it with
      // itself
      if (ReadFiles::isTargetImageInDatabase(args.targetPath.c_str(),
                                             dbFilenames[i].c_str()))
        continue;
      float d = distanceMetric->compute(targetFeatures, dbData[i]);
      // Accumulate the weighted distance for this database entry
      totalDistance[dbFilenames[i]] += dbEntry.weight * d;
      // Mark this image as seen
      seenAny[dbFilenames[i]] = true;
    }
  }
  // Convert the accumulated distances into a vector of MatchResult objects
  std::vector<MatchResult> results;
  results.reserve(totalDistance.size());

  // Convert the accumulated distances into MatchResult objects
  for (const auto &kv : totalDistance) {
    if (!seenAny[kv.first])
      continue;
    MatchResult res;
    res.filename = kv.first;
    res.distance = kv.second;
    results.push_back(res);
  }

  if (results.empty()) {
    printf("No matches (check DBs / feature extraction).\n");
    return 0;
  }
  // Sort the results by distance
  std::sort(results.begin(), results.end(), MatchUtil::compareMatches);
  std::vector<MatchResult> topMatches =
      MatchUtil::getTopNMatches(results, args.topN);

  return (0); // Success
}