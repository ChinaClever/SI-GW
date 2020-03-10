/*
 * datasegment.c
 * ���ݶδ�������
 *
 *
 *  Created on: 2016��7��29��
 *      Author: Lzy
 */
#include "netDataSegment.h"

static uchar gSentBuf[UDP_BUF_SIZE]={0};


/**
 *
 */
void data_dev_code(data_code *code)
{
	int i=0;
	for(i=0; i<DEV_CODE_SIZE; ++i) {
		if(i==1) {
			code->devCode[i] = 0;
		} else  {
			code->devCode[i] = 1; /*�豸����*/
		}
	}

	code->type = TRA_TYPR_UDP; /*ͨѶ����*/
	code->version = DATA_DEV_VERSION; /*�汾��*/
	code->trans = DATA_MSG_CLIENT; /*����˷��ͱ�־��10H �ͻ���Ӧ���־��03H */
	code->reserve = 0;
}

/**
 * ���ܣ��豸���ݴ��
 * ��ڲ�����pkt->���ṹ��
 * ���ڲ�����buf->���ݰ��׵�ַ
 * ����ֵ�����ݰ�����
 * ˵������Ҫ��������ʱ�ͻ���ô˺��������ݴ��
 */
static int msg_packet(dev_data_packet *pkt, uchar *buf)
{
    ushort i,rtn=0;
    uchar *ptr = buf;

    *(ptr++) = pkt->addr; 	/*�豸��*/

    *(ptr++) = pkt->fn[0]; /*������*/
    *(ptr++) = pkt->fn[1];

    /*���ݳ���*/
    *(ptr++) = (pkt->len >> 8); /*�߰�λ*/
    *(ptr++) = (pkt->len) & 0xFF; /*�Ͱ�λ*/

    for(i=0; i<pkt->len; ++i)
    	 *(ptr++) = pkt->data[i];
    rtn = 1 + 2 + 2 + pkt->len;

    return rtn; /*���ݰ�����*/
}


/**
 *  �������ݺ���
 */
int data_packet_sent(dev_data_packet *msg)
{
	ushort rtn=0;
	static data_code devCode; /*���Ŷ�*/
	data_dev_code(&devCode);

	memset(gSentBuf,0,sizeof(gSentBuf));
	rtn = msg_packet(msg,gSentBuf);
	rtn = data_msg_packetSent(&devCode, gSentBuf, rtn);

	return rtn;
}


/**
 * @brief  �������
 * @param code
 * @return
 */
static int dev_code_check(data_code *code)
{
	int i;
   for(i=0; i<DEV_CODE_SIZE; ++i)
    {
        if( i==1 && code->devCode[i] != 0)
            return false;

        else if(i!=1 && code->devCode[i] != 1) /*�豸����*/
            return false;
    }

    if((code->type != TRA_TYPR_UDP) && (code->type != TRA_TYPR_TCP))/*ͨѶ����*/
        return false;

    if(code->version != DATA_DEV_VERSION) /*�汾�ż��*/
        return false;

    if(code->trans != DATA_MSG_SERVICE) /*���䷽��*/
        return false;

    return true;
}


/**
 * ���ܣ��������ݰ�
 * ��ڲ�����buf->�����׵�ַ 	len->���ݳ���
 * ���ڲ�����pkt->���֮��MSG
 * ����:TRUE
 */
static int msg_analytic(uchar *buf, ushort len, dev_data_packet *pkt)
{
    uchar *ptr=buf;

    pkt->addr = *(ptr++);/*��ȡԴ��ַ��*/

    pkt->fn[0] = *(ptr++); /*������*/
    pkt->fn[1] = *(ptr++);

    pkt->len = (*ptr) * 256 + *(ptr+1); /*���ݳ���*/
    ptr += 2;

    if(pkt->len > 0) /*������*/
        pkt->data = ptr;

    return pkt->len;
}


/**
 * ���ܣ��������ݰ�
 * ��ڲ�����buf->�����׵�ַ 	len->���ݳ���
 * ���ڲ�����pkt->���֮��MSG
 * ����:TRUE
 * ˵�������յ����ݰ�ʱ�ͻ���ô˺��������ݽ����MSG��ʽ
 */
int data_packet_analytic(uchar *buf, ushort len, dev_data_packet *pkt)
{
    static data_packet msg;
    int ret = data_msg_analytic(buf,len,&msg);
    if(ret > 0)
    {
        ret = dev_code_check(&msg.code); /*���Ŷ��ж�*/
        if(ret == true)
             ret = msg_analytic(msg.data, msg.len, pkt);
        else
        	udp_printf("dev code error\n");
        return ret;
    }
    else
    	udp_printf("dev data analytic error\n");

    return false;
}
