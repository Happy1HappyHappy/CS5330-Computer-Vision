# Content-Based Image Retrieval (CBIR) Project

This project implements a Content-Based Image Retrieval system to search for similar images based on visual features. It supports multiple feature extraction methods and distance metrics, allowing for flexible image matching strategies.

## Authors
Claire Liu, Yu-Jing Wei

## Project Structure

The project is organized as follows:

```text
project2/
├── Makefile                   # Build system
├── data/                      # Data directory
│   └── olympus/               # Image database (example)
├── include/                   # Header files
│   ├── IExtractor.hpp         # Interface for feature extractors
│   ├── IDistanceMetric.hpp    # Interface for distance metrics
│   ├── featureExtractor.hpp   # Concrete feature extractor classes
│   ├── distanceMetrics.hpp    # Concrete distance metric classes
│   ├── extractorFactory.hpp   # Factory for creating extractors
│   ├── metricFactory.hpp      # Factory for creating metrics
│   ├── filters.hpp            # Image filtering utilities
│   ├── faceDetect.hpp         # Face detection utilities
│   ├── csvUtil.hpp            # CSV read/write utilities
│   ├── readFiles.hpp          # File reading utilities
│   ├── matchUtil.hpp          # Matching logic utilities
│   ├── position.hpp           # Region of Interest (ROI) definitions
│   ├── featureGenCLI.hpp      # CLI parser for feature generation
│   └── featureMatcherCLI.hpp  # CLI parser for feature matching
├── src/
│   ├── offline/
│   │   └── featureGenerator.cpp # Main entry point for feature extraction CLI
│   ├── online/
│   │   ├── featureMatcher.cpp   # Main entry point for feature matching CLI
│   │   ├── mainWindow.cpp       # GUI implementation (if applicable)
│   │   └── mainWindow.h         # GUI definition
│   └── utils/
│       ├── featureExtractor.cpp # Implementation of feature extractors
│       ├── distanceMetrics.cpp  # Implementation of distance metrics
│       ├── extractorFactory.cpp # Implementation of extractor factory
│       ├── metricFactory.cpp    # Implementation of metric factory
│       ├── filters.cpp          # Implementation of image filters
│       ├── faceDetect.cpp       # Implementation of face detection
│       ├── csvUtil.cpp          # Implementation of CSV utilities
│       ├── readFiles.cpp        # Implementation of file reading
│       └── matchUtil.cpp        # Implementation of matching utilities
├── resources/                 # Application resources
├── bin/                       # Executables output
└── obj/                       # Compiled objects
```

## Architecture

The project follows a modular architecture with clear separation of concerns, utilizing interfaces and factories for extensibility.

### Interfaces

- **`IExtractor`**: Abstract base class for all feature extractors.
  - `extractMat(const cv::Mat &image, std::vector<float> *out)`: Pure virtual function to extract features from an image.
  - `extract(const char *imagePath, std::vector<float> *out, Position pos)`: Wrapper to load image and extract features from a specific ROI.
- **`IDistanceMetric`**: Abstract base class for distance metrics.
  - `compute(const std::vector<float> &v1, const std::vector<float> &v2)`: Pure virtual function to calculate distance between two feature vectors.

### Classes & Methods

#### Feature Extractors (`src/utils/featureExtractor.cpp`)
- **`BaselineExtractor`**: Extracts a 9x9 feature vector from the center of the image.
- **`RGColorHistExtractor`**: Computes a 2D RG chromaticity histogram.
- **`RGBColorHistExtractor`**: Computes a 3D RGB color histogram.
- **`SobelMagnitudeExtractor`**: Computes a histogram of gradient magnitudes using Sobel filters.
- **`CIELabHistExtractor`**: Computes a histogram in the CIELab color space.
- **`GaborHistExtractor`**: extract features using Gabor filters for texture analysis.

