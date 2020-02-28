/*
 * rtu_recv.h
 *
 *  Created on: 2019��10��22��
 *      Author: luozhiyong
 */

#ifndef BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_UART_RTU_RECV_H_
#define BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_UART_RTU_RECV_H_
#include "rtu_sentcmd.h"

#define RTU_LINE_NUM LINE_NUM

typedef struct sRtuRecv {
    uchar addr; // ��ʾ�ӻ���ַ��
    uchar fn; // ��ʾ������
    ushort len; // ��ʾ�����ֽ���
    char offLine;

    sDataPacket *data;
    ushort crc; // ������
}sRtuRecv;

boolean rtu_recv_packet(uchar *buf, int len, sRtuRecv *pkt);

#endif /* BSP_STM32_STM32F429_ATK_APOLLO_APPLICATIONS_UART_RTU_RECV_H_ */