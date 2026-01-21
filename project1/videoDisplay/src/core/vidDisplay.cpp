#include "project1/utils/TimeUtil.hpp"
#include "project1/utils/Filters.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
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

        cv::namedWindow("Video", 1); // identifies a window
        cv::Mat frame;               // current frame
        cv::Mat grey;                // greyed current frame

        char colorMode = 'c'; // greyscale mode flag

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
                if (colorMode == 'g' || colorMode == 'G')
                {
                        // use OpenCV's built-in grayscale conversion
                        cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
                        cv::imshow("Video", grey);
                }
                else if (colorMode == 'h' || colorMode == 'H')
                {
                        // use our greyscale filter
                        cv::imshow("Video", grey);
                }
                else
                {
                        // color mode
                        cv::imshow("Video", frame);
                }

                // see if there is a waiting keystroke
                char key = cv::waitKey(1);

                // keypress 'q' to quit
                if (key == 'q' || key == 'Q')
                {
                        break;
                }
                // keypress 'c' for color mode
                else if (key == 'c' || key == 'C')
                {
                        colorMode = 'c';
                        cout << "Switched to Color Mode" << endl;
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
                // keypress 's' to save screenshot
                else if (key == 's' || key == 'S')
                {
                        string time = TimeUtil::getTimestamp();
                        string filenameColor = "results/screenshot_" + time + ".png";
                        string filenameGrey = "results/screenshot_" + time + "_grey.png";

                        bool ok;
                        bool greyOk;

                        // write the image
                        ok = cv::imwrite(filenameColor, frame);

                        if (colorMode == 'g' || colorMode == 'G' || colorMode == 'h' || colorMode == 'H')
                        {
                                // write the grey image
                                greyOk = cv::imwrite(filenameGrey, grey);
                        }

                        // cout the result
                        if (ok)
                        {
                                cout << "Saved Captured Frame: " << filenameColor << endl;
                        }
                        if (greyOk)
                        {
                                cout << "Saved Captured Frame: " << filenameGrey << endl;
                        }
                        if (!ok && !greyOk)
                        {
                                cout << "ERROR: Could not save image." << endl;
                        }
                }
        }

        return (0);
}