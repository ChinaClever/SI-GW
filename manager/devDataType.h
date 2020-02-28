/*
 * devDataType.h
 *
 *  Created on: 2016��8��1��
 *      Author: lzy
 */

#ifndef DEVDATA_DEVDATATYPE_H_
#define DEVDATA_DEVDATATYPE_H_
#include "netDataSegment.h"

#define NET_ARRAY_SIZE  PACK_ARRAY_SIZE

/*���ݵ�Ԫ�ṹ��*/
typedef struct
{
	ushort		value[NET_ARRAY_SIZE]; // ��ǰֵ

    ushort 		min[NET_ARRAY_SIZE]; //��Сֵ
    ushort 		max[NET_ARRAY_SIZE]; //���ֵ
    uchar 		*alrm; // ����״̬

    ushort 		*crMin; //�ٽ���Сֵ
    ushort 		*crMax; //�ٽ����ֵ
    uchar 		*crAlrm; //�ٽ籨��״̬
}_devDataUnit;

typedef struct
{
	ushort 		len; // ����
	_devDataUnit		vol; // ����
	_devDataUnit 		cur; //��ѹ

	uint 		pow[NET_ARRAY_SIZE]; // ����
    uint 		ele[NET_ARRAY_SIZE]; // ����

    ushort 		pf[NET_ARRAY_SIZE]; //��������
    uchar 		sw[NET_ARRAY_SIZE]; //����״̬

    ushort		*carbon; // ��C��
    ushort 		*rate; //��ѹƵ��
}_devDataObj;

typedef struct
{
	uchar 		len; // ����
	_devDataUnit		tem; // �¶�
	_devDataUnit 		hum; //ʪ��
    uchar 		*door; // �Ž�
    uchar 		*water; // ˮ��
    uchar 		*smoke; //����
}_envDataObjct;

typedef struct
{
	_devDataObj		    line; // ������
	_devDataObj 		output; //���λ
	_envDataObjct 		env; // ����
}pduDevData;


#endif /* DEVDATA_DEVDATATYPE_H_ */