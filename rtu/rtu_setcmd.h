/*
 * rtu_setcmd.h
 *
 *  Created on: 2019-10-23
 *      Author: luozhiyong
 */

#ifndef BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_RTU_RTU_SETCMD_H_
#define BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_RTU_RTU_SETCMD_H_
#include "rtu_recv.h"

enum SI_SetCmdEnum {
    SI_Addr_CMD = 0x1001, //PDU
    SI_MaxVol1_CMD,   // L1
    SI_MinVol1_CMD,   // L1
    SI_MaxVol2_CMD,   // L2
    SI_MinVol2_CMD,   // L2
    SI_MaxVol3_CMD,   // L3
    SI_MinVol3_CMD,   // L3

    SI_MaxCur1_CMD,   // L1
    SI_MinCur1_CMD,   // L1
    SI_MaxCur2_CMD,   // L2
    SI_MinCur2_CMD,   // L2
    SI_MaxCur3_CMD,   // L3
    SI_MinCur3_CMD,   // L3

    SI_MaxTem_CMD,   // 
    SI_MinTem_CMD,   // 
    SI_MaxHum_CMD,   // 
    SI_MinHum_CMD,   // 

    SI_BR_CMD, // 01Ϊ4800 02Ϊ9600 03Ϊ19200 04Ϊ38400
    SI_ELE_CMD, // 1111 0000

    SI_DC_MaxVol_CMD,   // 
    SI_DC_MinVol_CMD,   // 
	SI_DC_MaxCur_CMD,   //
	SI_DC_MinCur_CMD,   // 

	SI_Mode_CMD, // Modbus RTU ģʽ
	// 00 00 Modbus RTUģʽ(Ϊ 0x03 Ϊ 0x10)
	// 00 01 Modbus RTU ģʽ(Ϊ0x03 Ϊ 0x06)
	// 01 10 10 19 00 01 D4 CE
};

int rtu_getCmd(sRtu *rtu);
int rtu_setCmd(sRtuSetCmd *cmd);
int rtu_setReg(uchar addr, ushort reg, ushort value);
int rtu_clean_ele(uchar addr);


int rtu_setAllVolCmd(uchar addr,ushort minValue,ushort maxValue);
int rtu_setAllCurCmd(uchar addr,ushort minValue,ushort maxValue);
int rtu_setVolCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue,boolean acOrDc);//acOrDc true:AC false:DC
int rtu_setCurCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue,boolean acOrDc);//acOrDc true:AC false:DC
int rtu_setEnvCmd(uchar addr,ushort minValue,ushort maxValue,boolean temOrHum);//temOrHum true:Tem false:Hum
int rtu_setBrCmd(uchar addr,int baud);
int rtu_setAddrCmd(uchar addr,uchar changeAddr);
#endif /* BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_RTU_RTU_SETCMD_H_ */
