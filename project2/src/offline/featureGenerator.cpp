/*
Claire Liu, Yu-Jing Wei
featureGenerator.cpp

Path: project2/src/offline/featureGenerator.cpp
Description: Generates feature files for each image in a directory.
*/

#include "readFiles.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
 */
int main(int argc, char *argv[])
{
    // check for sufficient arguments
    if (argc < 2)
    {
        printf("usage: %s <directory path>\n", argv[0]);
        exit(-1);
    }

    char dirname[256];
    char buffer[256];

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname);

    int result = ReadFiles::readFilesInDir(dirname, buffer);

    return (0);
}
