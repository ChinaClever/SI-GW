/*
 * datasegment.h
 *
 *  Created on: 2016��7��29��
 *      Author: Lzy
 */

#ifndef NETDATA_NETDATASEGMENT_H_
#define NETDATA_NETDATASEGMENT_H_

#include "netDataPacket.h"

#define DATA_DEV_VERSION		1 /*�豸ͨѶ����*/


/**
 * �������д��� ���ݿ��
 */
typedef struct dev_data_packet
{
    uchar addr;	/*��ַ*/
    uchar fn[2];	/*������*/
    ushort len; /*���ݳ���*/
    uchar *data; /*����*/
}dev_data_packet;

int data_packet_sent(dev_data_packet *msg);
int data_packet_analytic(uchar *buf, ushort len, dev_data_packet *pkt);

#endif /* NETDATA_NETDATASEGMENT_H_ */