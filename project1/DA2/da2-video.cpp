/*
Claire Liu, Yu-Jing Wei
Filters.cpp

Path: project1/DA2/da2-video.cpp
Description: Filters implementation file defining image filtering functions.
*/

#include <cstdio>
#include <cstring>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "DA2Network.hpp"

// opens a video stream and runs it through the depth anything network
// displays both the original video stream and the depth stream
// The red filter will draw red on the object that is the farest from the camera
int main(int argc, char *argv[]) {
  cv::VideoCapture *capdev;
  cv::Mat src; 
  cv::Mat dst;
  cv::Mat dst_vis;
  char filename[256]; // a string for the filename
  const float reduction = 0.5;

  // make a DANetwork object
  DA2Network da_net( "model_fp16.onnx" );

  // open the video device
  capdev = new cv::VideoCapture(0);
  if( !capdev->isOpened() ) {
    printf("Unable to open video device\n");
    return(-1);
  }

  cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
		 (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));

  printf("Expected size: %d %d\n", refS.width, refS.height);

  float scale_factor = 256.0 / (refS.height*reduction);
  printf("Using scale factor %.2f\n", scale_factor);

  cv::namedWindow( "Video", 1 );
  cv::namedWindow( "Depth", 2 );


  int red_limit = 350; // Threadshold of the farest
  cv::createTrackbar("Farthest Redness", "Video", &red_limit, 255);

  for(;;) {
    // capture the next frame
    *capdev >> src;
    if( src.empty()) {
      printf("frame is empty\n");
      break;
    }
    // for speed purposes, reduce the size of the input frame by half
    cv::resize( src, src, cv::Size(), reduction, reduction );

    // set the network input
    da_net.set_input( src, scale_factor );

    // run the network
    da_net.run_network( dst, src.size() );

    // intializing dst_vis
    dst_vis.create(dst.size(), CV_8UC3);


    // convert depth (1-channel) to BGR for drawing
    cv::cvtColor(dst, dst_vis, cv::COLOR_GRAY2BGR);

        // draw red on the farthest pixels
        for (int i = 0; i < dst.rows; i++) {
            for (int j = 0; j < dst.cols; j++) {
                unsigned char depth = dst.at<unsigned char>(i, j);
                cv::Vec3b &pixel = dst_vis.at<cv::Vec3b>(i, j);

                if (depth > red_limit) {
                    pixel[0] = 0;   // Blue
                    pixel[1] = 0;   // Green
                    pixel[2] = 255; // Red
                }
            }
        }


    // apply a color map to the depth output to get a good visualization
    // cv::applyColorMap(dst, dst_vis, cv::COLORMAP_INFERNO );

    // if you want to modify the src image based on the depth image, do that here
    /*
    for(int i=0;i<src.rows;i++) {
      for(int j=0;j<src.cols;j++) {
	if( dst.at<unsigned char>(i, j) < 128 ) {
	  src.at<cv::Vec3b>(i,j) = cv::Vec3b( 128, 100, 140 );
	}
      }
    }
    */

    // display the images
    cv::imshow("video", src);
    cv::imshow("depth", dst_vis);

    // terminate if the user types 'q'
    char key = cv::waitKey(10);
    if( key == 'q' ) {
      break;
    }
  }

  printf("Terminating\n");

  return(0);
}

