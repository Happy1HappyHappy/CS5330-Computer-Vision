# CS5330 Computer Vision

**Authors:** Claire Liu, Yu-Jing Wei

## Overview
This repository contains coursework and projects for **CS5330 Pattern Recognition & Computer Vision**. It serves as a collection of our implementations, experiments, and reports throughout the course.

The projects are primarily implemented in **C++** using **OpenCV**.

## Projects

### [Project 1: Real-time Video Processing & Depth Estimation](project1/)
A comprehensive computer vision application focusing on real-time video filtering and monocular depth estimation.

*   **Path:** `project1/`
*   **Key Features:**
    *   **Video Display:** Real-time application of filters (blur, edge detection, sepia, etc.) on webcam feed.
    *   **Image Display:** Static image viewing utility.
    *   **Depth Anything V2:** Integration of the Depth Anything V2 model for monocular depth estimation.
*   **Documentation:** [See Project 1 README](project1/README.md)

### [Project 2: Content-Based Image Retrieval (CBIR)](project2/)
A flexible image retrieval system that allows searching for similar images using various content-based features.

*   **Path:** `project2/`
*   **Key Features:**
    *   **Feature Extraction:** Supports multiple methods including Baseline, Color Histograms (RG, RGB), Sobel Magnitude, CIELab, and Gabor filters.
    *   **Distance Metrics:** Implements SSD, Histogram Intersection, and Cosine Distance for robust matching.
    *   **Face Detection:** Includes utilities for detecting faces within images.
    *   **Flexible Matching:** CLI tools for both offline feature generation and online image matching with configurable weights.
*   **Documentation:** [See Project 2 README](project2/README.md)

## Tech Stack
*   **Language:** C++ (Standard C++17)
*   **Libraries:** OpenCV 4
*   **Build System:** Makefile

## Directory Structure
```
CS5330-Computer-Vision/
├── project1/       # Project 1: Real-time Filtering & Depth
├── project2/       # Project 2: Content-Based Image Retrieval
└── README.md       # This file
```
