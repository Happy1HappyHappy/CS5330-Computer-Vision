/*
  Claire Liu, Yu-Jing Wei
  featureGenCLI.hpp

  Path: project2/include/featureGenCLI.hpp
  Description: Header file for featureGenCLI.cpp to parse command-line
                arguments for feature generation.
*/

#include <getopt.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

/*
FeatureGenCLI class to parse command-line arguments for feature generation.
Struct Args:
    - inputDir: The directory containing input images.
    - featureStrs: A list of feature types to extract.
    - outputPath: The path to save the extracted features.
    - positionStr: The position string specifying the region of interest.
    - showHelp: A flag indicating whether to display the help message.
public:
    parse(int argc, char *argv[]): Parses the command-line arguments and returns an Args struct.
    printUsage(const char *prog): Prints the usage information for the program.
    splitCSV(const std::string &s): Splits a comma-separated string into a vector of strings.
    pushToken(std::vector<std::string> &out, std::string token): Adds a token to the output vector.
*/
class FeatureGenCLI
{
public:
    struct Args
    {
        std::string inputDir;
        std::vector<std::string> featureStrs;
        std::string outputPath;
        std::string positionStr = "whole";
        bool showHelp = false;
    };

    static Args parse(int argc, char *argv[]);
    static void printUsage(const char *prog);

private:
    static std::vector<std::string> splitCSV(const std::string &s);
    static void pushToken(std::vector<std::string> &out, std::string token);
};
