/*
 * web_aliiot.c
 *
 *  Created on: 2019��7��2��
 *      Author: luozhiyong
 */
#include "web_aliiot.h"

#ifdef PKG_USING_ALI_IOTKIT

void web_aliiot_get(struct webnet_session* session)
{
	char *ptr=pWebQuest;
	struct webnet_request* request = session->request;
	const char *id_str = webnet_request_get_query(request, "id");

	int id = atoi(id_str);
	sAli *t = ali_cfg_get(id);

	ptr[0] = 0;
	sprintf(ptr, "id=%d", t->id); ptr += strlen(ptr);
	sprintf(ptr, "&en=%d", t->en); ptr += strlen(ptr);
	sprintf(ptr, "&product_key=%s",t->key); ptr += strlen(ptr);
	sprintf(ptr, "&product_secret=%s",t->pro_secret); ptr += strlen(ptr);
	sprintf(ptr, "&device_name=%s", t->name); ptr += strlen(ptr);
	sprintf(ptr, "&device_secret=%s",t->dev_secret); ptr += strlen(ptr);

	webnet_session_printf(session, "%s", pWebQuest);
}

void web_aliiot_set(struct webnet_session* session)
{
	struct webnet_request* request = session->request;
	const char *id_str = webnet_request_get_query(request, "id");
	const char *en_str = webnet_request_get_query(request, "en");
	const char *key_str = webnet_request_get_query(request, "product_key");
	const char *secret_str = webnet_request_get_query(request, "product_secret");
	const char *name_str = webnet_request_get_query(request, "device_name");
	const char *devsecret_str = webnet_request_get_query(request, "device_secret");

	int id = atoi(id_str);
	struct sAli *t = ali_cfg_get(id);
	t->en = atoi(en_str);
	sprintf(t->name, name_str);
	sprintf(t->key, key_str);
	sprintf(t->pro_secret, secret_str);
	sprintf(t->dev_secret, devsecret_str);
	//ali_cfg_set();

	static const char* status = "id=%d&en=%d&product_key=%s&product_secret=%s&device_name=%s&device_secret=%s";
	webnet_session_printf(session, status,t->id, t->en,t->key,t->pro_secret, t->name,t->dev_secret);
}


void web_aliiot(void)
{
	webnet_cgi_register("getAliIot", web_aliiot_get);
	webnet_cgi_register("setAliIot", web_aliiot_set);
}

#endif
