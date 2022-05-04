//
// Created by LaoZhu on 2022/5/4.
//

#ifndef MAIN_C_DEVICE_CH372C_H_
#define MAIN_C_DEVICE_CH372C_H_

/* ********************************************************************************************************************* */
/* 硬件特性 */

#define    CH375_MAX_DATA_LEN    0x40            /* 最大数据包的长度, 缓冲区的长度 */

/* ********************************************************************************************************************* */
/* 命令代码 */

#define    CMD_GET_IC_VER        0x01            /* 获取芯片及固件版本 */
/* 输出: 版本号( 位7为1, 位6为0, 位5~位0为版本号 ) */
/*           CH375B返回版本号的值为0B7H即版本号为37H */

#define    CMD_ENTER_SLEEP        0x03            /* 进入睡眠状态 */

#define    CMD_SET_USB_SPEED    0x04            /* 设置USB总线速度, 在每次CMD_SET_USB_MODE设置USB工作模式时会自动恢复到12Mbps全速 */
/* 输入: 总线速度代码 */
/*           00H=12Mbps全速FullSpeed（默认值）, 01H=1.5Mbps（仅修改频率）, 02H=1.5Mbps低速LowSpeed */
#define    CMD_SET_SYS_FREQ    CMD_SET_USB_SPEED

#define    CMD_RESET_ALL        0x05            /* 执行硬件复位 */

#define    CMD_CHECK_EXIST        0x06            /* 测试工作状态 */
/* 输入: 任意数据 */
/* 输出: 输入数据的按位取反 */

#define    CMD_GET_TOGGLE        0x0A            /* 获取OUT事务的同步状态 */
/* 输入: 数据1AH */
/* 输出: 同步状态 */
/*           位4为1则OUT事务同步, 否则OUT事务不同步 */

#define    CMD_CHK_SUSPEND        0x0B            /* 设备方式: 设置检查USB总线挂起状态的方式 */
/* 输入: 数据10H, 检查方式 */
/*                    00H=不检查USB挂起, 04H=以50mS为间隔检查USB挂起, 05H=以10mS为间隔检查USB挂起 */

#define    CMD_DELAY_100US        0x0F            /* 并口方式: 延时100uS */
/* 输出: 延时期间输出0, 延时结束输出非0 */

#define    CMD_SET_USB_ID        0x12            /* 设备方式: 设置USB厂商VID和产品PID */
/* 输入: 厂商ID低字节, 厂商ID高字节, 产品ID低字节, 产品ID高字节 */

#define    CMD_SET_USB_ADDR    0x13            /* 设置USB地址 */
/* 输入: 地址值 */

#define    CMD_SET_USB_MODE    0x15            /* 设置USB工作模式 */
/* 输入: 模式代码 */
/*       00H=未启用的设备方式, 01H=已启用的设备方式并且使用外部固件模式, 02H=已启用的设备方式并且使用内置固件模式 */
/*       04H=未启用的主机方式, 05H=已启用的主机方式, 06H=已启用的主机方式并且自动产生SOF包, 07H=已启用的主机方式并且复位USB总线 */
/* 输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

#define    CMD_SET_ENDP2        0x18            /* 设备方式: 设置USB端点0的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define    CMD_SET_ENDP3        0x19            /* 设备方式: 设置USB端点0的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define    CMD_SET_ENDP4        0x1A            /* 设备方式: 设置USB端点1的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define    CMD_SET_ENDP5        0x1B            /* 设备方式: 设置USB端点1的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define    CMD_SET_ENDP6        0x1C            /* 设置USB端点2/主机端点的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但不返回ACK, 1110-正忙NAK, 1111-错误STALL */

#define    CMD_SET_ENDP7        0x1D            /* 设置USB端点2/主机端点的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但无须应答, 1110-正忙NAK, 1111-错误STALL */

#define    CMD_GET_STATUS        0x22            /* 获取中断状态并取消中断请求 */
/* 输出: 中断状态 */

#define    CMD_UNLOCK_USB        0x23            /* 设备方式: 释放当前USB缓冲区 */

#define    CMD_RD_USB_DATA0    0x27            /* 从当前USB中断的端点缓冲区读取数据块 */
/* 输出: 长度, 数据流 */

