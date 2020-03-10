/*
 * udpclient.c
 *
 *  Created on: 2019-6-12
 *      Author: luozhiyong
 */
#include "udp_client.h"

static int gUdpClientSock = -1;
static struct sockaddr_in gUdpClientserver_addr;

int udp_client_init(char *url, int port)
{
	struct sockaddr_in *server_addr = &gUdpClientserver_addr;

	
	struct hostent *host = (struct hostent *) gethostbyname(url);
	udp_client_close();

	
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		rt_kprintf("Socket error\n");
		return false;
	}
	gUdpClientSock = sock;

	
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(port);
	server_addr->sin_addr = *((struct in_addr *)host->h_addr);
	rt_memset(&(server_addr->sin_zero), 0, sizeof(server_addr->sin_zero));

	return sock;
}


int udp_client_sent(char *send_data)
{
	int ret = true;
	int sock = gUdpClientSock;
	struct sockaddr_in *server_addr = &gUdpClientserver_addr;
	if(sock >= 0) {
		
		ret = sendto(sock, send_data, strlen(send_data), 0, (struct sockaddr *)server_addr, sizeof(struct sockaddr));
		msleep(5);
	} else {
		ret = false;
	}

	return ret;
}


void udp_client_close(void)
{
	int sock = gUdpClientSock;
	if(sock >= 0) {
		closesocket(sock);
		gUdpClientSock = -1;
	}
}

int udp_client_connected(void)
{
	return gUdpClientSock;
}
