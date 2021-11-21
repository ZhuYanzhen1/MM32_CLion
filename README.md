# MM32_CLion

![Version](https://img.shields.io/badge/Version-1.0.1-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-AGPL-blue.svg)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;English / [中文](https://github.com/ZhuYanzhen1/MM32_CLion/blob/master/README_CN.md)

&nbsp;&nbsp;&nbsp;&nbsp;This project is a Windows CLion IDE file for the MM32F3277G9 chip and a circuit design for the main control board based on the MM32F3277G9 chip. The software code contains the link files required for GCC, CMake files, clang-format files, Doxygen configuration files, etc. to ensure that this package is ready to use when it arrives. The hardware mainly contains the schematic and PCB files, and the Gerber files will be released in the form of Releases.

***

### Develop Environment

+ Integrated Development Environment: CLion CL-212.5457.51
+ Operating System: Windows_NT x64 10.0.19041
+ Compilation Tool Chain: gcc-arm-none-eabi 10.3.1 20210824
+ Burning Tool: OpenOCD 0.11.0+dev-00449-gb2ab2241f-dirty
+ Document Genetare Tool: Doxygen GUI 1.9.2
+ EDA Tool: KiCAD 5.1.10-1

***

### Directory Structure

+ circuit: The circuit schematic and PCB of the development board designed based on the MM32F3277G9P chip.
+ docs: HTML documents and related pictures generated by Doxygen.
+ program: Program source files and configuration files.
  + hardware: initialization and reading and writing codes related to the peripherals of the MM32F3277G9P chip.
  + library: MM32F3277G9P chip HAL library code provided by MindMotion.
  + system: initialization and operation codes related to the MM32F3277G9P system.
  + user: user layer code, main logic implementation.
  + math: Codes and functions related to mathematical calculations, such as checksum calculations, FFT, etc.

***

### License

**Affiliation：AcmeTech<br>
Author: Zhu Yanzhen<br>
Maintainer：YanzhenZhu, 2208213223@qq.com**

&nbsp;&nbsp;&nbsp;&nbsp;The source code is released under a [AGPL-3.0 License](https://github.com/ZhuYanzhen1/MM32_CLion/blob/master/LICENSE).The product has been tested under Windows 10 and Ubuntu 18.04 and 20.04. This is research code, expect that it changes often and any fitness for a particular purpose is disclaimed.