#define    CMD_RD_USB_DATA        0x28            /* 从当前USB中断的端点缓冲区读取数据块, 并释放缓冲区, 相当于 CMD_RD_USB_DATA0 + CMD_UNLOCK_USB */
/* 输出: 长度, 数据流 */

#define    CMD_WR_USB_DATA3    0x29            /* 设备方式: 向USB端点0的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

#define    CMD_WR_USB_DATA5    0x2A            /* 设备方式: 向USB端点1的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

#define    CMD_WR_USB_DATA7    0x2B            /* 向USB端点2或者主机端点的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

/* ************************************************************************** */
/* 以下命令用于USB主机方式, 只有CH375支持 */

#define    CMD_SET_BAUDRATE    0x02            /* 主机方式 & 串口方式: 设置串口通讯波特率 */
/* 输入: 波特率分频系数, 波特率分频常数 */
/* 输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

#define    CMD_GET_DEV_RATE    0x0A            /* 主机方式: 获取当前连接的USB设备的数据速率类型 */
/* 输入: 数据07H */
/* 输出: 数据速率类型 */
/*                    位4为1则是1.5Mbps低速USB设备, 否则是12Mbps全速USB设备 */

#define    CMD_GET_MAX_LUN        0x0A            /* 主机方式: 获取USB存储器最大逻辑单元号 */
/* 输入: 数据38H */
/* 输出: 最大逻辑单元号 */

#define    CMD_SET_RETRY        0x0B            /* 主机方式: 设置USB事务操作的重试次数 */
/* 输入: 数据25H, 重试次数 */
/*                    位7为0则收到NAK时不重试, 位7为1位6为0则收到NAK时无限重试, 位7为1位6为1则收到NAK时最多重试2秒, 位5~位0为超时后的重试次数 */

#define    CMD_SET_DISK_LUN    0x0B            /* 主机方式: 设置USB存储器的当前逻辑单元号 */
/* 输入: 数据34H, 新的当前逻辑单元号(00H-0FH) */

#define    CMD_SET_PKT_P_SEC    0x0B            /* 主机方式: 设置USB存储器的每扇区数据包总数 */
/* 输入: 数据39H, 新的每扇区数据包总数(08H,10H,20H,40H) */

#define    CMD_TEST_CONNECT    0x16            /* 主机方式: 检查USB设备连接状态 */
/* 输出: 状态( USB_INT_CONNECT或USB_INT_DISCONNECT或USB_INT_USB_READY, 其它值说明操作未完成 ) */

#define    CMD_ABORT_NAK        0x17            /* 主机方式: 放弃当前NAK的重试 */

#define    CMD_CLR_STALL        0x41            /* 主机方式: 控制传输-清除端点错误 */
/* 输入: 端点号 */
/* 输出中断 */

#define    CMD_SET_ADDRESS        0x45            /* 主机方式: 控制传输-设置USB地址 */
/* 输入: 地址值 */
/* 输出中断 */

#define    CMD_GET_DESCR        0x46            /* 主机方式: 控制传输-获取描述符 */
/* 输入: 描述符类型 */
/* 输出中断 */

#define    CMD_SET_CONFIG        0x49            /* 主机方式: 控制传输-设置USB配置 */
/* 输入: 配置值 */
/* 输出中断 */

#define    CMD_AUTO_SETUP        0x4D            /* 主机方式: 自动配置USB设备 */
/* 输出中断 */

#define    CMD_ISSUE_TKN_X        0x4E            /* 主机方式: 发出同步令牌, 执行事务, 该命令可代替 CMD_SET_ENDP6/CMD_SET_ENDP7 + CMD_ISSUE_TOKEN */
/* 输入: 同步标志, 事务属性 */
/*           同步标志的位7为主机端点IN的同步触发位, 位6为主机端点OUT的同步触发位, 位5~位0必须为0 */
/*           事务属性的低4位是令牌, 高4位是端点号 */
/* 输出中断 */

#define    CMD_ISSUE_TOKEN        0x4F            /* 主机方式: 发出令牌, 执行事务 */
/* 输入: 事务属性 */
/*           低4位是令牌, 高4位是端点号 */
/* 输出中断 */

#define    CMD_DISK_BOC_CMD    0x50            /* 主机方式: 对USB存储器执行BulkOnly传输协议的命令 */
/* 输出中断 */

