# priRV32

&nbsp;&nbsp;&nbsp;&nbsp;![Version](https://img.shields.io/badge/Version-1.0.1-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-AGPL-blue.svg)

&nbsp;&nbsp;&nbsp;&nbsp;priRV32 is a kernel that can implement the RV32I sub-instruction set architecture and RV machine mode. This kernel is currently under development. The naming of modules and variables in this kernel code refers to the naming method in picoRV32, and there are many similarities in the naming of variables. In addition, in order to reduce the development time, the code of the peripheral part was directly packaged and used from the code provided by the Alinx FPGA development board. This kernel is for learning purposes only, and I am not responsible for any use in other scenarios.

***

### Preface

&nbsp;&nbsp;&nbsp;&nbsp;In the first half of the semester, I was studying at home due to the epidemic, and I saw RISC-V-related books sold on Taobao. Since then, the idea of ​​"writing RISC-V processors from scratch" has continued to appear in my mind. At that time, I didn't understand the basic knowledge of computers (I only read the authoritative guide for the ARM Cortex-M3 core). Sophomore year, I have been supplementing computer-related knowledge, including computer system structure, computer composition principle, data structure, Linux kernel and so on. Now that the junior year transfers to integrated circuits, there is a course of computer system structure, so I want to take this opportunity to implement a simple RV kernel. <br>
&nbsp;&nbsp;&nbsp;&nbsp;Since this is the first RV32 core I designed, considering my low level, this core only intends to implement the RV32I instruction set architecture, the core frequency is 36MHz, and it is against the current 8051 and ARM Cortex-M0 cores. In terms of combined functions, the kernel is named priRV32, which means "primary RV32 kernel". It is the first time to write a relatively large-scale project, and there are many immature and shortcomings in the language specification and implementation, and I invite developers Haihan.

***

### Develop Environment

+ IDE: Visual Studio Code 1.62.1 &  Quartus Prime 20.1 Lite Edition
+ OS: Windows_NT x64 10.0.19041
+ Simulator: icarus verilog 11.0

***

### Directory Structure

+ core: The source code files related to the priRV32 kernel, which mainly include the instruction fetching and decoding unit and the execution write-back unit.
+ fpga: FPGA project file based on Quartus Lite Edition integrated development environment.
+ periph: Some source files of simple peripheral interfaces.
   + uart: source file of UART serial peripherals.
+ tb: Test script files for the kernel and peripherals.

***

### License

**Affiliation：AcmeTech<br>
Author: Zhu Yanzhen<br>
Maintainer：YanzhenZhu, 2208213223@qq.com**

&nbsp;&nbsp;&nbsp;&nbsp;The source code is released under a [AGPL-3.0 License](https://github.com/ZhuYanzhen1/priRV32/blob/main/LICENSE).The product has been tested under Windows 10 and Ubuntu 18.04 and 20.04. This is research code, expect that it changes often and any fitness for a particular purpose is disclaimed.
