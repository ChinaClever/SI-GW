/*
 * netdata.h
 *
 *  Created on: 2016��7��29��
 *      Author: Lzy
 */

#ifndef NETDATA_NETDATAPACKET_H_
#define NETDATA_NETDATAPACKET_H_

#include "udpsent.h"

#define DATA_MSG_SIZE	512		/*���ݻ�������С*/
#define DEV_CODE_SIZE	4		/*�豸����λ��*/
#define DATA_MSG_CODE_SIZE	9

#define DATA_MSG_HDR	0x7E	/*��Ϣͷ~*/
#define DATA_MSG_STX	0x5E	/*��ʶ��^*/
#define DATA_MSG_ED	0x23	/*������*/

#define DATA_MSG_SERVICE	0x10	/*����˱�־*/
#define DATA_MSG_CLIENT		0x03	/*�ͻ��˱�־*/

/*����ͨѶ����*/
typedef enum {
	TRA_TYPR_UDP=1, /*UDPͨѶ*/
	TRA_TYPR_TCP,
	//===    ����������������Ҫ��ͨѶ����
}data_traffic_type;


/*�����붨��*/
typedef enum {
	DATA_ERR_ABNORMAL=-1, /*���ݲ���������*/
	DATA_ERR_HEAD=-2, /*ͬ��ͷ����*/
	DATA_ERR_END=-3, /*����������*/
	DATA_ERR_XOR=-4,/*У�������*/
}data_return_err;

/*���Ŷνṹ��*/
typedef struct
{
	uchar devCode[DEV_CODE_SIZE]; /*�豸����*/
	uchar type; /*ͨѶ����*/
	uchar version; /*�汾��*/
	uchar trans;	/*����˷��ͱ�־��10H �ͻ���Ӧ���־��03H */
	ushort reserve; /*Ԥ��2�ֽ�*/
}data_code;

/**
 * ���ݰ��ṹ�壬
 */
typedef struct
{
	uchar hdr;	/*��Ϣͷ 0x7E*/
	uchar stx;	/*��ʶ��0x5E*/
	data_code code; /*���Ŷ�*/
	ushort len; /*���ݳ��� */
	uchar *data;	/*���ݶ�*/
	ushort XOR; /*У����*/
	uchar ED; /*������*/
}data_packet;

int data_msg_packetSent(data_code *code,uchar *buf, ushort len);
int data_msg_analytic(uchar *buf, ushort len, data_packet *msg);


#endif /* NETDATA_NETDATAPACKET_H_ */