#define    CMD_DISK_INIT        0x51            /* 主机方式: 初始化USB存储器 */
/* 输出中断 */

#define    CMD_DISK_RESET        0x52            /* 主机方式: 控制传输-复位USB存储器 */
/* 输出中断 */

#define    CMD_DISK_SIZE        0x53            /* 主机方式: 获取USB存储器的容量 */
/* 输出中断 */

#define    CMD_DISK_READ        0x54            /* 主机方式: 从USB存储器读数据块(以扇区为单位) */
/* 输入: LBA扇区地址(总长度32位, 低字节在前), 扇区数(01H~FFH) */
/* 输出中断 */

#define    CMD_DISK_RD_GO        0x55            /* 主机方式: 继续执行USB存储器的读操作 */
/* 输出中断 */

#define    CMD_DISK_WRITE        0x56            /* 主机方式: 向USB存储器写数据块(以扇区为单位) */
/* 输入: LBA扇区地址(总长度32位, 低字节在前), 扇区数(01H~FFH) */
/* 输出中断 */

#define    CMD_DISK_WR_GO        0x57            /* 主机方式: 继续执行USB存储器的写操作 */
/* 输出中断 */

#define    CMD_DISK_INQUIRY    0x58            /* 主机方式: 查询USB存储器特性 */
/* 输出中断 */

#define    CMD_DISK_READY        0x59            /* 主机方式: 检查USB存储器就绪 */
/* 输出中断 */

#define    CMD_DISK_R_SENSE    0x5A            /* 主机方式: 检查USB存储器错误 */
/* 输出中断 */

#define    CMD_DISK_MAX_LUN    0x5D            /* 主机方式: 控制传输-获取USB存储器最大逻辑单元号 */
/* 输出中断 */

/* ********************************************************************************************************************* */
/* 操作状态 */

#define    CMD_RET_SUCCESS        0x51            /* 命令操作成功 */
#define    CMD_RET_ABORT        0x5F            /* 命令操作失败 */

/* ********************************************************************************************************************* */
/* USB中断状态 */

#ifndef    USB_INT_EP0_SETUP

/* 以下状态代码为特殊事件中断, 如果通过CMD_CHK_SUSPEND启用USB总线挂起检查, 那么必须处理USB总线挂起和睡眠唤醒的中断状态 */
#define    USB_INT_USB_SUSPEND    0x05            /* USB总线挂起事件 */
#define    USB_INT_WAKE_UP        0x06            /* 从睡眠中被唤醒事件 */

/* 以下状态代码0XH用于USB设备方式 */
/*   内置固件模式下只需要处理: USB_INT_EP1_OUT, USB_INT_EP1_IN, USB_INT_EP2_OUT, USB_INT_EP2_IN */
/*   位7-位4为0000 */
/*   位3-位2指示当前事务, 00=OUT, 10=IN, 11=SETUP */
/*   位1-位0指示当前端点, 00=端点0, 01=端点1, 10=端点2, 11=USB总线复位 */
#define    USB_INT_EP0_SETUP    0x0C            /* USB端点0的SETUP */
#define    USB_INT_EP0_OUT        0x00            /* USB端点0的OUT */
#define    USB_INT_EP0_IN        0x08            /* USB端点0的IN */
#define    USB_INT_EP1_OUT        0x01            /* USB端点1的OUT */
#define    USB_INT_EP1_IN        0x09            /* USB端点1的IN */
#define    USB_INT_EP2_OUT        0x02            /* USB端点2的OUT */
#define    USB_INT_EP2_IN        0x0A            /* USB端点2的IN */
/* USB_INT_BUS_RESET	0x0000XX11B */        /* USB总线复位 */
#define    USB_INT_BUS_RESET1    0x03            /* USB总线复位 */
#define    USB_INT_BUS_RESET2    0x07            /* USB总线复位 */
#define    USB_INT_BUS_RESET3    0x0B            /* USB总线复位 */
#define    USB_INT_BUS_RESET4    0x0F            /* USB总线复位 */

#endif

