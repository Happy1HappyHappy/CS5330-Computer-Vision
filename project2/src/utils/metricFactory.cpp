/*
  Claire Liu, Yu-Jing Wei
  metricFactory.cpp.cpp

  Path: project2/src/utils/metricFactory.cpp
  Description: Implements the factory method for creating feature metric instances.
*/

#include "metricFactory.hpp"
#include "distanceMetrics.hpp"
#include <memory>

/*
MetricFactory::create(MetricType type)
This static method creates and returns a shared pointer to an IDistanceMetric instance based
on the specified MetricType. It uses a switch statement to determine which type of
metric to create:
- SSD, it creates and returns a shared pointer to a SumSquaredDistance instance.
- HIST_INTERSECTION, it creates and returns a shared pointer to a HistogramIntersection instance.
- UNKNOWN_METRIC or any unrecognized type, it returns nullptr to indicate that no valid
    metric could be created.
*/
std::shared_ptr<IDistanceMetric> MetricFactory::create(MetricType type)
{
    switch (type)
    {
    case SSD:
        return std::make_shared<SumSquaredDistance>(type);
    case HIST_INTERSECTION:
        return std::make_shared<HistogramIntersection>(type);
    default:
        return nullptr;
    }
}

/*
MetricFactory::stringToMetricType(const char *typeStr)
This static method converts a string representation of a metric type to the corresponding
MetricType enum value. It compares the input string to known metric type strings:
- "ssd" returns SSD
- "hist_intersection" returns HIST_INTERSECTION
If the input string does not match any known metric type, it returns UNKNOWN_METRIC.
*/
MetricType MetricFactory::stringToMetricType(const char *typeStr)
{
    if (strcmp(typeStr, "ssd") == 0)
        return SSD;
    if (strcmp(typeStr, "hist_intersection") == 0)
        return HIST_INTERSECTION;
    return UNKNOWN_METRIC;
}

/*
MetricFactory::metricTypeToString(MetricType type)
This static method converts a MetricType enum value back to its string representation for
display purposes. It uses a switch statement to return the corresponding string for each
metric type:
- SSD returns "ssd"
- HIST_INTERSECTION returns "hist_intersection"
If the type is unrecognized, it returns "Unknown".
*/
std::string MetricFactory::metricTypeToString(MetricType type)
{
    switch (type)
    {
    case SSD:
        return "ssd";
    case HIST_INTERSECTION:
        return "hist_intersection";
    default:
        return "Unknown";
    }
}