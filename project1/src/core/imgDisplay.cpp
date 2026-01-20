#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char** argv) {

    if (argc < 2) {
        cerr << "Invalid arguments" << endl;
        return -1;
    }

    // Load image
    cv::Mat img = cv::imread(argv[1]);

    if (img.empty()) {
        cerr << "Image load error!" << endl;
        return -1;
    }

    // Configure and show window 
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", img);

    // Wait for user's keypress
    while(true) {
        char key = cv::waitKey(10);
        if (key == 'q' || key == 'Q') break;
        
    }
    
    return 0;
}