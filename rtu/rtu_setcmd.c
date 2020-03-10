/*
 * rtu_set.c
 *
 *  Created on: 2019��10��23��
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
		list_rpush(list, list_node_new(buf)); // ��β�����ӽڵ�
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


static ushort rtu_setUnitCmd(uchar addr,ushort reg, ushort minValue,ushort maxValue)
{
	rtu_setReg(addr, reg++, maxValue);
	rtu_setReg(addr, reg++,minValue);

	return reg;
}

static void rtu_setAllCmd(uchar addr,ushort reg, ushort minValue,ushort maxValue)
{
	int i, line = data_packet_getLines(addr);
	for(i=0; i<line; ++i) {
		reg = rtu_setUnitCmd(addr, reg, minValue, maxValue);
	}
}

void rtu_setVolCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue)
{
	ushort reg = SI_MaxVol1_CMD;
	boolean dc = data_packet_getDc(addr);
	if(dc) {
		reg = SI_MaxVol1_CMD+2*(phase-1);
	} else {
		reg = SI_DC_MaxVol_CMD;
	}

	if(phase) {
		rtu_setUnitCmd(addr, reg, minValue, maxValue);
	} else {
		rtu_setAllCmd(addr, reg, minValue, maxValue);
	}
}


void rtu_setCurCmd(uchar addr,uchar phase,ushort minValue,ushort maxValue)
{
	ushort reg = SI_MaxCur1_CMD;
	boolean dc = data_packet_getDc(addr);
	if(dc) {
		reg = SI_MaxCur1_CMD+2*(phase-1);
	} else {
		reg = SI_DC_MaxCur_CMD;
	}

	if(phase) {
		rtu_setUnitCmd(addr, reg, minValue, maxValue);
	} else {
		rtu_setAllCmd(addr, reg, minValue, maxValue);
	}
}

void rtu_setTemCmd(uchar addr,ushort minValue,ushort maxValue)
{
	ushort reg = SI_MaxTem_CMD;
	rtu_setUnitCmd(addr, reg, minValue, maxValue);
}

void rtu_setHumCmd(uchar addr,ushort minValue,ushort maxValue)
{
	ushort reg = SI_MaxHum_CMD;
	rtu_setUnitCmd(addr, reg, minValue, maxValue);
}


int rtu_setBrCmd(uchar addr,int baud)
{
	return rtu_setReg(addr,SI_BR_CMD,baud);
}

int rtu_setAddrCmd(uchar addr,uchar changeAddr)
{
	return rtu_setReg(addr,SI_Addr_CMD,changeAddr);
}
