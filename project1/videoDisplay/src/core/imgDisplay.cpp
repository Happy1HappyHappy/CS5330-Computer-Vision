/*
Claire Liu, Yu-Jing Wei
imgDisplay.cpp

Path: project1/videoDisplay/src/core/imgDisplay.cpp
Description: Loads and displays an image using OpenCV, with interactive filter
modes similar to vidDisplay.
*/

#include "project1/utils/Filters.hpp"
#include "project1/utils/TimeUtil.hpp"
#include "project1/utils/faceDetect.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: ./bin/imgDisplay <path_to_image>" << endl;
    return -1;
  }

  // Load image
  cv::Mat img = cv::imread(argv[1]);
  if (img.empty()) {
    cerr << "Image load error: " << argv[1] << endl;
    return -1;
  }

  std::filesystem::path fsPath = "./results/";
  std::error_code ec;

  // create results directory if it doesn't exist
  if (std::filesystem::create_directory(fsPath, ec)) {
    std::cout << "Folder created\n";
  } else if (ec) {
    std::cout << "Error creating folder: " << ec.message() << "\n";
    // continue anyway, might already exist or we can fail later on save
  }

  cv::namedWindow("Image", 1);
  cv::Mat currentFrame;      // current processed frame
  cv::Rect last(0, 0, 0, 0); // last detected face rectangle
  bool isVignette = false;   // vignette flag
  const int blurTimes = 2;   // number of times to apply blur
  char colorMode = 'c';      // default to color mode

  // Helper to print current mode
  auto printMode = [&](const string &msg) { cout << msg << endl; };

  while (true) {
    // Process image based on mode
    // Note: We always start from the original 'img' to avoid accumulating
    // effects indefinitely (unless that's desired, but for mode switching
    // usually we want fresh application)

    if (colorMode == '1') {
      Filters::blurOutsideFaces(img, currentFrame, last);
    } else if (colorMode == '2') {
      Filters::remainYellowInGrey(img, currentFrame);
    } else if (colorMode == 'b' || colorMode == 'B') {
      // Filters::blur5x5_1(img, currentFrame, blurTimes);
      Filters::blur5x5_2(img, currentFrame, blurTimes);
    } else if (colorMode == 'c' || colorMode == 'C') {
      currentFrame = img.clone();
    } else if (colorMode == 'e' || colorMode == 'E') {
      Filters::sepia(img, currentFrame, isVignette);
    } else if (colorMode == 'f' || colorMode == 'F') {
      // For static image, face detect is same every frame, but we call it to
      // visualize
      Filters::faceDetect(img, currentFrame, last);
    } else if (colorMode == 'g' || colorMode == 'G') {
      cv::cvtColor(img, currentFrame, cv::COLOR_BGR2GRAY);
    } else if (colorMode == 'h' || colorMode == 'H') {
      Filters::greyscale(img, currentFrame);
    } else if (colorMode == 'i' || colorMode == 'I') {
      Filters::blurQuantize(img, currentFrame, 10);
    } else if (colorMode == 'm' || colorMode == 'M') {
      cv::Mat sx, sy;
      Filters::sobelX3x3(img, sx);
      Filters::sobelY3x3(img, sy);
      Filters::magnitude(sx, sy, currentFrame);
    } else if (colorMode == 'x' || colorMode == 'X') {
      Filters::sobelX3x3(img, currentFrame);
    } else if (colorMode == 'y' || colorMode == 'Y') {
      Filters::sobelY3x3(img, currentFrame);
    } else {
      currentFrame = img.clone();
    }

    cv::imshow("Image", currentFrame);

    char key = cv::waitKey(
        0); // Wait indefinitely for key press since it's a static image

    // keypress 'q' to quit
    if (key == 'q' || key == 'Q') {
      break;
    }
    // keypress '1' for feature 1
    else if (key == '1') {
      colorMode = key;
      printMode("Applied feature 1: Blur the image outside of found faces.");
    }
    // keypress '2' for feature 2
    else if (key == '2') {
      colorMode = key;
      printMode("Applied feature 2: Keep yellow specific.");
    }
    // keypress 'b' for blur mode
    else if (key == 'b' || key == 'B') {
      colorMode = key;
      printMode("Applied Blur Filter");
    }
    // keypress 'c' for color mode
    else if (key == 'c' || key == 'C') {
      colorMode = key;
      printMode("Switched to Color Mode");
    }
    // keypress 'e' for sepia tone mode
    else if (key == 'e' || key == 'E') {
      colorMode = key;
      printMode("Switched to Sepia tone Mode");
    }
    // keypress 'f' for face detection mode
    else if (key == 'f' || key == 'F') {
      colorMode = key;
      printMode("Switched to Face Detection Mode");
    }
    // keypress 'g' to toggle grayscale mode
    else if (key == 'g' || key == 'G') {
      colorMode = key;
      printMode("Switched to CV Grayscale Mode");
    }
    // keypress 'h' for alternative greyscale mode
    else if (key == 'h' || key == 'H') {
      colorMode = key;
      printMode("Switched to Our Greyscale Mode");
    }
    // keypress 'i' for blur and quantization mode
    else if (key == 'i' || key == 'I') {
      colorMode = key;
      printMode("Switched to Blur and Quantization Mode");
    }
    // keypress 'm' for magnitude of Sobel X and Y mode
    else if (key == 'm' || key == 'M') {
      colorMode = key;
      printMode("Switched to Magnitude of Sobel X and Y Mode");
    }
    // keypress 'v' to toggle vignette filter
    else if (key == 'v' || key == 'V') {
      isVignette = !isVignette;
      cout << "Toggled Vignette Filter to " << (isVignette ? "ON" : "OFF")
           << endl;
      // Force re-render if we are in Sepia mode, otherwise user has to press
      // 'e' again to see it
      if (colorMode == 'e' || colorMode == 'E') {
        // loop will continue and re-render
      }
    }
    // keypress 'x' for Sobel X mode
    else if (key == 'x' || key == 'X') {
      colorMode = key;
      printMode("Switched to Sobel X Mode");
    }
    // keypress 'y' for Sobel Y mode
    else if (key == 'y' || key == 'Y') {
      colorMode = key;
      printMode("Switched to Sobel Y Mode");
    }
    // keypress 's' to save screenshot
    else if (key == 's' || key == 'S') {
      std::string filename = fsPath.string() + "screenshot_" +
                             TimeUtil::getTimestamp() + colorMode + ".png";
      bool ok = cv::imwrite(filename, currentFrame);
      if (ok) {
        cout << "Saved Captured Frame: " << filename << endl;
      } else {
        cout << "ERROR: Could not save image." << endl;
      }
    }
  }

  return 0;
}