/*
  Claire Liu, Yu-Jing Wei
  featureMatcherCLI.cpp

  Path: project2/src/utils/featureMatcherCLI.cpp
*/

#include "featureMatcherCLI.hpp"
#include <getopt.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

static bool ends_with(const std::string &s, const std::string &suffix)
{
    if (s.size() < suffix.size())
        return false;
    return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static std::string basename_no_dirs(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
        return path;
    return path.substr(pos + 1);
}

// e.g. "feature_vectors_rgbhist.csv" -> "rgbhist"
std::string FeatureMatcherCLI::inferFeatureKeyFromFilename(const std::string &dbPath)
{
    std::string base = basename_no_dirs(dbPath);

    if (ends_with(base, ".csv"))
        base = base.substr(0, base.size() - 4);

    size_t us = base.find_last_of('_');
    if (us == std::string::npos || us + 1 >= base.size())
        return "";

    return base.substr(us + 1);
}

// --db spec supports:
//  A) "rghist=path.csv"
//  B) "path_with_suffix_rghist.csv"  (infer from suffix)
bool FeatureMatcherCLI::parseDbSpec(const char *specCStr, DbEntry &out)
{
    std::string spec(specCStr);

    // A) key=path
    size_t eq = spec.find('=');
    if (eq != std::string::npos)
    {
        std::string key = spec.substr(0, eq);
        std::string path = spec.substr(eq + 1);

        if (key.empty() || path.empty())
            return false;

        FeatureType ft = ExtractorFactory::stringToFeatureType(key.c_str());
        if (ft == UNKNOWN_FEATURE)
            return false;

        out.featureType = ft;
        out.featureName = ExtractorFactory::featureTypeToString(ft);
        out.dbPath = path;
        return true;
    }

    // B) infer from filename
    std::string key = inferFeatureKeyFromFilename(spec);
    if (key.empty())
        return false;

    FeatureType ft = ExtractorFactory::stringToFeatureType(key.c_str());
    if (ft == UNKNOWN_FEATURE)
        return false;

    out.featureType = ft;
    out.featureName = ExtractorFactory::featureTypeToString(ft);
    out.dbPath = spec;
    return true;
}

FeatureMatcherCLI::Args FeatureMatcherCLI::parse(int argc, char *argv[])
{
    Args args{};
    args.metricType = MetricFactory::stringToMetricType("unknown"); // default

    static struct option long_options[] = {
        {"target", required_argument, 0, 't'},
        {"db", required_argument, 0, 'd'}, // repeatable
        {"metric", required_argument, 0, 'm'},
        {"top", required_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    optind = 1;

    int opt;
    while ((opt = getopt_long(argc, argv, "t:d:m:n:h", long_options, nullptr)) != -1)
    {
        switch (opt)
        {
        case 't':
            args.targetPath = optarg;
            break;
        case 'd':
        {
            DbEntry entry;
            if (!parseDbSpec(optarg, entry))
            {
                printf("Error: invalid --db spec '%s'\n", optarg);
                args.showHelp = true;
                break;
            }
            args.dbs.push_back(entry);
            break;
        }
        case 'm':
            args.metricType = MetricFactory::stringToMetricType(optarg);
            break;
        case 'n':
            args.topN = std::atoi(optarg);
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

void FeatureMatcherCLI::printUsage(const char *prog)
{
    printf("usage:\n");
    printf("  %s --target <img> --db <feature=csv | csv> [--db ...] --metric <type> --top <N>\n", prog);
    printf("  %s -t <img> -d <feature=csv | csv> [-d ...] -m <type> -n <N>\n", prog);
    printf("\n");
    printf("options:\n");
    printf("  -t, --target   <img>   target image path\n");
    printf("  -d, --db       <spec>  database CSV spec (repeatable)\n");
    printf("                    format A: rghist=feature_vectors_rghist.csv\n");
    printf("                    format B: feature_vectors_rghist.csv  (infer feature from _<feature>.csv)\n");
    printf("  -m, --metric   <type>  distance metric type (per your MetricFactory mapping)\n");
    printf("  -n, --top      <N>     number of matches to return\n");
    printf("  -h, --help             show help\n");
}
