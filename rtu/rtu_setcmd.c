/*
 * rtu_set.c
 *
 *  Created on: 2019-10-23
 *      Author: luozhiyong
 */
#include "rtu_setcmd.h"

int rtu_setCmd(sRtuSetCmd *cmd)
{
	sDataPacket *packet = data_packet_get(cmd->addr);
	if(packet->offLine > 0) {
		uchar *buf = malloc(RTU_CMD_LEN+2);
		int ret = rtu_sentCmd(cmd, buf);

		sRtu *rtu= rtu_cfg_addr(cmd->addr);
		list_t *list = rtu->cmds;
		list_rpush(list, list_node_new(buf)); 
	}

	return packet->offLine;
}


int rtu_getCmd(sRtu *rtu)
{
	int ret = RTU_CMD_LEN;
	list_t *list = rtu->cmds;
	list_node_t *node = list_at(list, 0);
	if(node) {
		memcpy(rtu->sentBuf, node->val, ret);
		list_remove(list, node);
	} else {
		ret = 0;
	}
	rtu->sentLen = ret;

	return ret;
}


int rtu_setReg(uchar addr, ushort reg, ushort value)
{
	sRtuSetCmd cmd;
	cmd.addr = addr;
	cmd.reg = reg;
	cmd.value = value;
	return rtu_setCmd(&cmd);
}

int rtu_clean_ele(uchar addr)
{
	return rtu_setReg(addr, SI_ELE_CMD, 0xf0);
}


int rtu_setAllVolCmd(uchar addr,ushort minValue,ushort maxValue)
{
	rtu_setReg(addr,SI_MinVol1_CMD,minValue);
	rtu_setReg(addr,SI_MaxVol1_CMD,maxValue);
	rtu_setReg(addr,SI_MinVol2_CMD,minValue);
	rtu_setReg(addr,SI_MaxVol2_CMD,maxValue);
	rtu_setReg(addr,SI_MinVol3_CMD,minValue);
	return rtu_setReg(addr,SI_MaxVol3_CMD,maxValue);
}

int rtu_setVolCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue,boolean acOrDc)
{
	if(acOrDc)//AC
	{
		if(phase==0)
			return rtu_setAllVolCmd(addr,minValue,maxValue);
		else
		{
			rtu_setReg(addr,SI_MinVol1_CMD+2*(phase-1),minValue);
			return rtu_setReg(addr,SI_MaxVol1_CMD+2*(phase-1),maxValue);
		}
	}
	else//DC
	{
		rtu_setReg(addr,SI_DC_MinVol_CMD,minValue);
		return rtu_setReg(addr,SI_DC_MaxVol_CMD,maxValue);
	}
}

int rtu_setAllCurCmd(uchar addr,ushort minValue,ushort maxValue)
{
	rtu_setReg(addr,SI_MinCur1_CMD,minValue);
	rtu_setReg(addr,SI_MaxCur1_CMD,maxValue);
	rtu_setReg(addr,SI_MinCur2_CMD,minValue);
	rtu_setReg(addr,SI_MaxCur2_CMD,maxValue);
	rtu_setReg(addr,SI_MinCur3_CMD,minValue);
	return rtu_setReg(addr,SI_MaxCur3_CMD,maxValue);
}

int rtu_setCurCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue,boolean acOrDc)
{
	if(acOrDc)//AC
	{
		if(phase==0)
			return rtu_setAllCurCmd(addr,minValue,maxValue);
		else
		{
			rtu_setReg(addr,SI_MinCur1_CMD+2*(phase-1),minValue);
			return rtu_setReg(addr,SI_MaxCur1_CMD+2*(phase-1),maxValue);
		}
	}
	else//DC
	{
		rtu_setReg(addr,SI_DC_MinCur_CMD,minValue);
		return rtu_setReg(addr,SI_DC_MaxCur_CMD,maxValue);
	}
}

int rtu_setEnvCmd(uchar addr,ushort minValue,ushort maxValue,boolean temOrHum)
{
	if(temOrHum)//Tem
	{
		rtu_setReg(addr,SI_MinTem_CMD,minValue);
		return rtu_setReg(addr,SI_MaxTem_CMD,maxValue);
	}
	else//Hum
	{
		rtu_setReg(addr,SI_MinHum_CMD,minValue);
		return rtu_setReg(addr,SI_MaxHum_CMD,maxValue);
	}
}

int rtu_setBrCmd(uchar addr,int baud)
{
	return rtu_setReg(addr,SI_BR_CMD,baud);
}

int rtu_setAddrCmd(uchar addr,uchar changeAddr)
{
	return rtu_setReg(addr,SI_Addr_CMD,changeAddr);
}