/* 以下状态代码2XH-3XH用于USB主机方式的通讯失败代码, 仅CH375支持 */
/*   位7-位6为00 */
/*   位5为1 */
/*   位4指示当前接收的数据包是否同步 */
/*   位3-位0指示导致通讯失败时USB设备的应答: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=超时 */
/* USB_INT_RET_ACK	0x001X0010B */            /* 错误:对于IN事务返回ACK */
/* USB_INT_RET_NAK	0x001X1010B */            /* 错误:返回NAK */
/* USB_INT_RET_STALL	0x001X1110B */        /* 错误:返回STALL */
/* USB_INT_RET_DATA0	0x001X0011B */        /* 错误:对于OUT/SETUP事务返回DATA0 */
/* USB_INT_RET_DATA1	0x001X1011B */        /* 错误:对于OUT/SETUP事务返回DATA1 */
/* USB_INT_RET_TOUT	0x001XXX00B */            /* 错误:返回超时 */
/* USB_INT_RET_TOGX	0x0010X011B */            /* 错误:对于IN事务返回数据不同步 */
/* USB_INT_RET_PID	0x001XXXXXB */            /* 错误:未定义 */

/* 以下状态代码1XH用于USB主机方式的操作状态代码, 仅CH375支持 */
#ifndef    USB_INT_SUCCESS
#define    USB_INT_SUCCESS        0x14            /* USB事务或者传输操作成功 */
#define    USB_INT_CONNECT        0x15            /* 检测到USB设备连接事件 */
#define    USB_INT_DISCONNECT    0x16            /* 检测到USB设备断开事件 */
#define    USB_INT_BUF_OVER    0x17            /* USB控制传输的数据太多, 缓冲区溢出 */
#define    USB_INT_USB_READY    0x18            /* USB设备已经被初始化（已分配USB地址） */
#define    USB_INT_DISK_READ    0x1D            /* USB存储器读数据块, 请求数据读出 */
#define    USB_INT_DISK_WRITE    0x1E            /* USB存储器写数据块, 请求数据写入 */
#define    USB_INT_DISK_ERR    0x1F            /* USB存储器操作失败 */
#endif

/* ********************************************************************************************************************* */
/* 常用USB定义 */

/* USB的包标识PID, 主机方式可能用到 */
#ifndef    DEF_USB_PID_SETUP
#define    DEF_USB_PID_NULL    0x00            /* 保留PID, 未定义 */
#define    DEF_USB_PID_SOF        0x05
#define    DEF_USB_PID_SETUP    0x0D
#define    DEF_USB_PID_IN        0x09
#define    DEF_USB_PID_OUT        0x01
#define    DEF_USB_PID_ACK        0x02
#define    DEF_USB_PID_NAK        0x0A
#define    DEF_USB_PID_STALL    0x0E
#define    DEF_USB_PID_DATA0    0x03
#define    DEF_USB_PID_DATA1    0x0B
#define    DEF_USB_PID_PRE        0x0C
#endif

/* USB请求类型, 外置固件模式可能用到 */
#ifndef    DEF_USB_REQ_TYPE
#define    DEF_USB_REQ_READ    0x80            /* 控制读操作 */
#define    DEF_USB_REQ_WRITE    0x00            /* 控制写操作 */
#define    DEF_USB_REQ_TYPE    0x60            /* 控制请求类型 */
#define    DEF_USB_REQ_STAND    0x00            /* 标准请求 */
#define    DEF_USB_REQ_CLASS    0x20            /* 设备类请求 */
#define    DEF_USB_REQ_VENDOR    0x40            /* 厂商请求 */
#define    DEF_USB_REQ_RESERVE    0x60            /* 保留请求 */
#endif

/* USB标准设备请求, RequestType的位6位5=00(Standard), 外置固件模式可能用到 */
#ifndef    DEF_USB_GET_DESCR
#define    DEF_USB_CLR_FEATURE    0x01
#define    DEF_USB_SET_FEATURE    0x03
#define    DEF_USB_GET_STATUS    0x00
#define    DEF_USB_SET_ADDRESS    0x05
#define    DEF_USB_GET_DESCR    0x06
#define    DEF_USB_SET_DESCR    0x07
#define    DEF_USB_GET_CONFIG    0x08
#define    DEF_USB_SET_CONFIG    0x09
#define    DEF_USB_GET_INTERF    0x0A
#define    DEF_USB_SET_INTERF    0x0B
#define    DEF_USB_SYNC_FRAME    0x0C
#endif

/* ********************************************************************************************************************* */

void ch372_config(void);
void ch372_interrupt_callback(void);

#endif //MAIN_C_DEVICE_CH372C_H_
