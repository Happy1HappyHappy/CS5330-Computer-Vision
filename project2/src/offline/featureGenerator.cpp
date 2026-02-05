/*
Claire Liu, Yu-Jing Wei
featureGenerator.cpp

Path: project2/src/offline/featureGenerator.cpp
Description: Generates feature vectors for each image in a directory and saves them to a CSV file.
*/

#include "readFiles.hpp"
#include "featureExtractor.hpp"
#include "csvUtil.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

int main(int argc, char *argv[])
{
    // argv[1]: directory path
    // argv[2]: feature type
    // argv[3]: output feature file path

    // check for sufficient arguments
    if (argc < 4)
    {
        printf("usage: %s <directory path> <feature type> <output feature file path>\n", argv[0]);
        exit(-1);
    }

    // get the directory path, feature type, and output feature file path from command line arguments
    std::string dirname = argv[1];
    FeatureType featureType = FeatureExtractor::stringToFeatureType(argv[2]);
    std::string outputFeatureFilePath = argv[3];
    printf("Processing directory %s\n", dirname.c_str());
    printf("Using feature type %s\n", FeatureExtractor::featureTypeToString(featureType));
    printf("Output feature file path: %s\n", outputFeatureFilePath.c_str());

    std::vector<std::string> imagePaths; // vector to hold file paths
    // read the files in the directory, getting their full path names in buffer
    ReadFiles::readFilesInDir((char *)dirname.c_str(), imagePaths);

    // Check the output feature file not exist or empty
    csvUtil::clearExistingFile(outputFeatureFilePath.c_str());

    std::vector<float> featureVector; // vector to hold features for each image
    // process each image file
    for (const auto &path : imagePaths)
    {
        featureVector.clear(); // clear the feature vector for each image
        // extract features from the image
        FeatureExtractor::extractFeatures(path.c_str(), featureType, &featureVector);
        printf("Extracting features from: %s\n", path.c_str());

        // save features to a file
        csvUtil::append_image_data_csv((char *)outputFeatureFilePath.c_str(), (char *)path.c_str(), featureVector, 0);
    }

    printf("Done. Processed %lu images.\n", imagePaths.size());

    return (0);
}
