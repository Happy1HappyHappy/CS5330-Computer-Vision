/*
Claire Liu, Yu-Jing Wei
ReadFiles.hpp

Path: include/readfiles.h
Description: Header file for readfiles.cpp to read image files in a directory.
*/

#pragma once // Include guard

#include <opencv2/opencv.hpp>

class ReadFiles
{
public:
    static int readFilesInDir(char *dirname, char *buffer);
};
