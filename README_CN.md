# MM32_CLion

![Version](https://img.shields.io/badge/Version-1.0.1-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-AGPL-blue.svg)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[English](https://github.com/ZhuYanzhen1/MM32_CLion/blob/master/README.md) / 中文

&nbsp;&nbsp;&nbsp;&nbsp;这个项目是MM32F3277G9芯片的Windows CLion集成开发环境文件以及基于MM32F3277G9芯片的主控板电路设计。软件代码中包含了GCC所需的链接文件、CMake文件、clang-format文件、Doxygen配置文件等，确保本软件包可以到手即用。硬件主要包含了原理图和PCB文件，Gerber文件会以Releases的形式发布。

***

### 开发环境

+ 集成开发环境：CLion CL-212.5457.51
+ 操作系统：Windows_NT x64 10.0.19041
+ 编译工具链：gcc-arm-none-eabi 10.3.1 20210824
+ 烧录工具：OpenOCD 0.11.0+dev-00449-gb2ab2241f-dirty
+ 文档生成工具：Doxygen GUI 1.9.2
+ EDA工具：KiCAD 5.1.10-1

***

### 目录结构

+ circuit：基于MM32F3277G9P芯片设计的开发板电路原理图及PCB。
+ docs：由Doxygen生成的html文档及相关图片。
+ program：程序源文件及配置文件。
  + hardware：与MM32F3277G9P芯片外设相关的初始化及读写代码。
  + library：由MindMotion提供的MM32F3277G9P芯片HAL库代码。
  + system：与MM32F3277G9P系统有关初始化及操作代码。
  + user：用户层代码，主要逻辑实现。
  + math：与数学计算有关的代码和函数，如校验计算、FFT等。
  + debug：用于实现串口调参的数据链路层和传输层代码。
  + device：针对器件的数据链路层协议，实现对器件级别的寄存器读写。

***

### 许可证

**组织：AcmeTech <br>
作者：朱彦臻<br>
维护人：朱彦臻, 2208213223@qq.com**

&nbsp;&nbsp;&nbsp;&nbsp;本项目的源代码根据[AGPL-3.0许可证](https://github.com/ZhuYanzhen1/MM32_CLion/blob/master/LICENSE)发布。本项目已经在Windows10、Ubuntu 18.04和20.04下进行了测试。这是一个学习型代码，希望它经常更改，并且不承认任何特定用途的适用性。