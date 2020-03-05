/*
 * udpservice.c
 * UDP �����  ʵ��UDP���չ���
 *  1���ṩ�����׽��ֽӿ�
 *  2���ṩ���ݽ��սӿ�
 *
 *  Created on: 2016��7��29��
 *      Author: Lzy
 */
#include "udpservice.h"
#define UDP_PORT	18750 // �˿ں�
extern void net_recv_dataPacket(dev_data_packet *pkt);

/**
 * ���ܣ�����UDP������׽���
 * ���� ���׽���
 */
int udp_serviceSocket(int port)
{
	int sockfd;
	struct sockaddr_in server_addr;/* ����IP��ַ�Ͷ˿ں� */

	/* ����һ��socket��������SOCK_DGRAM��UDP���� */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		udp_printf("Socket error\n");
		return -1;
	}

	/* ��ʼ������˵�ַ */
	server_addr.sin_family = AF_INET;		/*IPv4��������*/
	server_addr.sin_port = htons(port);    /*�˿ںţ�������������ֽ����ת�� */
	server_addr.sin_addr.s_addr = INADDR_ANY;
	//server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

	/* ��socket������˵�ַ */
	if (bind(sockfd, (struct sockaddr *)&server_addr,
			sizeof(struct sockaddr)) == -1)
	{
		/* �󶨵�ַʧ�� */
		udp_printf("Bind error\n");
		return -1;
	}
	//udp_printf("UDP Server Waiting for client on port %d...\n", port);

	return sockfd;
}



/**
 * ��  �ܣ�UDP ���ݽ���
 * ���ڲ�����*recv_data -> �������ݻ�����
 * ��  ��: ���ݳ���
 */
int udp_serviceRecvData(int sockfd,struct sockaddr_in *client_addr,uchar *recv_data)
{
	int ret=0;
	uint addr_len = sizeof(struct sockaddr);

	/*��ʱ����*/
	//	struct timeval timeout={3,0};//3S
	//	ret=setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	//	if(ret<0)
	//		udp_printf("UDP setsockopt Err\n");

	/* ��sock����ȡ���BUF_SIZE - 1�ֽ����� */
	/* UDP��ͬ��TCP�����������������ȡ������ʧ�ܵ���������������˳�ʱ�ȴ� */
	ret = recvfrom(sockfd, recv_data, UDP_BUF_SIZE - 1, 0,
			(struct sockaddr *)client_addr, &addr_len);
	//	udp_printf("UDP Recv Data len:%d %s\n",ret, recv_data);

	return ret;
}


static void udp_recv_data(uchar *buf, int len)
{
	dev_data_packet pkt;
	int rtn = data_packet_analytic(buf, len, &pkt);
	if(rtn > 0) {
		if(pkt.addr == 0xff) {
			int addr;
			for(addr=1; addr<=DEV_NUM; ++addr) { // �豸���и���
				pkt.addr = addr;
				net_recv_dataPacket(&pkt);
			}
		} else {
			net_recv_dataPacket(&pkt);
		}
	} else
		udp_printf("recv err %d\n", rtn);
}


static void udps_thread_entry(void *p)
{
	int rtn=0;
	udp_delay(4);
	uchar buf[UDP_BUF_SIZE/2] = {0};
	struct sockaddr_in clientAddr;
	int sock_fd = udp_serviceSocket(UDP_PORT);

	if(sock_fd!=-1)
	{
		while(1) {
			memset(buf, 0, sizeof(buf));
			rtn = udp_serviceRecvData(sock_fd, &clientAddr, buf);
			if(rtn > 0) {
				udp_recv_data(buf, rtn);
			}
		}
	}
	else
	{
		udp_printf("Socket error\n");
	}

}

/**
 * �����������������߳�
 */
void udp_recv_thread(void)
{
	rt_thread_t tid = rt_thread_create("m_recv",udps_thread_entry, NULL, 2*512, 27, 15);
	if (tid != RT_NULL) rt_thread_startup(tid);
}
