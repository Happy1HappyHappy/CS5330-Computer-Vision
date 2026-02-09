# OSX compiler
#CC = clang++

# Dwarf compiler
CC = /Applications/Xcode.app/Contents/Developer/usr/bin/g++

CXX = $(CC)

# OSX include paths (for MacPorts)
#CFLAGS = -I/opt/local/include -I../include

# OSX include paths (for homebrew, probably)
CFLAGS = -Wc++17-extensions \
	-std=c++17 \
	-I/opt/homebrew/include/opencv4 \
	-I./include \
	-DENABLE_PRECOMPILED_HEADERS=OFF

# Dwarf include paths
#CFLAGS = -I../include # opencv includes are in /usr/include
CXXFLAGS = $(CFLAGS)

# OSX Library paths (if you use MacPorts)
#LDFLAGS = -L/opt/local/lib

#OSX Library paths (if you use homebrew, probably)
#LDFLAGS = -L/usr/local/lib

# Dwarf Library paths

LDFLAGS = -L/opt/homebrew/opt/opencv/lib
# LDFLAGS = -L/opt/local/lib/opencv4/3rdparty -L/opt/local/lib # opencv libraries are here

# opencv libraries
# LDLIBS = -ltiff -lpng -ljpeg -llapack -lblas -lz -ljasper -lwebp -lIlmImf -lgs -framework AVFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -framework CoreGraphics -framework AppKit -framework OpenCL  -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect
LDLIBS = \
	-lopencv_core \
	-lopencv_highgui \
	-lopencv_imgcodecs \
	-lopencv_imgproc \
	-lopencv_videoio \
	-lopencv_objdetect


BINDIR = ./bin
SRCDIR = ./src
OBJDIR = ./obj
UTILSDIR = ./src/utils


# Prerequisites / Dependencies
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/offline/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/online/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(UTILSDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Targets
# Targets
all: fg matcher gui

gui:
	qmake project2_gui.pro -o Makefile.gui
	$(MAKE) -f Makefile.gui


COMMON_OBJS = $(OBJDIR)/csvUtil.o \
              $(OBJDIR)/extractorFactory.o \
			  ${OBJDIR}/faceDetect.o \
              $(OBJDIR)/featureExtractor.o \
              $(OBJDIR)/featureGenCLI.o \
			  ${OBJDIR}/filters.o \
              $(OBJDIR)/readFiles.o \

fg: $(OBJDIR)/featureGenerator.o $(COMMON_OBJS)
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

matcher: $(OBJDIR)/distanceMetrics.o \
		 $(OBJDIR)/featureMatcher.o \
		 ${OBJDIR}/featureMatcherCLI.o \
         $(OBJDIR)/metricFactory.o \
         $(OBJDIR)/matchUtil.o \
         $(COMMON_OBJS)
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

# defaults (can be overridden)
N ?= 3
I ?= data/olympus
O ?= data/fv.csv
P ?= whole

baseline: T ?= data/olympus/pic.1016.jpg
baseline:
	./bin/fg -i $(I) \
			-f baseline \
			-o $(O)
	./bin/matcher \
			-t $(T) \
			-d baseline:$(P):ssd=data/fv_baseline_whole.csv \
			-n $(N)
rghist: T ?= data/olympus/pic.0164.jpg
rghist:
	./bin/fg -i $(I) \
			-f rghist2d \
			-o $(O)
	./bin/matcher \
			-t $(T) \
			-d rghist2d:$(P):hist_ix=data/fv_rghist2d_whole.csv \
			-n $(N)
rgbhist: T ?= data/olympus/pic.0164.jpg
rgbhist:
	./bin/fg -i $(I) \
			-f rgbhist3d \
			-o $(O)
	./bin/matcher \
			-t $(T) \
			-d rgbhist3d:$(P):hist_ix=data/fv_rgbhist3d_whole.csv \
			-n $(N)
			
multihist: T ?= data/olympus/pic.0274.jpg
multihist: W1 ?= 1
multihist: W2 ?= 1
multihist:
	./bin/fg -i $(I) \
			-f rgbhist3d \
			-o $(O) \
			-p up
	./bin/fg -i $(I) \
			-f rgbhist3d \
			-o $(O) \
			-p bottom
	./bin/matcher \
			-t $(T) \
			-d rgbhist3d:up:hist_ix:$(W1)=data/fv_rgbhist3d_up.csv \
			-d rgbhist3d:bottom:hist_ix:$(W2)=data/fv_rgbhist3d_bottom.csv \
			-n $(N)

mulit_center_focus: T ?= data/olympus/pic.1051.jpg
mulit_center_focus: W1 ?= 1
mulit_center_focus: W2 ?= 1
mulit_center_focus: W3 ?= 1
mulit_center_focus:
	./bin/fg -i $(I) \
			-f rghist2d \
			-o $(O) \
			-p center
	./bin/fg -i $(I) \
			-f rgbhist3d \
			-o $(O) \
			-p whole
	./bin/fg -i $(I) \
			-f cielab \
			-o $(O) \
			-p center
	./bin/matcher \
			-t $(T) \
			-d rghist2d:center:hist_ix:$(W1)=data/fv_rghist2d_center.csv \
			-d rgbhist3d:whole:hist_ix:$(W2)=data/fv_rgbhist3d_whole.csv \
			-d cielab:center:hist_ix:$(W3)=data/fv_cielab_center.csv \
			-n $(N)

magnitude: T ?= data/olympus/pic.0164.jpg
magnitude:
	./bin/fg -i $(I) \
			-f magnitude \
			-o $(O)
	./bin/matcher \
			-t $(T) \
			-d magnitude:$(P):ssd=data/fv_magnitude_whole.csv \
			-n $(N)

dnn: T ?= data/olympus/pic.0164.jpg
dnn: M ?= ssd
dnn:
	./bin/matcher \
			-t $(T) \
			-d baseline:$(P):$(M)=data/ResNet18_olym.csv \
			-n $(N)

cie_gabor: T ?= data/olympus/pic.0535.jpg
cie_gabor: W1 ?= 1
cie_gabor: W2 ?= 1
cie_gabor:
	./bin/fg -i $(I) \
			-f cielab,gabor \
			-o $(O)
	./bin/matcher \
			-t $(T) \
			-d cielab:$(P):hist_ix:$(W1)=data/fv_cielab_whole.csv \
			-d gabor:$(P):cosine:$(W2)=data/fv_gabor_whole.csv \
			-n $(N)

people: T ?= data/olympus/pic.0842.jpg
people:
	./bin/fg -i $(I) \
			-f rghist2d \
			-o $(O) \
			-p center
	./bin/fg -i $(I) \
			-f rgbhist3d \
			-o $(O) \
			-p center
	./bin/fg -i $(I) \
			-f cielab \
			-o $(O) \
			-p center
	./bin/fg -i $(I) \
			-f magnitude \
			-o $(O) \
			-p center
	./bin/fg -i $(I) \
			-f gabor \
			-o $(O) \
			-p center
	./bin/matcher \
			-t $(T) \
			-d rghist2d:center:hist_ix=data/fv_rghist2d_center.csv \
			-d rgbhist3d:center:hist_ix=data/fv_rgbhist3d_center.csv \
			-d cielab:center:hist_ix:3=data/fv_cielab_center.csv \
			-d magnitude:center:cosine:10=data/fv_magnitude_center.csv \
			-d gabor:center:cosine:5=data/fv_gabor_center.csv \
			-n $(N)

clean:
	rm -rf obj/*.o bin/* *~ 
	rm -f Makefile.gui .qmake.stash
	rm -f *.o moc_*.cpp moc_*.h moc_predefs.h
