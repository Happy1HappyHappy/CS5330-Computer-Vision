// Authors: Claire Liu, Yu-Jing Wei
// File: imgDisplay.cpp
// Path: project1/videoDisplay/src/core/imgDisplay.cpp
// Description: Loads and displays an image using OpenCV.

#include "project1/utils/TimeUtil.hpp"
#include "project1/utils/Filters.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
const int blurTimes = 5;

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        cerr << "Invalid arguments" << endl;
        return -1;
    }

    // Load image
    cv::Mat img = cv::imread(argv[1]);
    // Display original image
    cv::namedWindow("OriginalImage", cv::WINDOW_AUTOSIZE);
    cv::imshow("OriginalImage", img);

    if (img.empty())
    {
        cerr << "Image load error!" << endl;
        return -1;
    }

    // Configure and show window
    cv::Mat blurred1(img.size(), img.type());
    cv::Mat blurred2(img.size(), img.type());

    while (true)
    {
        char key = cv::waitKey(10);
        if (key == 'q' || key == 'Q')
            break;
        if (key == 'b' || key == 'B')
        {
            // Apply blur filters
            Filters::blur5x5_1(img, blurred1, blurTimes);
            Filters::blur5x5_2(img, blurred2, blurTimes);
            // Display blurred images
            cv::namedWindow("BlurredImage1", cv::WINDOW_AUTOSIZE);
            cv::namedWindow("BlurredImage2", cv::WINDOW_AUTOSIZE);
            cv::imshow("BlurredImage1", blurred1);
            cv::imshow("BlurredImage2", blurred2);
        }
        if (key == 's' || key == 'S')
        {
            // Save images
            cv::imwrite("results/original.png", img);
            cv::imwrite("results/blurred1.png", blurred1);
            cv::imwrite("results/blurred2.png", blurred2);
            cout << "Images saved!" << endl;
        }
    }

    return 0;
}