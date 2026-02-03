# Content-Based Image Retrieval (CBIR) Project

This project implements a Content-Based Image Retrieval system to search for similar images based on visual features.

## Project Structure

The project is organized as follows:

```text
CBIR_Project/
├── Makefile                   # Build system
├── data/
│   ├── olympus/               # Image database
│   └── features.csv           # Extracted feature storage
├── include/                   # Header files
│   ├── csvUtil.hpp            # CSV read/write utilities
│   ├── distanceMetrics.hpp    # Similarity metrics (SSD, Intersection, etc.)
│   ├── featureExtractor.hpp   # Feature extraction logic
│   └── readFiles.hpp          # File reading utilities
├── src/
│   ├── offline/
│   │   └── featureGenerator.cpp # CLI for feature extraction
│   ├── online/
│   │   ├── main.cpp           # App entry point
│   │   ├── mainWindow.cpp     # GUI implementation
│   │   └── mainWindow.h       # GUI definition
│   └── utils/
│       └── readfiles.cpp      # File utility implementation
├── resources/                 # Application resources
├── bin/                       # Executables output
└── obj/                       # Compiled objects
```

## Features

- **Feature Extraction**:
  - Multi-dimensional color histograms.
  - Sobel-based texture analysis.
- **Distance Metrics**:
  - Sum of Squared Differences (SSD).
  - Histogram Intersection.

## Prerequisites

- **OpenCV**: Required for image processing.
- **Qt**: Required for the graphical user interface.
- **Xcode/g++**: Compiler support for C++17.

## Build Instructions

Use the provided `Makefile` to compile the project components:

1.  **Build Feature Generator**:
    ```bash
    make fg
    ```
2.  **Build All (Including GUI)**:
    ```bash
    make all
    ```
3.  **Clean Build**:
    ```bash
    make clean
    ```

## Usage

### 1. Offline Indexing
Extract features from the image collection and save them to a CSV file.
```bash
./bin/fg <image_directory> <output_csv_path>
```

### 2. Online Retrieval
Run the GUI application to query images visually.
```bash
# GUI execution command (target to be finalized in Makefile)
./bin/CBIR_GUI 
```

## Authors
- Developed for CS5330 Computer Vision.