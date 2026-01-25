# OSX compiler
#CC = clang++

# Dwarf compiler
CC = /Applications/Xcode.app/Contents/Developer/usr/bin/g++

CXX = $(CC)

# OSX include paths (for MacPorts)
#CFLAGS = -I/opt/local/include -I../include

# OSX include paths (for homebrew, probably)
CFLAGS = -Wc++11-extensions -std=c++11 -I/opt/homebrew/include/opencv4 -DENABLE_PRECOMPILED_HEADERS=OFF

# Dwarf include paths
#CFLAGS = -I../include # opencv includes are in /usr/include
CXXFLAGS = $(CFLAGS)

# OSX Library paths (if you use MacPorts)
#LDFLAGS = -L/opt/local/lib

#OSX Library paths (if you use homebrew, probably)
#LDFLAGS = -L/usr/local/lib

# Dwarf Library paths

LDFLAGS = -L/opt/homebrew/opt/opencv/lib

# opencv libraries
LDLIBS = \
	-lopencv_core \
	-lopencv_highgui \
	-lopencv_imgcodecs \
	-lopencv_imgproc \
	-lopencv_videoio


BINDIR = ./bin
SRCDIR = ./src
OBJDIR = ./obj

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

imgdisplay: $(OBJDIR)/imgDisplay.o
	mkdir -p $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf obj/*/*.o bin/* *~ 
