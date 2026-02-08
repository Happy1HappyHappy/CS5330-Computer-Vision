/*
  Claire Liu, Yu-Jing Wei
  featureMatcherCLI.hpp

  Path: project2/include/featureMatcherCLI.hpp
  Description: Header file for featureMatcherCLI.cpp to parse command-line
                arguments for feature matching.
*/

#pragma once
#include <string>
#include <vector>

#include "extractorFactory.hpp"
#include "metricFactory.hpp"
#include "position.hpp"

/*
FeatureMatcherCLI class to parse command-line arguments for feature matching.
Struct DbEntry: Represents a database entry for feature matching.
Struct Args: Represents the command-line arguments for feature matching.
public:
    - parse(int argc, char *argv[]): Parses the command-line arguments and returns an Args struct.
    - printUsage(const char *prog): Prints the usage information for the program.
private:
    - parseDbSpec(const char *spec, DbEntry &out): Parses a database specification string into a DbEntry struct.
    - inferFeatureKeyFromFilename(const std::string &dbPath): Infers the feature key from a database filename.

*/
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
        float weight = 1.0f;
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
