/*
 * json_net_recv.c
 *
 *  Created on: 2019年10月12日
 *      Author: luozhiyong
 */
#include "json_net_recv.h"
#include "rtu_setcmd.h"

typedef struct _sJsonCmd
{
	int dev_num, pduType, dev_spec;
	char *ip, *dev_type, *dev_name;

	int id, fn;
	double min, max;
}sJsonCmd;


static char *json_getString(cJSON *json, const char *key)
{
	char *res = NULL;
	cJSON* item = cJSON_GetObjectItem(json, key);
	if(item && (item->type == cJSON_String)) {
		res = item->valuestring;
	}

	return res;
}

static int json_getInt(cJSON *json, const char *key)
{
	int ret = -1;
	cJSON* item = cJSON_GetObjectItem(json, key);
	if(item && (item->type == cJSON_Number)) {
		ret = item->valueint;
	}

	return ret;
}

static double json_getDouble(cJSON *json, const char *key)
{
	double ret = -1;
	cJSON* item = cJSON_GetObjectItem(json, key);
	if(item && (item->type == cJSON_Number)) {
		ret = item->valuedouble;
	}

	return ret;
}



bool json_recv_company(cJSON *json)
{
	bool ret = true;
	char *strName = "CLEVER";
	const char *str = json_getString(json, "company");
	ret = strncmp(str, strName, strlen(strName));
	if(ret) ret = false;

	return ret;
}


bool json_recv_version(cJSON *json)
{
	bool ret = json_recv_company(json);
	if(ret) {
		int v = json_getInt(json, "version");
		if(v == JSON_VERSION) ret = true;
	}

	return ret;
}


bool json_recv_headInfo(cJSON *object, sJsonCmd *cmd)
{
	bool ret = false;
	char *str = "SI-PDU";
	cJSON *obj = cJSON_GetObjectItem(object, "head_info");
	if(obj) {
		cmd->ip = json_getString(obj, "ip");
		cmd->dev_type = json_getString(obj, "dev_type");
		cmd->dev_num = json_getInt(obj, "dev_num");
		cmd->dev_name = json_getString(obj, "dev_name");

		ret = strncmp(cmd->dev_type, str, strlen(str));
		if(0 == ret) {
			ret = true;
		}
	}

	return ret;
}




bool json_recv_sentData(sJsonCmd *cmd)
{
	if((cmd->max<0) ||(cmd->min<0)) return false;

	switch(cmd->fn) {
	case 1: rtu_setVolCmd(cmd->dev_num, cmd->id, cmd->min, cmd->max); break;
	case 2: rtu_setCurCmd(cmd->dev_num, cmd->id, cmd->min, cmd->max); break;
	case 3: rtu_setTemCmd(cmd->dev_num, cmd->min, cmd->max); break;
	case 4: rtu_setHumCmd(cmd->dev_num, cmd->min, cmd->max); break;
	}

	return true;
}



bool json_recv_itemValues(cJSON *obj, sJsonCmd *cmd, const char *key)
{
	char buf[24] = {0};
	cmd->id = json_getInt(obj, "id");

	sprintf(buf, "%s_min", key);
	cmd->min = json_getDouble(obj, key);

	sprintf(buf, "%s_max", key);
	cmd->max = json_getDouble(obj, key);

	return json_recv_sentData(cmd);
}


bool json_recv_lineItem(cJSON *obj, sJsonCmd *cmd)
{
	cmd->fn = 1;
	json_recv_itemValues(obj,cmd, "vol");

	cmd->fn = 2;
	json_recv_itemValues(obj,cmd, "cur");

	return true;
}




bool json_recv_lineItemList(cJSON *object, sJsonCmd *cmd)
{
	cJSON *arr_node = cJSON_GetObjectItem(object, "line_item_list");
	if(arr_node->type == cJSON_Array) {
		int i, size = cJSON_GetArraySize(arr_node);
		cJSON *arr_item = arr_node->child;// 获取数组对象孩子节点 子对象
		for(i=0; i<size; ++i){
			json_recv_lineItem(arr_item, cmd);
			arr_item = arr_item->next;//下一个子对象
		}
	}

	return true;
}





bool json_recv_temItemList(cJSON *object, sJsonCmd *cmd)
{
	cmd->fn = 3;
	cJSON *arr_node = cJSON_GetObjectItem(object, "tem_item_list");
	if(arr_node->type == cJSON_Array) {
		int size = cJSON_GetArraySize(arr_node);
		if(size > 0) {
			cJSON *arr_item = cJSON_GetArrayItem(arr_node,0);
			json_recv_itemValues(arr_item, cmd, "tem");
		}
	}

	return true;
}


bool json_recv_humItemList(cJSON *object, sJsonCmd *cmd)
{
	cmd->fn = 4;
	cJSON *arr_node = cJSON_GetObjectItem(object, "hum_item_list");
	if(arr_node->type == cJSON_Array) {
		int size = cJSON_GetArraySize(arr_node);
		if(size > 0) {
			cJSON *arr_item = cJSON_GetArrayItem(arr_node,0);
			json_recv_itemValues(arr_item, cmd, "hum");
		}
	}

	return true;
}


bool json_recv_envInfo(cJSON *object, sJsonCmd *cmd)
{
	cJSON *obj = cJSON_GetObjectItem(object, "env_info");
	json_recv_temItemList(obj, cmd);
	json_recv_humItemList(obj, cmd);

	return true;
}



boolean json_analysis(char *str)
{
	sJsonCmd cmd;
	boolean ret = true;
	cJSON *json = cJSON_Parse(str);  //从缓冲区中解析出JSON结构
	if(json) {
		ret = json_recv_version(json);
		if(ret) {
			json_recv_lineItemList(json, &cmd);
			json_recv_envInfo(json, &cmd);
		}
		cJSON_Delete(json); //将JSON结构所占用的数据空间释放
	}

	return ret;
}



