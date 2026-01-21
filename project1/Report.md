## How cvtColor work in OpenCV? (Task3)
OpenCV provides over 150 color-space coversion methods. We use the RGB -> Gray color coversion code(`cv::COLOR_RGB2GRAY`) in our project. The coversion is based on the formula listed belows. 

Transformations within RGB space like adding/removing the alpha channel, reversing the channel order, conversion to/from 16-bit RGB color (R5:G6:B5 or R5:G5:B5), as well as conversion to/from grayscale using:
$$Y \leftarrow 0.299 \cdot R + 0.587 \cdot G + 0.114 \cdot B$$
and
$$R \leftarrow Y, G \leftarrow Y, B \leftarrow Y, A \leftarrow \max(ChannelRange)$$

The original and `cvtColor::COLOR_RGB2GRAY` version images are listed below:
![Color](Assets/images/screenshot_20260120_165022459.png)
![Grey](Assets/images/screenshot_20260120_165022459_grey.png)