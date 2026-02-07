/*
  Claire Liu, Yu-Jing Wei
  featureGenCLI.hpp

  Path: project2/include/featureGenCLI.hpp
  Description:
*/

#include <getopt.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

class FeatureGenCLI
{
public:
    struct Args
    {
        std::string inputDir;
        std::vector<std::string> featureStrs;
        std::string outputPath;
        bool showHelp = false;
    };

    static Args parse(int argc, char *argv[]);

    static void printUsage(const char *prog);

private:
    static std::vector<std::string> splitCSV(const std::string &s);
    static void pushToken(std::vector<std::string> &out, std::string token);
};
