## How cvtColor work in OpenCV? (Task3)
OpenCV provides over 150 color-space coversion methods. We use the RGB -> Gray color coversion code(`cv::COLOR_RGB2GRAY`) in our project. The coversion is based on the formula listed belows. 

Transformations within RGB space like adding/removing the alpha channel, reversing the channel order, conversion to/from 16-bit RGB color (R5:G6:B5 or R5:G5:B5), as well as conversion to/from grayscale using:
$$Y \leftarrow 0.299 \cdot R + 0.587 \cdot G + 0.114 \cdot B$$
and
$$R \leftarrow Y, G \leftarrow Y, B \leftarrow Y, A \leftarrow \max(ChannelRange)$$

The original and `cvtColor::COLOR_RGB2GRAY` version images are listed below:
**Color:**
![Color](Assets/images/screenshot_20260120_165022459.png)
**Grey:**
![Grey](Assets/images/screenshot_20260120_165022459_grey.png)

## Alternative grayscale filter (Task4)
We use weighted RGB mean for the filter. The weight is designed to mimic the true tonal perception of the human visual system. The coverison is listed below:
$$Y \leftarrow  0.32  \cdot R + 0.64 \cdot G + 0.04 \cdot B$$

**Color:**
![Color](Assets/images/screenshot_20260120_212559895.png)
**Our gray filter:**
![OurGray](Assets/images/screenshot_20260120_212559895_OurGrey.png)
**CV's gray filter:**
![CVGray](Assets/images/screenshot_20260120_212559895_grey.png)

## Sepia filter (Task5)
We declare two diffrent variable to store RGB information. One set of variable store orginal RGB information and the other store the RGB information after coversion.

**Original:**
![Color](Assets/images/screenshot_20260120_220456250c.png)

**Apply Sepia filter:**
![Sepia](Assets/images/screenshot_20260120_220500992e.png)

## 5x5 blur filter (Task6)
**Original:**
![original](Assets/images/original.png)

**Apply 5x5 blur:**
![5x5](Assets/images/blurred1.png)

**Timing information:**
![time](<Assets/images/Screenshot 2026-01-22 at 10.16.34.png>)