/*
 * data_packet.c
 *
 *  Created on: 2019Äê6ÔÂ6ÈÕ
 *      Author: luozhiyong
 */
#include "data_packet.h"


sDataPacket *data_packet_get(int id)
{
	static sPackets* packets = nullptr;
	if(!packets) {
		packets = malloc(sizeof(sPackets));
		memset(packets, 0, sizeof(sPackets));
	}

	return &(packets->packet[id]);
}

int data_packet_getLines(int id)
{
	int ret = 0;
	sDataPacket *pack = data_packet_get(id);
	if(pack && (pack->offLine>0)) {
		ret = pack->data.line.size;
	}
	return ret;
}

int data_packet_getDc(int id)
{
	int ret = 0;
	sDataPacket *pack = data_packet_get(id);
	if(pack && (pack->offLine>0)) {
		ret = pack->dc;
	}

	return ret;
}
