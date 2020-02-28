/*
 * json_net_recv.c
 *
 *  Created on: 2019��10��12��
 *      Author: luozhiyong
 */
#include "json_net_recv.h"






boolean json_recv_array(cJSON *json)
{
	boolean ret = true;
	cJSON *arr_node = cJSON_GetObjectItem(json, "line_vol_items");
	if(arr_node->type == cJSON_Array)
	{
		int i, size = cJSON_GetArraySize(arr_node);
		cJSON *arr_item = arr_node->child;// ��ȡ��������ӽڵ� �Ӷ���
		for(i=0; i<size; ++i){
			int id = cJSON_GetObjectItem(arr_item,"id")->valueint;
			double min = cJSON_GetObjectItem(arr_item,"min")->valuedouble;
			double max = cJSON_GetObjectItem(arr_item,"max")->valuedouble;

			arr_item = arr_item->next;//��һ���Ӷ���
			//setSwStatus(id-1, v);
		}
	} else {
		ret = false;
	}

	return ret;
}

boolean json_analysis(char *str)
{
	cJSON *json = cJSON_Parse(str);  //�ӻ������н�����JSON�ṹ
	boolean ret = json_recv_array(json);
	cJSON_Delete(json); //��JSON�ṹ��ռ�õ����ݿռ��ͷ�
	return ret;
}


