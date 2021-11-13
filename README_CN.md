# MM32_CLion

&nbsp;&nbsp;&nbsp;&nbsp;![Version](https://img.shields.io/badge/Version-1.0.1-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-AGPL-blue.svg)

&nbsp;&nbsp;&nbsp;&nbsp;这个项目是

***

### 开发环境

+ 集成开发环境：CLion 
+ 操作系统：Windows_NT x64 10.0.19041
+ 仿真器：icarus verilog 11.0

***

### 目录结构

+ core：priRV32内核有关的源码文件，主要包括了取指译码单元和执行写回单元。
+ fpga：基于Quartus Lite Edition集成开发环境的FPGA项目文件。
+ periph：一些简单外设接口的源码文件。
  + uart：UART串口外设的源码文件。
+ tb：内核及外设的测试脚本文件。

***

### 许可证

**组织：AcmeTech <br>
作者：朱彦臻<br>
维护人：朱彦臻, 2208213223@qq.com**

&nbsp;&nbsp;&nbsp;&nbsp;本项目的源代码根据[AGPL-3.0许可证](https://github.com/ZhuYanzhen1/priRV32/blob/main/LICENSE)发布。本项目已经在Windows 10、Ubuntu 18.04和20.04下进行了测试。这是一个学习型代码，希望它经常更改，并且不承认任何特定用途的适用性。