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
#include <sstream>
#include <cctype>

// ---- local helpers (avoid name collision with cv::split) ----
namespace
{

    static bool ends_with_str(const std::string &s, const std::string &suffix)
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

    static std::string trim_copy(const std::string &s)
    {
        size_t b = 0;
        while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b])))
            ++b;
        size_t e = s.size();
        while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
            --e;
        return s.substr(b, e - b);
    }

    static std::vector<std::string> split_str(const std::string &s, char delim)
    {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
            out.push_back(trim_copy(item));
        return out;
    }

} // namespace

// e.g. "feature_vectors_rgbhist.csv" -> "rgbhist"
std::string FeatureMatcherCLI::inferFeatureKeyFromFilename(const std::string &dbPath)
{
    std::string base = basename_no_dirs(dbPath);

    if (ends_with_str(base, ".csv"))
        base = base.substr(0, base.size() - 4);

    size_t us = base.find_last_of('_');
    if (us == std::string::npos || us + 1 >= base.size())
        return "";

    return base.substr(us + 1);
}

// --db spec supports:
//  feature:position:metric[:weight]=csv
bool FeatureMatcherCLI::parseDbSpec(const char *specCStr, DbEntry &out)
{
    std::string spec = trim_copy(specCStr);

    // feature:position:metric=path
    size_t eq = spec.find('=');

    std::string lhs = trim_copy(spec.substr(0, eq));
    std::string rhs = trim_copy(spec.substr(eq + 1));

    auto parts = split_str(lhs, ':');
    if (parts.size() == 3 || parts.size() == 4)
    {
        // feature
        FeatureType ft = ExtractorFactory::stringToFeatureType(parts[0].c_str());
        if (ft == UNKNOWN_FEATURE)
            return false;

        // position
        Position pos = stringToPosition(parts[1].c_str());
        // metric
        MetricType mt = MetricFactory::stringToMetricType(parts[2].c_str());
        if (mt == UNKNOWN_METRIC)
            return false;

        // weight (optional)
        float weight = 1.0f;
        if (parts.size() == 4)
        {
            try
            {
                weight = std::stof(parts[3]);
            }
            catch (...)
            {
                return false;
            }
            if (weight <= 0.0f)
                return false;
        }

        out.featureType = ft;
        out.featureName = ExtractorFactory::featureTypeToString(ft);
        out.position = pos;
        out.metricType = mt;
        out.hasMetric = true;
        out.weight = weight;
        out.dbPath = rhs;
        return true;
    }
    return false;
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
            for (const auto &one : split_str(optarg, ','))
            {
                if (one.empty())
                    continue;
                DbEntry entry;
                if (!parseDbSpec(one.c_str(), entry))
                {
                    printf("Error: invalid --db spec '%s'\n", one.c_str());
                    args.showHelp = true;
                    break;
                }
                args.dbs.push_back(entry);
            }
            break;
        }
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
    printf("  -d, --db       <spec>  (repeatable, or comma-separated)\n");
    printf("                         format: feature:position:metric:[weight]=db_filename.csv\n");
    printf("                            feature: baseline | cielab | gabor | magnitude | rghist | rgbhist\n");
    printf("                            position: up | bottom | whole | center\n");
    printf("                            metric: ssd | hist_ix | cosine\n");
    printf("  -n, --top      <N>     number of matches to return\n");
    printf("  -h, --help             show help\n");
}
