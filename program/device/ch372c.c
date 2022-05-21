//
// Created by LaoZhu on 2022/5/4.
//

#include "ch372c.h"
#include "hal_conf.h"
#include "mm32_device.h"
#include "delay.h"
#include "gpio.h"
#include "../ctrl_mcu/pin.h"

void CH375_WR_CMD_PORT(unsigned char cmd) {                 /* 向CH375的命令端口写入命令,周期不小于4uS,如果单片机较快则延时 */
    ch372c_gpio_output();
    for (unsigned char counter = 0; counter < 8; ++counter)
        GPIO_WriteBit(GPIOF, 1 << (3 + counter), (cmd & (1 << counter)) >> counter);
    GPIO_SetBits(CH372_A0_PORT, CH372_A0_PIN);
    GPIO_SetBits(CH372_RD_PORT, CH372_RD_PIN);
    GPIO_ResetBits(CH372_WR_PORT, CH372_WR_PIN);
    delayus(1);
    GPIO_SetBits(CH372_WR_PORT, CH372_WR_PIN);
    GPIO_SetBits(GPIOF, 0x07f8);
    delayus(6);
}

void CH375_WR_DAT_PORT(unsigned char dat) {                 /* 向CH375的数据端口写入数据,周期不小于1.5uS,如果单片机较快则延时 */
    ch372c_gpio_output();
    for (unsigned char counter = 0; counter < 8; ++counter)
        GPIO_WriteBit(GPIOF, (1 << (3 + counter)), (dat & (1 << counter)) >> counter);
    GPIO_ResetBits(CH372_A0_PORT, CH372_A0_PIN);
    GPIO_SetBits(CH372_RD_PORT, CH372_RD_PIN);
    GPIO_ResetBits(CH372_WR_PORT, CH372_WR_PIN);
    delayus(1);
    GPIO_SetBits(CH372_WR_PORT, CH372_WR_PIN);
    GPIO_SetBits(CH372_A0_PORT, CH372_A0_PIN);
    GPIO_SetBits(GPIOF, 0x07f8);
    delayus(4);
}

unsigned char CH375_RD_DAT_PORT(void) {                        /* 从CH375的数据端口读出数据,周期不小于1.5uS,如果单片机较快则延时 */
    unsigned char result;
    ch372c_gpio_input();

    GPIO_SetBits(GPIOF, 0x07f8);

    GPIO_ResetBits(CH372_A0_PORT, CH372_A0_PIN);
    GPIO_SetBits(CH372_WR_PORT, CH372_WR_PIN);
    GPIO_ResetBits(CH372_RD_PORT, CH372_RD_PIN);
    delayus(1);
    result = (GPIOF->IDR & 0x07f8) >> 3;
    GPIO_SetBits(CH372_RD_PORT, CH372_RD_PIN);
    GPIO_SetBits(CH372_A0_PORT, CH372_A0_PIN);
    delayus(4);
    return result;
}

void ch372_config(void) {
    static volatile unsigned char i = 0, uid_ver = 0;
    ch372c_gpio_config();
    /* 设置USB工作模式, 必要操作 */
    GPIO_ResetBits(CH372_CS_PORT, CH372_CS_PIN);
    delayms(1);
    CH375_WR_CMD_PORT(CMD_RESET_ALL);
    delayms(100);

    CH375_WR_CMD_PORT(CMD_GET_IC_VER);
    uid_ver = CH375_RD_DAT_PORT();

    CH375_WR_CMD_PORT(CMD_SET_USB_MODE);
    CH375_WR_DAT_PORT(1);                                    /* 设置为使用内置固件的USB设备方式 */
    delayus(20);
    for (i = 0xff; i != 0; i--)                /* 等待操作成功,通常需要等待10uS-20uS */
    {
        delayus(5);
        if (CH375_RD_DAT_PORT() == CMD_RET_SUCCESS)
            break;
    }
    (void) uid_ver;
    while (1);
}

