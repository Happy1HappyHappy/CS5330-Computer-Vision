// Authors: Claire Liu, Yu-Jing Wei
// File: vidDisplay.cpp
// Path: project1/videoDisplay/src/core/vidDisplay.cpp
// Description: Loads and displays a video using OpenCV.

#include "project1/utils/TimeUtil.hpp"
#include "project1/utils/Filters.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include <filesystem>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{

        std::filesystem::path fsPath = "./results/";
        std::error_code ec;
        cv::namedWindow("Video", 1); // identifies a window
        cv::Mat frame;               // original frame
        cv::Mat currentFrame;        // current frame
        bool isVignette = false;     // vignette flag
        const int blurTimes = 2;     // number of times to apply blur
        char colorMode = 'c';        // greyscale mode flag, default to color mode

        // create results directory if it doesn't exist
        if (std::filesystem::create_directory(fsPath, ec))
        {
                std::cout << "Folder created\n";
        }
        else if (ec)
        {
                std::cout << "Error creating folder: " << ec.message() << "\n";
                return -1;
        }
        else
        {
                std::cout << "Folder already exists\n";
        }

        // open the default video camera
        cv::VideoCapture capdev(0);
        // if not success, exit program
        if (!capdev.isOpened())
        {
                printf("Unable to open video device\n");
                return (-1);
        }

        // get some properties of the image
        cv::Size refS((int)capdev.get(cv::CAP_PROP_FRAME_WIDTH),
                      (int)capdev.get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        for (;;)
        {
                capdev >> frame; // get a new frame from the camera, treat as a stream

                // empty frame indicates end of video
                if (frame.empty())
                {
                        printf("frame is empty\n");
                        break;
                }

                // display the image
                if (colorMode == 'b' || colorMode == 'B')
                {
                        // apply blur filter verison 1
                        // Filters::blur5x5_1(frame, currentFrame, blurTimes);

                        // apply blur filter verison 2
                        Filters::blur5x5_2(frame, currentFrame, blurTimes);
                }
                else if (colorMode == 'c' || colorMode == 'C')
                {
                        // default color mode
                        currentFrame = frame;
                }
                else if (colorMode == 'e' || colorMode == 'E')
                {
                        Filters::sepia(frame, currentFrame, isVignette);
                }
                else if (colorMode == 'g' || colorMode == 'G')
                {
                        // use OpenCV's built-in grayscale conversion
                        cv::cvtColor(frame, currentFrame, cv::COLOR_BGR2GRAY);
                }
                else if (colorMode == 'h' || colorMode == 'H')
                {
                        // use our greyscale filter
                        Filters::greyscale(frame, currentFrame);
                }
                else
                {
                        // default color mode
                        currentFrame = frame;
                }

                // show the current frame
                cv::imshow("Video", currentFrame);

                // see if there is a waiting keystroke
                char key = cv::waitKey(1);

                // keypress 'q' to quit
                if (key == 'q' || key == 'Q')
                {
                        break;
                }
                else if (key == 'b' || key == 'B')
                {
                        colorMode = key;
                        cout << "Applied Blur Filter" << endl;
                }
                // keypress 'c' for color mode
                else if (key == 'c' || key == 'C')
                {
                        colorMode = key;
                        cout << "Switched to Color Mode" << endl;
                }
                // keypress 'e' for sepia tone mode
                else if (key == 'e' || key == 'E')
                {
                        colorMode = key;
                        cout << "Switched to Sepia tone Mode" << endl;
                }
                // keypress 'g' to toggle grayscale mode
                else if (key == 'g' || key == 'G')
                {
                        colorMode = key;
                        cout << "Switched to CV Grayscale Mode" << endl;
                }
                // keypress 'h' for alternative greyscale mode
                else if (key == 'h' || key == 'H')
                {
                        colorMode = key;
                        cout << "Switched to Our Greyscale Mode" << endl;
                }
                // keypress 'v' to toggle vignette filter
                else if (key == 'v' || key == 'V')
                {
                        isVignette = !isVignette;
                        cout << "Toggled Vignette Filter to " << (isVignette ? "ON" : "OFF") << endl;
                }
                // keypress 's' to save screenshot
                else if (key == 's' || key == 'S')
                {
                        // generate filename with timestamp and color mode
                        std::string filename = fsPath.string() + "screenshot_" + TimeUtil::getTimestamp() + colorMode + ".png";

                        // flags to indicate if save was successful
                        bool ok = false;

                        // save the image based on mode
                        ok = cv::imwrite(filename, currentFrame);

                        // cout the result
                        if (ok)
                        {
                                cout << "Saved Captured Frame: " << filename << endl;
                        }
                        else
                        {
                                cout << "ERROR: Could not save image." << endl;
                        }
                }
        }

        return 0;
}