/*
 * udpsent.c
 * UDP�ͻ��� 	ʵ��UDP���ݵķ��͹���
 *	1���Զ������׽���
 *	2���˿ںŻ��������
 *	3�����ݷ���ǰ�����������������, ���򲻷���
 *  Created on: 2016��7��29��
 *      Author: Lzy
 */
#include "udpsent.h"

#include <time.h>
#include <netdb.h>

#define udp_PORT_SIZE 20 // udp �˿ںŵĸ���

static int sock_fd=-1; /*�װ���*/
static int port = 28720;
static struct sockaddr_in server_addr;
static char *server_ip = NULL;
extern char gUdpCount; // ����0���������   ������������

/* ����һ��socket��������SOCK_DGRAM��udp����*/
static int udp_creatSocket(void)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		udp_printf("udp Client Creat Socket Err\n");

	return sock;
}

/* ��ʼ��Ԥ���ӵķ���˵�ַ*/
static void udp_initServerAddr(void)
{
	char* url = server_ip;
//	printf("%s\n",url);

	struct hostent *host= (struct hostent *) gethostbyname(url); /* ͨ��������ڲ���url���host��ַ���������������������������*/

	/* ��ʼ��Ԥ���ӵķ���˵�ַ */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
}


/**
 * ��ʼ���׽��֣���ʼ���˿ں�
 */
static void udp_initSocket(void)
{
	if(sock_fd < 0)
	{
		sock_fd = udp_creatSocket(); // �����׽���
		if(sock_fd< 0)
			udp_printf("udp init Socket Err\n");

		//srand(time(0)); // ��ȡ��������
		port = 28720+rand()%udp_PORT_SIZE; // �������һ���˿�
		server_ip = udp_getServerIP();
	}
}



/**
 * ��  �ܣ�udp ���������������
 * ��  �أ�true
 */
int udp_sentCheck(void)
{
	int ret = false;
	udp_initSocket();

	/*���͵����������׽��֣� �з����IP������ʱ����0*/
	if((sock_fd>=0)&&(strlen(server_ip) > 0) && (gUdpCount > 2))
		ret = true;
//	else
//		udp_debug("udp sent check Err!\n");

	return ret;
}



/**
 * ���ܣ�udp ���ݷ���
 * ��ڲ�����*send_data -> ���ݻ�����  len -> ���ݳ���
 * ���ڲ�����
 * ����: ���ͳ��ȣ�
 */
int udp_clientSentData(uchar *send_data, uint len)
{
	int ret=0, sockfd=sock_fd;

	ret = udp_sentCheck();	/*����˼��*/
	if(ret == true)
	{
		udp_initServerAddr();

		/* �������ݵ�����Զ�� */
		ret = sendto(sockfd, send_data, len, 0,
				(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		udp_udelay(15);  // ����ʱ
	}

	return ret;
}

/**
 * �رտͻ��� �׽���
 */
void udp_clientClose(void)
{
	//close(sock_fd);
}