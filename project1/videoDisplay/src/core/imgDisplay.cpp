#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char** argv) {

    if (argc < 2) {
        cerr << "Invalid input arguments" << endl;
        return -1;
    }

    // Load image
    cv::Mat src;
    cv::Mat dst;
    char filename[256];
    strcpy(filename, argv[1]);

    src = cv::imread(filename);
    
    if (src.empty()) {
        printf("Unable to read image %s\n", filename);
        exit -1;
    }

    // Configure and show window 
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    
    cv::imshow("Image", src);

    // Wait for user's keypress
    
    while(true) {
        char key = cv::waitKey(10);
        if (key == 'q' || key == 'Q') break;
        else if (key == 's' || key == 'S') {
            cv::imwrite("output_image.png", img);
            cout << "Image saved as output_image.png" << endl;
        }
        
    }
    
    return 0;
}