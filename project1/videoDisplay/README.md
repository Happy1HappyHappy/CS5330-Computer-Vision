# Video Display

**Authors:** Claire Liu, Yu-Jing Wei

## Description
This project implements a real-time video filter application and a static image display tool using OpenCV and C++. It captures video from the default camera or loads an image from a file, applying various image processing filters and effects based on user input.

## Directory Structure

```
videoDisplay/
├── Makefile
├── README.md
├── bin/
│   ├── imgDisplay (generated executable)
│   └── vidDisplay (generated executable)
├── data/
├── include/
│   └── project1/
│       └── utils/
│           ├── Filters.hpp
│           ├── TimeUtil.hpp
│           └── faceDetect.hpp
├── resources/
│   └── haarcascade_frontalface_alt2.xml
├── results/ (generated screenshots)
└── src/
    ├── core/
    │   ├── imgDisplay.cpp
    │   └── vidDisplay.cpp
    └── utils/
        ├── Filters.cpp
        ├── TimeUtil.cpp
        └── faceDetect.cpp
```

## Prerequisites
- C++ Compiler (supporting C++17)
- OpenCV (Open Source Computer Vision Library)
- Make

## Building the Project
To compile the project, run the following command in the `videoDisplay` directory:

```bash
make <target>
```

This will create the executable in the `bin` directory.

## Usage
Run the executable to start the video display:

```bash
./bin/vidDisplay
```

Or run the image display:

```bash
./bin/imgDisplay <path_to_image>
```

## Controls

The application is interactive and responds to the following key presses. Make sure the video window is focused when pressing keys.

| Key | Description |
| :--- | :--- |
| `1` | **Feature 1**: Blurs the image outside of detected faces. |
| `2` | **Feature 2**: Keeps yellow objects in color while converting the rest to grayscale. |
| `b` | **Blur**: Applies a 5x5 blur filter. |
| `c` | **Color Mode**: Displays the original color image (Default). |
| `e` | **Sepia**: Applies a sepia tone filter. |
| `f` | **Face Detection**: Detects faces and draws boxes around them. |
| `g` | **CV Grayscale**: Converts the image to grayscale using OpenCV's built-in function. |
| `h` | **Custom Grayscale**: Converts the image to grayscale using a custom implementation. |
| `i` | **Blur & Quantize**: Applies blur and color quantization. |
| `m` | **Magnitude**: Displays the magnitude of Sobel X and Y gradients. |
| `q` | **Quit**: Exits the application. |
| `s` | **Save**: Saves the current frame as a screenshot in the `results` directory. |
| `v` | **Vignette**: Toggles the vignette effect (can be combined with other modes like Sepia). |
| `x` | **Sobel X**: Displays the Sobel X gradient. |
| `y` | **Sobel Y**: Displays the Sobel Y gradient. |

## Functions

### Filters (`include/project1/utils/Filters.hpp`)

#### Color & Tone
| Function | Description |
| :--- | :--- |
| `static int greyscale(cv::Mat &src, cv::Mat &dst)` | Converts a color image to grayscale. |
| `static int sepia(cv::Mat &src, cv::Mat &dst, bool vignetteFlag = false)` | Applies a sepia tone effect, optionally with a vignette. |
| `static int remainYellowInGrey(cv::Mat &src, cv::Mat &dst)` | Converts non-yellow parts of the image to grayscale, keeping yellow objects in color. |
| `static float vignette(float x, float y, float centerX, float centerY, float radius)` | Helper function to calculate vignette scaling factor. |

#### Blurring & Quantization
| Function | Description |
| :--- | :--- |
| `static int blur5x5_1(cv::Mat &src, cv::Mat &dst, int times = 1)` | Applies a 5x5 blur filter (implementation 1). |
| `static int blur5x5_2(cv::Mat &src, cv::Mat &dst, int times = 1)` | Applies a 5x5 blur filter (implementation 2). |
| `static int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels)` | Applies blurring and color quantization to reduce the number of colors. |
| `static int blurOutsideFaces(cv::Mat &src, cv::Mat &dst, cv::Rect &last)` | Blurs the background while keeping detecting faces sharp. |

#### Edge Detection
| Function | Description |
| :--- | :--- |
| `static int sobelX3x3(cv::Mat &src, cv::Mat &dst)` | Computes the X gradient using a 3x3 Sobel operator. |
| `static int sobelY3x3(cv::Mat &src, cv::Mat &dst)` | Computes the Y gradient using a 3x3 Sobel operator. |
| `static int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst)` | Computes the gradient magnitude from X and Y gradients. |

#### Helpers
| Function | Description |
| :--- | :--- |
| `static int faceDetect(cv::Mat &src, cv::Mat &dst, cv::Rect &last)` | Detects faces in the image and draws bounding boxes (Wrapper for Face Detection). |
| `static int convolve(cv::Mat &src, cv::Mat &dst, int *kernel1, int *kernel2, int kSize, int kSum)` | Applies a convolution with provided kernels (helper for Sobel/Blur). |
| `static double getTime()` | Returns the current time (helper for internal implementation). |

### Face Detection (`include/project1/utils/faceDetect.hpp`)

| Function | Description |
| :--- | :--- |
| `int detectFaces(cv::Mat &grey, std::vector<cv::Rect> &faces)` | Detects faces in a grayscale image using OpenCV's Haar cascades. |
| `int drawBoxes(cv::Mat &frame, std::vector<cv::Rect> &faces, int minWidth = 50, float scale = 1.0)` | Draws bounding boxes around detected faces on the frame. |

### Time Utility (`include/project1/utils/TimeUtil.hpp`)

| Function | Description |
| :--- | :--- |
| `static std::string getTimestamp()` | Returns the current timestamp as a string. |
| `static double getTime()` | Returns the current time in seconds. |
