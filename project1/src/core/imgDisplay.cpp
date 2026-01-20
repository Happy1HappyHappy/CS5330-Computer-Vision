#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    // Initialize application
    App app;

    // Load image
    Image img = app.loadImage("example.png");

    // Display image
    app.displayImage(img);

    return 0;
}