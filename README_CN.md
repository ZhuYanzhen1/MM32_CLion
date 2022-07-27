# MM32_CLion

![Version](https://img.shields.io/badge/Version-1.6.1-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-AGPL-blue.svg)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[English](https://github.com/ZhuYanzhen1/MM32_CLion/blob/master/README.md) / 中文

&nbsp;&nbsp;&nbsp;&nbsp;这个项目是MM32F3277G9芯片的Windows CLion集成开发环境文件以及基于MM32F3277G9芯片的主控板电路设计。软件代码中包含了GCC所需的链接文件、CMake文件、clang-format文件、Doxygen配置文件等，确保本软件包可以到手即用。硬件主要包含了原理图和PCB文件，Gerber文件会以Releases的形式发布。适配的调参工具使用Qt进行开发，仓库中包含了该工具的Qt源码文件。另外还提供了一些便于开发的Python脚本工具，如计算滤波器参数等脚本。<br>
&nbsp;&nbsp;&nbsp;&nbsp;本项目的开发是围绕着2022年的全国大学生智能车大赛进行的，在本项目中创新地使用了LQR控制器、多速率卡尔曼融合等复杂算法以及单元测试、持续集成等先进开发手段进行开发，极大地提高了开发效率，降低了开发成本。

***

### 开发环境

+ 集成开发环境：CLion CL-221.5921.27
+ 操作系统：Windows 10 专业版 21H2 x64 10.0.19044
+ Make工具：GNU Make 4.3 x86_64-w64-mingw32
+ 编译工具链：gcc-arm-none-eabi 10.3.1 20210824
+ 烧录工具：OpenOCD 0.11.0+dev-00449-gb2ab2241f+ (2021-11-13-20:25)
+ 文档生成工具：Doxygen GUI 1.9.4 (Qt version 5.15.2)
+ EDA工具：KiCAD 6.0.5 (VC++ 1929, 64bit)
+ Qt环境：Qt 5.14.2 (MSVC 2017, 32bit)
+ Python版本：Python 3.10.5 [MSC v.1929 64 bit (AMD64)] on win32

***

### 目录结构

+ .github：用于存放Github Actions持续集成的配置文件。
+ circuit：项目有关的电路原理图及PCB文件。
  + bldc_driver：无刷驱动板的电路原理图及PCB文件。
  + control_board：主控板的电路原理图及PCB文件。
  + geomagnetometer：地磁计板的电路原理图及PCB文件。
  + power_board：电源板的电路原理图及PCB文件。
  + uart_tt_mcu：串口透传模块单片机端的电路原理图及PCB文件。
  + uart_tt_pc：串口透传模块电脑端的电路原理图及PCB文件。
  + wireless_debugger：无线调试器的电路原理图及PCB文件。
+ docs：由Doxygen生成的html文档和相关图片及电路板交互式BOM。
+ mechanical：非标准机械部件的3D模型文件。
  + battery_mount：电池支架的3D模型文件。
  + car_shell：车壳的3D模型文件。
  + control_board_mount：主控板和电源板支架的3D模型文件。
  + magnometer_mount：地磁计支架的3D模型文件。
+ program：主控制器程序源文件及配置文件。
  + ctrl_mcu：控制MCU用户层代码，主要逻辑实现。
  + debug：用于实现串口调参的传输层代码和CMBackTrace程序文件。
  + device：针对器件的数据链路层协议，实现对器件级别的寄存器读写。
  + fatfs：FAT文件系统的有关代码。
  + form：显示屏所有窗体控件和窗体动作。
  + freertos：FreeRTOS实时操作系统的有关代码。
  + gui：显示屏的有关绘图函数和窗体代码实现。
  + hardware：与MM32F3277G9P芯片外设相关的初始化及读写代码。
  + library：由MindMotion提供的MM32F3277G9P芯片HAL库代码。
  + math：与数学计算有关的代码和函数，如校验计算、FFT等。
  + proc_mcu：处理MCU用户层代码，主要逻辑实现。
  + protocol：[sdtp和mdtp](https://github.com/ZhuYanzhen1/CDTP)的数据链路层传输协议。
  + system：与MM32F3277G9P系统有关初始化及操作代码。
+ scripts：用于方便程序测试和设计的Python脚本文件。
+ testbench：用于测试功能的基于CUnit库的单元测试代码。
+ tools：用于调参的Qt调参上位机源码。

***

### 许可证

**组织：AcmeTech <br>
作者：朱彦臻，赵癸林，赖寒冰<br>
维护人：朱彦臻，赵癸林, 2208213223@qq.com，1662568794@qq.com**

&nbsp;&nbsp;&nbsp;&nbsp;本项目的源代码根据[AGPL-3.0许可证](https://github.com/ZhuYanzhen1/MM32_CLion/blob/master/LICENSE)发布。本项目已经在Windows10、Ubuntu 18.04和20.04下进行了测试。这是一个学习型代码，希望它经常更改，并且不承认任何特定用途的适用性。