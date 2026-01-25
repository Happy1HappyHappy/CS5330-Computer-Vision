# MacOS compiler
CC = /Applications/Xcode.app/Contents/Developer/usr/bin/g++

# Make it the C++ compiler too
CXX = $(CC)

# OSX include paths for MacPorts
CFLAGS = -std=c++17 \
         -I/opt/homebrew/include/opencv4 \
         -I/opt/homebrew/include \
         -I/opt/homebrew/opt/onnxruntime/include/onnxruntime \
         -DENABLE_PRECOMPILED_HEADERS=OFF

# Make these the C++ flags too
CXXFLAGS = $(CFLAGS)

# Library paths, update to wwhere your openCV libraries are stored
# these settings work for macports
LDFLAGS = -L/opt/homebrew/lib \
          -L/opt/homebrew/opt/opencv/lib \
          -L/opt/homebrew/opt/onnxruntime/lib


# opencv libraries and MacOS frameworks
# these settings work for macOS and macports
LDLIBS = -ltiff -lpng -ljpeg -llapack -lblas -lz -lwebp \
         -framework AVFoundation \
         -framework CoreMedia \
         -framework CoreVideo \
         -framework CoreServices \
         -framework CoreGraphics \
         -framework AppKit \
         -framework OpenCL \
         -lopencv_core \
         -lopencv_highgui \
         -lopencv_video \
         -lopencv_videoio \
         -lopencv_imgcodecs \
         -lopencv_imgproc \
         -lopencv_objdetect \
         -lonnxruntime

# Location to write the executables
BINDIR = .

# rules for making various executables
mnist: mnist-cv.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

depth: depthanything.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)
	install_name_tool -add_rpath /opt/local/lib ./$@

da2: da2-example.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)
	install_name_tool -add_rpath /opt/local/lib ./$@

da2vid: da2-video.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)
	install_name_tool -add_rpath /opt/local/lib ./$@

clean:
	rm -f *.o *~ 
