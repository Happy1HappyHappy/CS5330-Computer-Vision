/*
Claire Liu, Yu-Jing Wei
matchUitl.hpp

Path: include/matchUitl.hpp
Description: Header file for matchUitl.cpp to matching utilities.
*/

#pragma once // Include guard

#include <opencv2/opencv.hpp>

class MatchUtil
{
public:
    static float matchTopN(
        char *dbPath,
        std::vector<char *> dbFilenames,
        std::vector<std::vector<float>> dbData,
        std::vector<MatchResult> results,
        int topN);
};