/*
  Claire Liu, Yu-Jing Wei
  readFiles.cpp

  Path: project2/src/utils/readFiles.cpp
  Description: Reads image files in a directory.
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
int ReadFiles::readFilesInDir(char *dirname, std::vector<std::string> &files)
{
    char buffer[256]; // buffer to hold full path names
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

            // printf("processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            // printf("full path name: %s\n", buffer);

            // store the full path name in the vector
            files.push_back(std::string(buffer));
        }
    }

    return 0;
}
