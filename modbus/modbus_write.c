/*
 * modbus_write.c
 *
 *  Created on: 2019��10��24��
 *      Author: luozhiyong
 */
#include "modbus_write.h"
#include "rtu_setcmd.h"

int mb_write_reg(uchar addr, ushort reg, ushort value)
{
	return rtu_setReg(addr, reg, value);
}


int mb_write_regs(uchar addr, ushort reg, ushort num, const uchar *ptr)
{
	ushort i, ret, value;
	ptr += 3;
	for(i=0; i<num; ++i) {
		value = FLIPW(ptr); ptr += 2;
		ret = mb_write_reg(addr, reg+i, value);
		if(ret == 0)  break;
	}
	return ret;
}


