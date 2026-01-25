/*
Claire Liu, Yu-Jing Wei
vidDisplay.cpp

Path: project1/videoDisplay/src/core/vidDisplay.cpp
Description: Loads and displays a video using OpenCV.
*/

#include "project1/utils/TimeUtil.hpp"
#include "project1/utils/Filters.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
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
        cv::Rect last(0, 0, 0, 0);   // last detected face rectangle
        bool isVignette = false;     // vignette flag
        const int blurTimes = 2;     // number of times to apply blur
        char colorMode = 'c';        // greyscale mode flag, default to color mode

        // Video recording variables
        cv::VideoWriter videoWriter; // Object to handle video file output
        bool isRecording = false; // Flag to indicate if recording is active
        double fps = 24.0; // Frames per second for the output video

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
                if (colorMode == '1')
                {
                        // Feature 1: Blur the image outside of found faces
                        Filters::blurOutsideFaces(frame, currentFrame, last);
                }
                else if (colorMode == '2')
                {
                        // apply remain color filter
                        Filters::remainYellowInGrey(frame, currentFrame);
                }
                else if (colorMode == '3')
                {
                        // apply make face colorful filter
                        Filters::makeFaceColorful(frame, currentFrame, last);
                }
                else if (colorMode == 'b' || colorMode == 'B')
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
                        // apply sepia tone filter
                        Filters::sepia(frame, currentFrame, isVignette);
                }
                else if (colorMode == 'f' || colorMode == 'F')
                {
                        // apply face detection filter
                        Filters::faceDetect(frame, currentFrame, last);
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
                else if (colorMode == 'i' || colorMode == 'I')
                {
                        // apply blur and quantization filter
                        Filters::blurQuantize(frame, currentFrame, 10);
                }
                else if (colorMode == 'm' || colorMode == 'M')
                {
                        // apply magnitude of Sobel X and Y
                        cv::Mat sx, sy;
                        Filters::sobelX3x3(frame, sx);
                        Filters::sobelY3x3(frame, sy);
                        Filters::magnitude(sx, sy, currentFrame);
                }
                else if (colorMode == 'x' || colorMode == 'X')
                {
                        // apply Sobel X filter
                        Filters::sobelX3x3(frame, currentFrame);
                }
                else if (colorMode == 'y' || colorMode == 'Y')
                {
                        // apply Sobel Y filter
                        Filters::sobelY3x3(frame, currentFrame);
                }
                else
                {
                        // default color mode
                        currentFrame = frame;
                }

                // if recording is active, write the frame to the video file
                if (isRecording && videoWriter.isOpened()) 
                {
                        // Write the current processed frame (with all filters applied) to the file
                        videoWriter.write(currentFrame);

                        // Visual feedback: Draw a red recording dot on the live display
                        cv::circle(currentFrame, cv::Point(30, 30), 10, cv::Scalar(0, 0, 255), -1);
                        cv::putText(currentFrame, "REC", cv::Point(50, 42), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
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
                // keypress '1' for feature 1
                else if (key == '1')
                {
                        colorMode = key;
                        cout << "Applied feature 1: Blur the image outside of found faces." << endl;
                }
                // keypress '2' for feature 2
                else if (key == '2')
                {
                        colorMode = key;
                        cout << "Applied feature 2: Remain Yellow In Grey" << endl;
                }
                // keypress '3' for feature 3
                else if (key == '3')
                {
                        colorMode = key;
                        cout << "Applied feature 3: Make Face Colorful" << endl;
                }
                // keypress 'b' for blur mode
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
                // keypress 'f' for face detection mode
                else if (key == 'f' || key == 'F')
                {
                        colorMode = key;
                        cout << "Switched to Face Detection Mode" << endl;
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
                // keypress 'i' for blur and quantization mode
                else if (key == 'i' || key == 'I')
                {
                        colorMode = key;
                        cout << "Switched to Blur and Quantization Mode" << endl;
                }
                // keypress 'm' for magnitude of Sobel X and Y mode
                else if (key == 'm' || key == 'M')
                {
                        colorMode = key;
                        cout << "Switched to Magnitude of Sobel X and Y Mode" << endl;
                }

                // keypress 'r' for recording mode
                else if (key == 'r' || key == 'R') 
                {
                        isRecording = !isRecording; // toggle the recording state

                        if (isRecording) 
                        {
                                // Generate  filename using a timestamp 
                                std::string vidPath = fsPath.string() + "record_" + TimeUtil::getTimestamp() + ".avi";
                                
                                // Setup MJPG codec and 30 FPS
                                int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

                                // Open the writer using the camera's resolution (refS)
                                videoWriter.open(vidPath, fourcc, fps, refS, true);

                                if (!videoWriter.isOpened()) 
                                {
                                std::cerr << "ERROR: Failed to open video file for writing." << std::endl;
                                isRecording = false;
                                } 
                                else 
                                {
                                std::cout << "STARTED Recording: " << vidPath << std::endl;
                                }
                        } else 
                        {
                                // Release the writer to finalize the video file
                                videoWriter.release();
                                std::cout << "STOPPED Recording. File saved." << std::endl;
                        }
                }
                // keypress 'v' to toggle vignette filter
                else if (key == 'v' || key == 'V')
                {
                        isVignette = !isVignette;
                        cout << "Toggled Vignette Filter to " << (isVignette ? "ON" : "OFF") << endl;
                }
                // keypress 'x' for Sobel X mode
                else if (key == 'x' || key == 'X')
                {
                        colorMode = key;
                        cout << "Switched to Sobel X Mode" << endl;
                }
                // keypress 'y' for Sobel Y mode
                else if (key == 'y' || key == 'Y')
                {
                        colorMode = key;
                        cout << "Switched to Sobel Y Mode" << endl;
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