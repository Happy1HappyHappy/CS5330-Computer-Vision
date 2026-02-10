QT += core gui widgets

TARGET = gui
TEMPLATE = app

# Output directory for the binary
DESTDIR = bin

CONFIG += c++17
QMAKE_MACOSX_DEPLOYMENT_TARGET = 26.2

# Build artifact directories
OBJECTS_DIR = obj/gui
MOC_DIR = obj/gui/moc
RCC_DIR = obj/gui/rcc
UI_DIR = obj/gui/ui

# Source files
SOURCES += src/online/main.cpp \
           src/online/mainWindow.cpp

# Header files
HEADERS += src/online/mainWindow.h

# Include paths
INCLUDEPATH += include \
               /opt/homebrew/include/opencv4

# OpenCV linking (adjust paths if necessary)
LIBS += -L/opt/homebrew/opt/opencv/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_objdetect
