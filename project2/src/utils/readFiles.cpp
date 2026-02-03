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
int ReadFiles::readFilesInDir(char *dirname, char *buffer)
{
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;

    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL)
    {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }

    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL)
    {

        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif"))
        {

            printf("processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            printf("full path name: %s\n", buffer);
        }
    }

    return 0;
}
