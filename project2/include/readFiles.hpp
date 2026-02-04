/*
Claire Liu, Yu-Jing Wei
readFiles.hpp

Path: include/readFiles.hpp
Description: Header file for readFiles.cpp to read image files in a directory.
*/

#pragma once // Include guard

#include <opencv2/opencv.hpp>

class ReadFiles
{
public:
    static int readFilesInDir(char *dirname, std::vector<std::string> &files);
};
