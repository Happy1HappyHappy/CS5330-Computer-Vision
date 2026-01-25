# Image Display

**Authors:** Claire Liu, Yu-Jing Wei

## Description
A simple C++ program that loads and displays an image using OpenCV. It creates a window to show the specified image and waits for user input to close.

## Prerequisites
- C++ Compiler (supporting C++11)
- OpenCV (Open Source Computer Vision Library)
- Make

## Building the Project
To compile the project, run the following command in the `imageDisplay` directory:

```bash
make
```

This will create the executable in the `bin` directory.

## Usage
Run the executable with the path to an image file as an argument:

```bash
./bin/imgdisplay <path_to_image>
```

**Example:**
```bash
./bin/imgdisplay ../data/example.jpg
```

## Controls
- **'q' or 'Q'**: Close the image window and exit the program.
