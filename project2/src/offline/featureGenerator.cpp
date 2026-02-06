/*
Claire Liu, Yu-Jing Wei
featureGenerator.cpp

Path: project2/src/offline/featureGenerator.cpp
Description: Generates feature vectors for each image in a directory and saves them to a CSV file.
*/

#include "readFiles.hpp"
#include "extractorFactory.hpp"
#include "featureExtractor.hpp"
#include "csvUtil.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

int main(int argc, char *argv[])
{
    /*
    This program generates feature vectors for each image in a specified directory and
    saves them to a CSV file.
    The program takes three command line arguments: the directory path containing the images,
    the type of feature to extract (e.g., "baseline", "colorhist"), and the output file path
    for the CSV file where the features will be saved.

    - @param argc The number of command line arguments.
    - @param argv An array of character pointers representing the command line arguments.
    - @return 0 on success, non-zero value on error.
    */

    // check for sufficient arguments
    if (argc < 4)
    {
        printf("usage: %s <directory path> <feature type> <output feature file path>\n", argv[0]);
        exit(-1);
    }

    // get the directory path, feature type, and output feature file path from command line arguments
    std::string dirname = argv[1];
    FeatureType featureType = ExtractorFactory::stringToFeatureType(argv[2]);
    std::string outputFeatureFilePath = argv[3];
    printf("Processing directory %s\n", dirname.c_str());
    printf("Using feature type %s\n", ExtractorFactory::featureTypeToString(featureType).c_str());
    printf("Output feature file path: %s\n", outputFeatureFilePath.c_str());

    // read the files in the directory, get the file paths, and store them in a vector
    std::vector<std::string> imagePaths; // vector to hold file paths
    ReadFiles::readFilesInDir((char *)dirname.c_str(), imagePaths);

    // Check the output feature CSV file not exist or empty
    csvUtil::clearExistingFile(outputFeatureFilePath.c_str());

    // create the appropriate feature extractor based on the feature type and
    // extract features for each image
    std::vector<float> featureVector; // vector to hold features for each image
    auto extractor = ExtractorFactory::create(featureType);
    for (const auto &path : imagePaths)
    {
        featureVector.clear(); // clear the feature vector for each image
        printf("Extracting features from: %s\n", path.c_str());
        extractor->extract(path.c_str(), &featureVector);

        // save features in an image to output file
        csvUtil::append_image_data_csv(
            (char *)outputFeatureFilePath.c_str(),
            (char *)path.c_str(),
            featureVector,
            0);
    }

    printf("Done. Processed %lu images.\n", imagePaths.size());

    return (0);
}
