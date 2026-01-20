# include "project1/utils/TimeUtil.hpp"
# include <iostream>
# include <cstdio>
# include <string>
# include <chrono>
# include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[]) {
        cv::VideoCapture capdev(0);
        // cv::VideoCapture *capdev;

        // open the video device
        // capdev = new cv::VideoCapture(0);
        if( !capdev.isOpened() ) {
                printf("Unable to open video device\n");
                return(-1);
        }

        // get some properties of the image
        cv::Size refS( (int) capdev.get(cv::CAP_PROP_FRAME_WIDTH ),
                       (int) capdev.get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        cv::namedWindow("Video", 1); // identifies a window
        cv::Mat frame;

        for(;;) {
                capdev >> frame; // get a new frame from the camera, treat as a stream
                
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }                

                cv::imshow("Video", frame);

                // see if there is a waiting keystroke
                char key = cv::waitKey(1);

                if( key == 'q' || key == 'Q') {
                    break;
                }  else if ( key == 's' || key == 'S') {
                        
                        string filename = "results/screenshot_" + TimeUtil::getTimestamp() + ".png";

                        bool ok = cv::imwrite(filename, frame);
                        if(!ok) {
                        cerr << "Failed to save image to " << filename << endl;
                        } else {
                        cout << "Saved Captured Frame: " << filename << endl;
                        }
                }
        }

        // delete capdev;
        return(0);
}