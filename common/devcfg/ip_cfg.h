/*
 * ip_cfg.h
 *
 *  Created on: 2019��7��4��
 *      Author: luozhiyong
 */

#ifndef BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_DEVCFG_IP_CFG_H_
#define BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_DEVCFG_IP_CFG_H_
#include "user_cfg.h"

typedef struct sNetAddr {
	boolean dhcp;
	boolean ipV4;
	boolean isUpload; // �����Ƿ��ϴ�

	char ip[20]; // IP��ַ
	char gw[20]; // ����
	char mask[20]; //��������
	char dns[20]; // DNS
	char mac[20]; // mac��ַ
}sNetAddr;

sNetAddr *ip_cfg_get(void);
void ip_cfg_set(void);

#endif /* BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_DEVCFG_IP_CFG_H_ */