/*
 * devDataRecv.c
 *
 *  Created on: 2019锟斤拷10锟斤拷12锟斤拷
 *      Author: luozhiyong
 */
#include "devDataSent.h"

static int charToShort(uchar *form, int len, int *to)
{
	int i, offset=0;
	for(i=0; i<len; i+=2)
		to[offset++] = form[i] *256 + form[i+1];
	return offset;
}

static void setUnit(sDataUnit *unit, dev_data_packet *pkt, double rate)
{
	int buf[24] = {0}, i;
	char setbuf[128];
	memset(setbuf,0,sizeof(setbuf));
	charToShort(pkt->data, pkt->len, buf);

	ushort id = pkt->addr;
	int fn = pkt->fn[1]; //锟斤拷锟轿伙拷锟�
	if(fn) {
		unit->min[fn-1] = buf[0]/rate; // 锟斤拷小值
		unit->max[fn-1] = buf[1]/rate; // 锟斤拷锟街�
	} else {
		for( i=0; i<LINE_NUM; ++i) {
			unit->min[i] = buf[0]/rate; // 锟斤拷小值
			unit->max[i] = buf[1]/rate; // 锟斤拷锟街�
		}
	}
}


static void setLineVol(sDevData *dev, dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; //锟斤拷锟轿伙拷锟�


	sDataUnit *unit = &(dev->line.vol);
	setUnit(unit, pkt, COM_RATE_VOL);
	if(fn) {
		rtu_setVolCmd(id,fn,unit->min[fn-1],unit->max[fn-1]);
	} else{
		rtu_setVolCmd(id,fn,unit->min[0],unit->max[0]);
	}
}



static void setLineCur(sDevData *dev,dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; //锟斤拷锟轿伙拷锟�
	

	sDataUnit *unit = &(dev->line.cur);
	setUnit(unit, pkt, 1);
	if(fn) {
		rtu_setCurCmd(id,fn,unit->min[fn-1],unit->max[fn-1]);
	} else{
		rtu_setCurCmd(id,fn,unit->min[0],unit->max[0]);
	}
}


static void setEnvTem(sDevData *dev,dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; //锟斤拷锟轿伙拷锟�
	

	sDataUnit *unit = &(dev->env.tem);
	setUnit(unit, pkt, COM_RATE_VOL);
	if(fn) {
		rtu_setTemCmd(id,unit->min[fn-1],unit->max[fn-1]);
	}
}


static void setEnvHum(sDevData *dev, dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; //锟斤拷锟轿伙拷锟�
	

	sDataUnit *unit = &(dev->env.hum);
	setUnit(unit, pkt, COM_RATE_VOL);
	if(fn) {
		rtu_setHumCmd(id,unit->min[fn-1],unit->max[fn-1]);
	}
}



/**
 * 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟� 锟斤拷锟叫斤拷锟斤拷
 */
void net_recv_dataPacket(dev_data_packet *pkt)
{
	int id = pkt->addr;
	sDataPacket *packet = data_packet_get(id);
    if(packet->offLine < 1) return;
	sDevData *dev = &(packet->data);
	switch(pkt->fn[0])
	{
	//	case 0:  setOutputCur(pkt);	break; // 锟斤拷锟斤拷锟斤拷锟轿伙拷锟斤拷锟�
	case 1:  setLineVol(dev, pkt); break;	// 锟斤拷锟斤拷锟斤拷锟窖�
	case 2:  setLineCur(dev, pkt); break; // 锟斤拷锟斤拷锟斤拷锟斤拷锟�
	case 3:  setEnvTem(dev, pkt); break; // 锟斤拷锟斤拷湿锟斤拷
	case 4:  setEnvHum(dev, pkt); break; // 锟斤拷锟斤拷湿锟斤拷
	//	case 5:  setDevName(pkt); break; // 锟斤拷锟斤拷锟斤拷锟轿伙拷锟斤拷锟�
	//	case 6:  setUserPwd(pkt); break; // 锟矫伙拷锟斤拷锟斤拷锟斤拷锟斤拷
	//	case 7:  setDevNet(pkt); break; // 锟借备锟斤拷锟斤拷锟斤拷息
	//	case 10: setOutputName(pkt); break; // 锟借备锟斤拷锟轿伙拷锟斤拷锟�
	//	case 13: setSwicth(pkt); break; // 锟斤拷锟矫匡拷锟斤拷
	//	case 18: setAllSwicth(pkt); break; // 锟斤拷锟斤拷锟斤拷锟叫匡拷锟斤拷
	//	case 20: setSysCmd(pkt); break; // 系统锟斤拷锟斤拷
	}
}

