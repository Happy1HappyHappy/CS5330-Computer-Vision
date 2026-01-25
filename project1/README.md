# CS5330 Computer Vision - Project 1

**Authors:** Claire Liu, Yu-Jing Wei

## Overview
This repository contains implementations for real-time video processing, static image display, and monocular depth estimation using Deep Learning. The project is organized into three main modules.

## Directory Structure

```
project1/
├── videoDisplay/      # [Main Project] Real-time video filters & effects
├── imageDisplay/      # Basic static image display tool
├── DA2/               # Depth Anything V2 (Monocular Depth Estimation)
├── Report.md          # Detailed project report
└── Assets/            # Images and resources for the report
```

## Modules

### 1. Video Display (`videoDisplay/`)
The core of Project 1. A real-time video application that captures feed from a webcam and applies various image processing filters interactively.

*   **Key Features:** Gaussian blur, Sobel edge detection, Sepia tone, Face detection & blurring, and more.
*   **Documentation:** [Read more](./videoDisplay/README.md)

### 2. Image Display (`imageDisplay/`)
A simple utility to load and display static images from the filesystem.

*   **Key Features:** Basic image loading and window management.
*   **Documentation:** [Read more](./imageDisplay/README.md)

### 3. Depth Anything V2 (`DA2/`)
An implementation of **Depth Anything V2** for monocular depth estimation using ONNX Runtime. Supports both single images and real-time video.

*   **Key Features:** Neural network-based depth maps, real-time inference, and heat map visualization.
*   **Documentation:** [Read more](./DA2/README.md)

## How to Build
Each module is self-contained with its own `Makefile`. To build a specific module, navigate to its directory and run `make`.

**Example (Video Display):**
```bash
cd videoDisplay
make
./bin/vidDisplay
```

**Example (DA2):**
```bash
cd DA2
make da2vid
./da2vid
```

## Documentation
For a detailed analysis of the implemented algorithms, experiments, and results, please refer to the main [Report.md](Report.md).

## Acknowledgements
Thanks to Professor Bruce A. Maxwell for providing guidance and starter code.
