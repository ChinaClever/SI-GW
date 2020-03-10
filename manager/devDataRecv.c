/*
 * devDataRecv.c
 *
 *  Created on: 2019-10-12
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
	int fn = pkt->fn[1]; 
	if(fn) {
		unit->min[fn-1] = buf[0]/rate; 
		unit->max[fn-1] = buf[1]/rate; 
	} else {
		for( i=0; i<LINE_NUM; ++i) {
			unit->min[i] = buf[0]/rate;
			unit->max[i] = buf[1]/rate; 
		}
	}
}


static void setLineVol(sDevData *dev, dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; 

	sDataUnit *unit = &(dev->line.vol);
	setUnit(unit, pkt, COM_RATE_VOL);
	if(fn) {
		rtu_setVolCmd(id,fn,unit->min[fn-1],unit->max[fn-1],true);
	}
	else{
		rtu_setVolCmd(id,fn,unit->min[0],unit->max[0],true);
	}
}



static void setLineCur(sDevData *dev,dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; 
	

	sDataUnit *unit = &(dev->line.cur);
	setUnit(unit, pkt, 1);
	if(fn) {
		rtu_setCurCmd(id,fn,unit->min[fn-1],unit->max[fn-1],true);
	}
	else{
		rtu_setVolCmd(id,fn,unit->min[0],unit->max[0],true);
	}
}


static void setEnvTem(sDevData *dev,dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; 
	

	sDataUnit *unit = &(dev->env.tem);
	setUnit(unit, pkt, COM_RATE_VOL);
	if(fn) {
		rtu_setEnvCmd(id,unit->min[fn-1],unit->max[fn-1],true);
	}
}


static void setEnvHum(sDevData *dev, dev_data_packet *pkt)
{
	int id = pkt->addr;
	int fn = pkt->fn[1]; 
	

	sDataUnit *unit = &(dev->env.hum);
	setUnit(unit, pkt, COM_RATE_VOL);
	if(fn) {
		rtu_setEnvCmd(id,unit->min[fn-1],unit->max[fn-1],false);
	}
}



/**
 * 
 */
void net_recv_dataPacket(dev_data_packet *pkt)
{
	int id = pkt->addr;
	sDataPacket *packet = data_packet_get(id);
    if(packet->offLine < 1) return;
	sDevData *dev = &(packet->data);
	switch(pkt->fn[0])
	{
	//	case 0:  setOutputCur(pkt);	break; 
	case 1:  setLineVol(dev, pkt); break;	
	case 2:  setLineCur(dev, pkt); break;
	case 3:  setEnvTem(dev, pkt); break;
	case 4:  setEnvHum(dev, pkt); break;
	//	case 5:  setDevName(pkt); break; 
	//	case 6:  setUserPwd(pkt); break; 
	//	case 7:  setDevNet(pkt); break; 
	//	case 10: setOutputName(pkt); break; 
	//	case 13: setSwicth(pkt); break; 
	//	case 18: setAllSwicth(pkt); break; 
	//	case 20: setSysCmd(pkt); break; 
	}
}

