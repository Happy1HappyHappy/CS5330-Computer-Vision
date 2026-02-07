/*
  Claire Liu, Yu-Jing Wei
  featureGenCLI.cpp

  Path: project2/src/utils/featureGenCLI.cpp
*/

#include "featureGenCLI.hpp"
#include <getopt.h>
#include <algorithm>
#include <cstdio>

FeatureGenCLI::Args FeatureGenCLI::parse(int argc, char *argv[])
{
    Args args;

    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"feature", required_argument, 0, 'f'},
        {"output", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    optind = 1; // reset getopt state

    int opt;
    while ((opt = getopt_long(argc, argv, "i:f:o:h", long_options, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'i':
            args.inputDir = optarg;
            break;
        case 'f':
        {
            auto parts = splitCSV(optarg);
            args.featureStrs.insert(args.featureStrs.end(), parts.begin(), parts.end());
            break;
        }
        case 'o':
            args.outputPath = optarg;
            break;
        case 'h':
            args.showHelp = true;
            break;
        default:
            args.showHelp = true;
            break;
        }
    }
    return args;
}

void FeatureGenCLI::printUsage(const char *prog)
{
    printf("usage:\n");
    printf("  %s --input <dir> --feature <type> [--feature <type> ...] --output <csv>\n", prog);
    printf("  %s -i <dir> -f <type1,type2,...> -o <csv>\n", prog);
    printf("\n");
    printf("options:\n");
    printf("  -i, --input    <dir>     input image directory\n");
    printf("  -f, --feature  <type>    baseline | rghist | rgbhist | magnitude\n");
    printf("                           can be repeated, or comma-separated\n");
    printf("  -o, --output   <csv>     output csv path\n");
    printf("  -h, --help               show help\n");
}

std::vector<std::string> FeatureGenCLI::splitCSV(const std::string &s)
{
    std::vector<std::string> out;
    std::string token;

    for (char c : s)
    {
        if (c == ',')
        {
            pushToken(out, token);
            token.clear();
        }
        else
        {
            token.push_back(c);
        }
    }
    pushToken(out, token);

    out.erase(std::remove_if(out.begin(), out.end(),
                             [](const std::string &x)
                             { return x.empty(); }),
              out.end());
    return out;
}

void FeatureGenCLI::pushToken(std::vector<std::string> &out, std::string token)
{
    while (!token.empty() && (token.front() == ' ' || token.front() == '\t'))
        token.erase(token.begin());
    while (!token.empty() && (token.back() == ' ' || token.back() == '\t'))
        token.pop_back();

    if (!token.empty())
        out.push_back(token);
}
