/*
Claire Liu, Yu-Jing Wei
featureGenerator.cpp

Path: project2/src/offline/featureGenerator.cpp
Description: Generates feature vectors for each image in a directory and saves
them to a CSV file.
*/

#include "csvUtil.hpp"
#include "extractorFactory.hpp"
#include "featureExtractor.hpp"
#include "featureGenCLI.hpp"
#include "position.hpp"
#include "readFiles.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>

/*
This program generates feature vectors for each image in a specified directory
and saves them to a CSV file. The program takes three command line arguments:
the directory path containing the images, the type of feature to extract (e.g.,
"baseline", "gabor"), and the output file path for the CSV file where the
features will be saved.

- @param argc The number of command line arguments.
- @param argv An array of character pointers representing the command line
arguments.
- @return 0 on success, non-zero value on error.
*/
int main(int argc, char *argv[])
{

  // Parse command line arguments
  auto args = FeatureGenCLI::parse(argc, argv);
  if (args.showHelp)
  {
    FeatureGenCLI::printUsage(argv[0]);
    return 0;
  }

  // Check required arguments
  if (args.inputDir.empty() || args.featureStrs.empty() ||
      args.outputPath.empty())
  {
    printf("Error: missing required arguments.\n\n");
    FeatureGenCLI::printUsage(argv[0]);
    return -1;
  }

  // get the directory path,  output file path and position
  std::string dirname = args.inputDir;
  std::string outputBase = args.outputPath;
  Position pos = stringToPosition(args.positionStr);
  printf("Processing directory %s\n", dirname.c_str());

  // read the files in the directory, get the file paths, and store them in a
  // vector
  std::vector<std::string> imagePaths;
  ReadFiles::readFilesInDir((char *)dirname.c_str(), imagePaths);

  // process each feature type
  for (const auto &featureStr : args.featureStrs)
  {
    FeatureType featureType =
        ExtractorFactory::stringToFeatureType(featureStr.c_str());
    std::string featureName = featureStr;
    // Check if the feature type is valid
    if (featureType == UNKNOWN_FEATURE)
    {
      printf("Error: unknown feature type '%s'\n", featureStr.c_str());
      return -1;
    }

    // generate the output file path
    std::string outPath = outputBase;
    // append feature name and position to the output file path
    if (outPath.size() >= 4 && outPath.substr(outPath.size() - 4) == ".csv")
      outPath = outPath.substr(0, outPath.size() - 4) + "_" + featureName +
                "_" + args.positionStr + ".csv";
    else
      outPath = outPath + "_" + featureName + "_" + args.positionStr + ".csv";
    printf("Using feature type %s\n", featureName.c_str());
    printf("Output feature file path: %s\n", outPath.c_str());

    // Check the output feature CSV file not exist or empty
    csvUtil::clearExistingFile(outPath.c_str());

    // create the appropriate feature extractor based on the feature type and
    // position
    auto extractor = ExtractorFactory::create(featureType);
    if (!extractor)
    {
      printf("Error: extractor is nullptr for %s\n", featureName.c_str());
      return -1;
    }
    std::vector<float> featureVector; // vector to hold features for each image
    // extract features for each image
    for (const auto &path : imagePaths)
    {
      featureVector.clear(); // clear the feature vector for each image
      int rc = extractor->extract(path.c_str(), &featureVector, pos);
      if (rc != 0)
      {
        printf("Warning: extract failed for %s\n", path.c_str());
        continue;
      }

      // save features in an image to output file
      csvUtil::append_image_data_csv(outPath.c_str(), path.c_str(),
                                     featureVector, 0);
    }
  }
  printf("Done. Processed %lu images.\n", imagePaths.size());
  return (0);
}
