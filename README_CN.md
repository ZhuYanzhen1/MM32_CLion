# priRV32

&nbsp;&nbsp;&nbsp;&nbsp;![Version](https://img.shields.io/badge/Version-1.0.1-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-AGPL-blue.svg)

&nbsp;&nbsp;&nbsp;&nbsp;priRV32是一个能够实现RV32I子指令集架构和RV机器模式的内核，这个内核目前还在开发过程中。此内核代码中模块和变量的命名参考了picoRV32中的命名方法，在很多变量命名上有相同之处。另外为了减少开发时间，外设部分的代码我是直接从黑金FPGA开发板提供的代码上封装改过来使用的。此内核仅作为学习用途，任何用于其它场景的本人概不负责。

***

### 前言

&nbsp;&nbsp;&nbsp;&nbsp;大一下半学期由于疫情的原因在家学习，看到在淘宝上有RISC-V相关的书籍卖。从那之后一个“从零开始写RISC-V处理器”的想法开始不断地出现在我的脑海里。当时由于没有接触过计算机底层的知识（只看过ARM Cortex-M3内核权威指南），所以没怎么看懂。大二一整年都在补计算机相关的知识，有计算机系统结构、计算机组成原理、数据结构、Linux内核等等。现在大三转到集成电路后，开设有计算机系统结构的课程，所以借此机会想把一个简单的RV内核实现出来。<br>
&nbsp;&nbsp;&nbsp;&nbsp;由于这是我设计的第一个RV32内核，考虑到自己水平很低，所以这个内核只打算实现RV32I指令集架构，内核主频为36MHz，对标当前的8051及ARM Cortex-M0内核。结合功能来看，给这个内核起名为priRV32，寓意为“初级的RV32内核”。第一次写较为大型的项目，在语言规范以及实现上面有许多稚嫩和不足之处还请广大开发者海涵。

***

### 开发环境

+ 集成开发环境：Visual Studio Code 1.62.1 &  Quartus Prime 20.1 Lite Edition
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