//void ch372_interrupt_callback(void) {
//    unsigned char InterruptStatus;
//    unsigned char length, len;
//    CH375_WR_CMD_PORT(CMD_GET_STATUS);                                    /* 获取中断状态并取消中断请求 */
//    InterruptStatus = CH375_RD_DAT_PORT();                                    /* 获取中断状态 */
//    /* 清中断标志,对应于INT0中断 */
//    switch (InterruptStatus) {  // 分析中断状态
//        case USB_INT_EP2_OUT:                                            // 批量端点下传成功
//            if (CH375BULKDOWN)
//                mCH375BulkDownData();                                    //如果上传数据阶段则调用数据上传
//            else {                                            //不是数据下传则判断是否
//                CH375_WR_CMD_PORT(CMD_RD_USB_DATA);                                    //发出读数据命令
//                length = CH375_RD_DAT_PORT();
//                if (!length)break;                                //数据包长度为零则跳出
//                //首先读出的是长度
//                for (len = 0; len != length; len++)
//                    MassPara.buf[len] = CH375_RD_DAT_PORT();    //将数据读入到缓冲区
//                mCH375BulkOnly();
//
//                if (!CH375BULKDOWN) {
//                    if (CH375BULKUP)
//                        CH375bulkUpData();                    //调用批量数据上传
//                    else if (!FSTALL)
//                        mCH375UpCsw();                                //没有数据上传调用CSW上传
//                    //在这里做上传数据调用
//                }
//            }
//            break;
//        case USB_INT_EP2_IN:
//            if (CH375BULKUP)CH375bulkUpData();                                //调用数据上传
//            else if (CH375CSW)mCH375UpCsw();                                    //上传CSW
//            CH375_WR_CMD_PORT(CMD_UNLOCK_USB);                                 //批量端点上传成功,未处理
//            break;
//        case USB_INT_EP1_IN:                                                //中断端点上传成功，未处理
//            CH375_WR_CMD_PORT(CMD_UNLOCK_USB);  //释放缓冲区
//            break;
//        case USB_INT_EP1_OUT:                                                //中断端点下传成功，未处理
//            CH375_WR_CMD_PORT(CMD_UNLOCK_USB);  //释放缓冲区
//            break;
//        case USB_INT_EP0_SETUP:                                            //控制端点建立成功
//            CH375FLAGERR = 0;
//            CH375_WR_CMD_PORT(CMD_RD_USB_DATA);
//            length = CH375_RD_DAT_PORT();
//            for (len = 0; len != length; len++)request.buffer[len] = CH375_RD_DAT_PORT();  // 取出数据
//            if (length == 0x08) {
//                mVarSetupLength = request.buffer[6] & 0x7f;                            //控制传输数据长度最大设置为128
//                if ((request.r.bmReuestType) & 0x40) {                             //厂商请求，未处理
//                }
//                if ((request.r.bmReuestType) & 0x20) {                            //类请求，未处理
//                    if (request.buffer[1] == 0xfe)request.buffer[0] = 0;                //类请求得到逻辑盘数目，这里只有一个盘所以
//                    //	else if(request.buffer[1]==0x00);								//复位逻辑单元，这里未处理
//                }
//                if (!((request.r.bmReuestType) & 0x60)) {                        //标准请求
//                    mVarSetupRequest = request.r.bRequest;                            //暂存标准请求码
//                    switch (request.r.bRequest) {  // 分析标准请求
//                        case DEF_USB_CLR_FEATURE:                                    //清除特性
//                            if ((request.r.bmReuestType & 0x1F) == 0X02) {                    //不是端点不支持
//                                switch (request.buffer[4]) {
//                                    case 0x82:CH375_WR_CMD_PORT(CMD_SET_ENDP7);                    //清除端点2上传
//                                        CH375_WR_DAT_PORT(0x8E);                        //发命令清除端点
//                                        if (CH375CSW)mCH375UpCsw();
//                                        lastFSTALL = FSTALL;
//                                        FSTALL = 0;
//                                        break;
//                                    case 0x02:CH375_WR_CMD_PORT(CMD_SET_ENDP6);
//                                        CH375_WR_DAT_PORT(0x80);                //清除端点2下传
//                                        if (CH375CSW)mCH375UpCsw();
//                                        lastFSTALL = FSTALL;
//                                        FSTALL = 0;
//                                        break;
//                                    case 0x81:CH375_WR_CMD_PORT(CMD_SET_ENDP5);                    //清除端点1上传
//                                        CH375_WR_DAT_PORT(0x8E);
//                                        break;
//                                    case 0x01:CH375_WR_CMD_PORT(CMD_SET_ENDP4);                    //清除端点1下传
//                                        CH375_WR_DAT_PORT(0x80);
//                                        break;
//                                    default:break;
//                                }
//                            } else {
//                                CH375FLAGERR = 1;                                //不支持的清除特性，置错误标志
//                            }
//                            break;
//                        case DEF_USB_GET_STATUS:                                //获得状态
//                            request.buffer[0] = 0;
//                            request.buffer[1] = 0;                                //上传状态
//                            break;
//                        case DEF_USB_SET_ADDRESS:                                //设置地址
//                            VarUsbAddress = request.buffer[2];                    //暂存USB主机发来的地址
//                            break;
//                        case DEF_USB_GET_DESCR:                                //获得描述符
//                            if (request.buffer[3] == 1)                            //设备描述符上传
//                                VarSetupDescr = DevDes;
//                            else if (request.buffer[3] == 2)                            //配置描述符上传
//                                VarSetupDescr = ConDes;
//                            else if (request.buffer[3] == 3) {
//                                if (request.buffer[2] == 0) VarSetupDescr = LangDes;
//                                else VarSetupDescr = SerDes;                        //做字符串处理
//                            }
//                            mCh375DesUp();                                            //其余描述符不支持
//                            break;
//                        case DEF_USB_GET_CONFIG:                                    //获得配置
//                            request.buffer[0] = 0;                                    //没有配置则传0
//                            if (CH375CONFLAG)
//                                request.buffer[0] = 1;                                    //已经配置则传1；这是在描述符里规定的
//                            break;
//                        case DEF_USB_SET_CONFIG:                                    //设置配置
//                            CH375CONFLAG = 0;
//                            CH375ACT = 1;
//                            if (request.buffer[2] != 0) {
//                                CH375CONFLAG = 1;                                            //设置配置标志
//                                CH375ACT = 0;                                                //输出配置完成信号
//                            }
//                            break;
//                        case DEF_USB_GET_INTERF:                                        //得到接口
//                            request.buffer[0] = 1;                                    //上传接口数，本事例只支持一个接口
//                            break;
//                        case DEF_USB_SET_INTERF:                                        //设置接口
//                            break;
//                        default :break;                                                    //不支持的标准请求
//                    }
//                }
//            } else {  //不支持的控制传输，不是8字节的控制传输
//                CH375FLAGERR = 1;
//            }
//            if (!CH375FLAGERR) mCh375Ep0Up();                                        //没有错误/调用数据上传，，长度为0上传为状态
//            else {
//                CH375_WR_CMD_PORT(CMD_SET_ENDP3);                                //设置端点1为STALL，指示一个错误
//                CH375_WR_DAT_PORT(0x0F);
//            }
//            CH375_WR_CMD_PORT(CMD_UNLOCK_USB);
//            break;
//        case USB_INT_EP0_IN:CH375_WR_CMD_PORT(CMD_UNLOCK_USB);                                                //控制端点上串成功
//            if (mVarSetupRequest == DEF_USB_GET_DESCR) {                                //描述符上传
//                mCh375DesUp();
//                mCh375Ep0Up();
//            } else if (mVarSetupRequest == DEF_USB_SET_ADDRESS) {                            //设置地址
//                CH375_WR_CMD_PORT(CMD_SET_USB_ADDR);
//                CH375_WR_DAT_PORT(VarUsbAddress);                                //设置USB地址,设置下次事务的USB地址
//            }
//            break;
//        case USB_INT_EP0_OUT:                                                    //控制端点下传成功
//            CH375_WR_CMD_PORT(CMD_UNLOCK_USB);  //释放缓冲区
//            break;
//        default:
//            if ((InterruptStatus & 0x03) == 0x03) {                                    //总线复位
//                FSTALL = 0;
//                CH375FLAGERR = 0;                                                    //错误清0
//                CH375CONFLAG = 0;                                                    //配置清0
//                mVarSetupLength = 0;
//                CH375FLAGERR = 0;                                                //错误清0
//                CH375BULKUP = 0;                                                    //
//                CH375BULKDOWN = 0;
//                CH375CSW = 0;
//                FSTALL = 0;
//                CH375ACT = 1;                                                        //清配置完成输出
//            } else {                                                                //命令不支持
//                ;
//            }
//            CH375_WR_CMD_PORT(CMD_UNLOCK_USB);  //释放缓冲区
//            CH375_RD_DAT_PORT();
//            break;
//    }
//}