#### Distance Metrics (`src/utils/distanceMetrics.cpp`)
- **`SumSquaredDistance` (SSD)**: Computes the sum of squared differences.
- **`HistogramIntersection`**: Computes 1 minus the intersection of two normalized histograms.
- **`CosDistance`**: Computes the cosine distance between feature vectors.

#### Factories
- **`ExtractorFactory`**: Creates instances of `IExtractor` based on `FeatureType`.
- **`MetricFactory`**: Creates instances of `IDistanceMetric` based on `MetricType`.

#### Utilities
- **`Filters`** (`src/utils/filters.cpp`):
  - `sobelX3x3`, `sobelY3x3`: Computes Sobel gradients.
  - `magnitude`: Computes gradient magnitude.
  - `blur5x5`: Applies Gaussian blur.
- **`FaceDetect`** (`src/utils/faceDetect.cpp`):
  - `detectFaces`: Detects faces using Haar cascades.
  - `drawBoxes`: Draws bounding boxes around detected faces.
- **`CSVUtil`** (`src/utils/csvUtil.cpp`):
  - `saveFeatures`: Appends feature vectors to a CSV file.
  - `readFeatures`: Reads feature vectors from a CSV file.
- **`ReadFiles`** (`src/utils/readFiles.cpp`):
  - `readFilesInDir`: Lists all image files in a directory.
- **`MatchUtil`** (`src/utils/matchUtil.cpp`):
  - `getTopNMatches`: Sorts and retrieves the top N matching images based on distance.

## Prerequisites

- **OpenCV**: Required for image processing (`opencv4`).
- **Qt** (Optional): Required for GUI components.
- **C++17 Compiler**: Required for building the project (`g++` or `clang++`).

## Build Instructions

Use the provided `Makefile` to compile the project:

1.  **Build Feature Generator** (Offline):
    ```bash
    make fg
    ```
2.  **Build Feature Matcher** (Online CLI):
    ```bash
    make matcher
    ```
3.  **Build All**:
    ```bash
    make all
    ```
4.  **Clean Build**:
    ```bash
    make clean
    ```

## Usage

### 1. Offline Feature Extraction (`fg`)

Extract features from a directory of images and save them to a CSV file.

```bash
./bin/fg --input <image_directory> --output <output_csv_path> --feature <type> [options]
```

**Options:**
- `-i, --input <dir>`: Input image directory.
- `-o, --output <csv>`: Output CSV file path.
- `-f, --feature <type>`: Feature type(s) to extract. Can be repeated or comma-separated.
  - Types: `baseline`, `cielab`, `gabor`, `magnitude`, `rghist2d`, `rgbhist3d`.
- `-p, --pos <pos>`: Region of Interest (ROI) (default: `whole`).
  - Values: `whole`, `center`, `up`, `bottom`.
- `-h, --help`: Show help message.

**Example:**
```bash
./bin/fg -i data/olympus -o data/features.csv -f rgbhist3d -p whole
```

### 2. Online Image Matching (`matcher`)

Find similar images to a query image using a database of features.

```bash
./bin/matcher --target <target_image> --db <db_spec> --top <N> [options]
```

**Options:**
- `-t, --target <img>`: Path to the target (query) image.
- `-d, --db <spec>`: Database specification. Can be repeated or comma-separated for multi-feature matching.
  - **Format**: `feature:position:metric:[weight]=db_filename.csv`
  - **Feature**: `baseline`, `cielab`, `gabor`, `magnitude`, `rghist2d`, `rgbhist3d`
  - **Position**: `whole`, `center`, `up`, `bottom`
  - **Metric**: `ssd`, `hist_ix`, `cosine`
  - **Weight**: Optional float value (default: 1.0)
- `-n, --top <N>`: Number of top matches to display.
- `-h, --help`: Show help message.

**Example:**
```bash
./bin/matcher -t data/olympus/pic.1016.jpg -d rgbhist3d:whole:hist_ix=data/features.csv -n 5
```