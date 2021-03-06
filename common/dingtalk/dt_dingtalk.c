/*
 * dt_dingtalk.c
 *
 *  Created on: 2019��11��20��
 *      Author: luozhiyong
 */
#include "dt_dingtalk.h"

static int dt_post(char *url, char *data)
{
	char *head = "Content-Type: application/json; charset=utf-8\r\n";
	return http_post(url, data, head);
}


void dt_talk(char *data)
{
	char *fmt =	"{\"msgtype\":\"text\",\"text\":{\"content\":\"[%s]%s\"},\"at\":{\"atMobiles\":[\"%s\"],\"isAtAll\":%s}}";

	sDtCfg *cfg = dt_cfg_get();
	if(cfg->en) {
		char text[256] = {0};
		char *isAtAll = "false";
		if(cfg->isAtAll) isAtAll = "true";
		sprintf(text, fmt, cfg->key, data, cfg->mobile, isAtAll);
		dt_post(cfg->webhook, text);
	}
}
