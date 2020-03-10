/*
 * rtu_setcmd.h
 *
 *  Created on: 2019锟斤拷10锟斤拷23锟斤拷
 *      Author: luozhiyong
 */

#ifndef BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_RTU_RTU_SETCMD_H_
#define BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_RTU_RTU_SETCMD_H_
#include "rtu_recv.h"

enum SI_SetCmdEnum {
    SI_Addr_CMD = 0x1001, //PDU锟斤拷址
    SI_MaxVol1_CMD,   // 锟斤拷锟斤拷锟斤拷压锟斤拷锟睫憋拷锟斤拷L1
    SI_MinVol1_CMD,   // 锟斤拷锟斤拷欠压锟斤拷锟睫憋拷锟斤拷L1
    SI_MaxVol2_CMD,   // 锟斤拷锟斤拷锟斤拷压锟斤拷锟睫憋拷锟斤拷L2
    SI_MinVol2_CMD,   // 锟斤拷锟斤拷欠压锟斤拷锟睫憋拷锟斤拷L2
    SI_MaxVol3_CMD,   // 锟斤拷锟斤拷锟斤拷压锟斤拷锟睫憋拷锟斤拷L3
    SI_MinVol3_CMD,   // 锟斤拷锟斤拷欠压锟斤拷锟睫憋拷锟斤拷L3

    SI_MaxCur1_CMD,   // 锟斤拷锟矫碉拷锟斤拷锟斤拷锟睫憋拷锟斤拷L1
    SI_MinCur1_CMD,   // 锟斤拷锟矫碉拷锟斤拷锟斤拷锟睫憋拷锟斤拷L1
    SI_MaxCur2_CMD,   // 锟斤拷锟矫碉拷锟斤拷锟斤拷锟睫憋拷锟斤拷L2
    SI_MinCur2_CMD,   // 锟斤拷锟矫碉拷锟斤拷锟斤拷锟睫憋拷锟斤拷L2
    SI_MaxCur3_CMD,   // 锟斤拷锟矫碉拷锟斤拷锟斤拷锟睫憋拷锟斤拷L3
    SI_MinCur3_CMD,   // 锟斤拷锟矫碉拷锟斤拷锟斤拷锟睫憋拷锟斤拷L3

    SI_MaxTem_CMD,   // 锟斤拷锟斤拷锟铰讹拷锟斤拷锟睫憋拷锟斤拷值
    SI_MinTem_CMD,   // 锟斤拷锟斤拷锟铰讹拷锟斤拷锟睫憋拷锟斤拷值2
    SI_MaxHum_CMD,   // 锟斤拷锟斤拷湿锟饺讹拷锟斤拷锟睫憋拷锟斤拷值
    SI_MinHum_CMD,   // 锟斤拷锟斤拷湿锟斤拷锟斤拷锟睫憋拷锟斤拷值

    SI_BR_CMD, // 锟斤拷锟矫诧拷锟斤拷锟绞ｏ拷01为4800锟斤拷02为9600锟斤拷03为19200锟斤拷04为38400锟斤拷
    SI_ELE_CMD, // 锟斤拷锟斤拷值锟斤拷锟斤拷 锟斤拷1111 0000锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷为锟斤拷一锟斤拷锟节讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟街憋拷锟斤拷锟�

    SI_DC_MaxVol_CMD,   // 锟斤拷锟斤拷直锟斤拷锟斤拷压锟斤拷锟斤拷锟斤拷锟斤拷
    SI_DC_MinVol_CMD,   // 锟斤拷锟斤拷直锟斤拷锟斤拷压锟斤拷锟斤拷锟斤拷锟斤拷
	SI_DC_MaxCur_CMD,   // 锟斤拷锟斤拷直压锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	SI_DC_MinCur_CMD,   // 锟斤拷锟斤拷直锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷

	SI_Mode_CMD, // Modbus RTU 模式锟叫伙拷
	// 00 00锟斤拷示锟斤拷司锟斤拷准Modbus RTU模式(锟斤拷取锟斤拷锟斤拷锟斤拷	为 0x03锟斤拷锟斤拷锟矫癸拷锟斤拷锟斤拷为 0x10)
	// 00 01 锟斤拷业锟斤拷准 Modbus RTU 模式(锟斤拷取锟斤拷锟斤拷锟斤拷为	0x03锟斤拷锟斤拷锟矫癸拷锟斤拷锟斤拷为 0x06)
	// 01 10 10 19 00 01 D4 CE
};

int rtu_getCmd(sRtu *rtu);
int rtu_setCmd(sRtuSetCmd *cmd);
int rtu_setReg(uchar addr, ushort reg, ushort value);
int rtu_clean_ele(uchar addr);

void rtu_setVolCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue);
void rtu_setCurCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue);
void rtu_setTemCmd(uchar addr,ushort minValue,ushort maxValue);
void rtu_setHumCmd(uchar addr,ushort minValue,ushort maxValue);

int rtu_setBrCmd(uchar addr,int baud);
int rtu_setAddrCmd(uchar addr,uchar changeAddr);
#endif /* BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_RTU_RTU_SETCMD_H_ */
