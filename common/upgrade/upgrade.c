/*
 * upgrade.c
 *
 *  Created on: 2019年12月18日
 *      Author: luozhiyong
 */
#include "udp_server.h"


extern int http_ota_fw_download(const char* uri);

int up_serv_recv(int sock, char *ip)
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(struct sockaddr);
	char recv_data[32];
	int len = 24;

	int bytes_read = false;
	if(sock >= 0) {
		bytes_read = recvfrom(sock, recv_data, len-1, 0, (struct sockaddr *)&client_addr, &addr_len);
		recv_data[bytes_read] = '\0'; /* 把末端清零 */ /* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待 */
		// rt_kprintf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		sprintf(ip, "%s", inet_ntoa(client_addr.sin_addr));
	}

	return bytes_read;
}


void up_serv_task(void *arg)
{
	int port = 1750;
	int ret, sock = 0;
	char url[48], ip[32];

	sleep(2);
	sock = udp_serv_init(port);
	while(1) {
		ret = up_serv_recv(sock, ip);
		if(ret > 0) {
			sprintf(url, "http://%s:1180", ip);
			http_ota_fw_download(url);
		}
		msleep(1);
	}
}

int up_serv_thread(void)
{
	rt_thread_t tid = rt_thread_create("upgrade",up_serv_task, NULL,4*1024,2, 5);
	if (tid != RT_NULL) rt_thread_startup(tid);

	return 0;
}

INIT_ENV_EXPORT(up_serv_thread);
