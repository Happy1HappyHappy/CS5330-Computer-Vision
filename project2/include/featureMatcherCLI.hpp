/*
  Claire Liu, Yu-Jing Wei
  featureMatcherCLI.hpp

  Path: project2/include/featureMatcherCLI.hpp
  Description:
*/

#pragma once
#include <string>
#include <vector>

#include "extractorFactory.hpp" // FeatureType + stringToFeatureType/featureTypeToString
#include "metricFactory.hpp"    // MetricType + stringToMetricType
#include "position.hpp"

class FeatureMatcherCLI
{
public:
    struct DbEntry
    {
        FeatureType featureType = UNKNOWN_FEATURE;
        std::string featureName; // for printing/debug
        Position position;

        MetricType metricType = UNKNOWN_METRIC;
        bool hasMetric = false;
        std::string dbPath;
    };

    struct Args
    {
        std::string targetPath;
        std::vector<DbEntry> dbs;

        MetricType metricType = UNKNOWN_METRIC;
        bool hasGlobalMetric = false;

        int topN = 0;
        bool showHelp = false;
    };

    static Args parse(int argc, char *argv[]);
    static void printUsage(const char *prog);

private:
    static bool parseDbSpec(const char *spec, DbEntry &out);
    static std::string inferFeatureKeyFromFilename(const std::string &dbPath);
};
