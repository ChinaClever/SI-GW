/*
 * netDataPacket.c
 * �������ݰ�
 * 	�ӿڣ�int data_msg_packetSent(data_code *code,uchar *buf, ushort len)
 * 	˵�������ݴ����Э���ʽ�������ͳ�ȥ
 *
 *	�ӿڣ�int data_msg_analytic(uchar *buf, ushort len, data_packet *msg)
 *	˵�������ݰ���������ԭ��data_packet�ṹ��
 *
 *  Created on: 2016��7��29��
 *      Author: Lzy
 */
#include "netDataPacket.h"

static uchar gSentBuf[UDP_BUF_SIZE]={0};


/**
 * ��  �ܣ�����У����
 * ��ڲ�����pkt->���ṹ��
 * ����ֵ��У����
 */
uchar data_packet_xor(uchar *buf, ushort len)
{
    ushort i;
    uchar XOR=0;

    for(i=0; i<len; ++i)
        XOR += buf[i];

    return XOR;
}


/**
 * ��  �ܣ����ݼ����ͷ��������
 * ��ڲ�����pkt->���ṹ��
 * ����ֵ��
 */
static void data_packet_head(data_packet *pkt)
{
    pkt->hdr = DATA_MSG_HDR;	/*��Ϣͷ*/
    pkt->stx = DATA_MSG_STX; /*��ʶ��*/

    pkt->ED = DATA_MSG_ED; /*������*/
}

/**
 * ���ܣ�������Ŷ�����
 * ��ڲ�����msg->���ṹ��
 * ���ڲ�����buf->���ݰ��׵�ַ
 * ����ֵ�����ݰ�����
 */
static int data_packet_code(data_code *msg, uchar *buf)
{
    ushort i,rtn=DATA_MSG_CODE_SIZE; /*������ֽ���Ϊ9*/
    uchar *ptr = buf;

    /*������Ŷ�*/
    for(i=0; i<DEV_CODE_SIZE; ++i)
        *(ptr++) = msg->devCode[i];	/*�豸��*/

    *(ptr++) = msg->type;	/*ͨѶ����*/
    *(ptr++) = msg->version; /*�汾��*/
    *(ptr++) = msg->trans; /*���䷽��*/

    msg->reserve = 0;
    memcpy(ptr, &(msg->reserve),sizeof(ushort)); /*Ԥ����*/

    return rtn;
}

/**
 * ���ܣ�����������
 * ��ڲ�����msg->���ṹ��
 * ���ڲ�����buf->���ݰ��׵�ַ
 * ����ֵ�����ݰ�����
 */
static int data_packet_domain(data_packet *msg, uchar *buf)
{
    ushort i,rtn=0;
    uchar *ptr = buf;

    /*�������ݳ���*/
    *(ptr++) = ((msg->len) >> 8); /*���ȸ�8λ*/
    *(ptr++) = (0xff)&(msg->len); /*��8λ*/

    for(i=0; i<msg->len; ++i)
        *(ptr++) = msg->data[i];

    msg->XOR = data_packet_xor(msg->data, msg->len);	/*����У����*/
    *(ptr++) = msg->XOR;

    rtn = 2+ msg->len + 1;
    return rtn;
}

/**
 * ���ܣ����ݴ��
 * ��ڲ�����pkt->���ṹ��
 * ���ڲ�����buf->���ݰ��׵�ַ
 * ����ֵ�����ݰ�����
 * ˵������Ҫ��������ʱ�ͻ���ô˺��������ݴ��
 */
static int data_msg_packet(data_packet *pkt, uchar *buf)
{
	ushort rtn=0;
	uchar *ptr = buf;

	data_packet_head(pkt); /*��д��ͷ����β*/
	*(ptr++) = pkt->hdr;  /*��Ϣͷ*/
	*(ptr++) = pkt->stx; /*��ʶ��*/

	ptr += data_packet_code(&(pkt->code),ptr); /*������Ŷ�����*/
	ptr += data_packet_domain(pkt, ptr); /*��������*/

	*(ptr++) = pkt->ED; /*������*/
	rtn = 2 + DATA_MSG_CODE_SIZE + 2 + pkt->len + 1 + 1;

    return rtn;
}

/**
 * ���ܣ����ݷ���
 * ��ڲ�����data_code -> ���Ŷι��� 	buf->�����׵�ַ len ->���ݳ���
 * ����ֵ�����ݰ�����
 * ˵������Ҫ��������ʱ�ͻ���ô˺�����������
 */
int data_msg_packetSent(data_code *code,uchar *buf, ushort len)
{
	static data_packet msg;

	msg.code = *code;
	msg.len = len;
	msg.data = buf;

	memset(gSentBuf,0,sizeof(gSentBuf));
	len = data_msg_packet(&msg, gSentBuf); // ���ݴ��
	len = udp_clientSentData(gSentBuf,len);

	return len;
}


/**
 * ��  �ܣ�ͬ�����ݰ� ������ݰ��Ƿ�����
 * ��ڲ�����pkt->���ṹ��
 * ����ֵ��ͬ��ͷ��λ�ã�   -2 ͬ��ͷ���� 	-3����������
 */
static int data_packet_sync(uchar *buf, ushort len)
{
    int ret=0,rtn = -1;
    uchar head[2] = {DATA_MSG_HDR, DATA_MSG_STX}; /*��ͷ*/
    ushort offset=0;

    if(len < 2+DATA_MSG_CODE_SIZE+3) /*���ݰ�������*/
        return DATA_ERR_ABNORMAL;

    /*����ͬ��ͷ*/
    do
    {
        if(offset < len-2)
        {
            ret = memcmp(buf+offset, head, sizeof(head)); /*���Ұ�ͷ*/
            if(ret!=0) /*���ǰ�ͷ*/
                offset++; /*ƫ������*/
        }
        else
            return DATA_ERR_HEAD; /*ͬ��ͷ����*/
    } while(ret!=0);

    /*��֤��β*/
    rtn = offset + 2 + DATA_MSG_CODE_SIZE; /*���Ŷν���λ��*/
    rtn += (buf[rtn]<<8) + buf[rtn+1]; /*���ݳ���*/
    rtn += 2+1; /*���ݳ��ȶ��ֽڡ�У��λһ�ֽ�*/
    if(rtn < len)
    {
        if(buf[rtn] == DATA_MSG_ED) /**/
            return offset; /*����ͷλ��*/
    }

    return DATA_ERR_END;/*������δ�ҵ�*/
}

/**
 * ���ܣ��������Ŷ�����
 * ��ڲ�����buf->�����׵�ַ
 * ���ڲ�����code->���֮��MSG
 * ����:����
 */
static int data_packet_getCode(uchar *buf, data_code *code)
{
    uchar i,*ptr=buf;

    for(i=0; i<DEV_CODE_SIZE; ++i)
        code->devCode[i] = *(ptr++); /*�豸����*/

    code->type = *(ptr++); /*ͨѶ����*/
    code->version = *(ptr++); /*�汾��*/
    code->trans = *(ptr++); /*����˷��ͱ�־��10H �ͻ���Ӧ���־��03H */
    code->reserve = 0; //(*(ptr++)) * 256;
    code->reserve += 0;//*(ptr++);

    return DATA_MSG_CODE_SIZE;
}

/**
 * ��  �ܣ��������ݶ�����
 * ��ڲ�����buf->�����׵�ַ
 * ���ڲ�����msg->���֮��MSG
 * ��  ��:���� 	-1 ����
 */
static int data_packet_getDomain(uchar *buf, data_packet *msg)
{
    ushort len=0;
    uchar XOR,*ptr = buf;

    len = (ptr[0]<<8) + ptr[1]; /*��ȡ���ݳ���*/
    msg->len = len;
    ptr += 2;

    if(len > 0)
    {
        msg->data = ptr; /* ���ݶ�*/
        ptr += len;
    }

    msg->XOR = *(ptr++);
    XOR = data_packet_xor(msg->data, len);
    if(XOR != msg->XOR)
        return DATA_ERR_XOR;

    return (2+len+1);
}


/**
 * ���ܣ��������ݰ�
 * ��ڲ�����buf->�����׵�ַ 	len->���ݳ���
 * ���ڲ�����pkt->���֮��MSG
 * ����:TRUE
 * ˵�������յ����ݰ�ʱ�ͻ���ô˺��������ݽ����MSG��ʽ
 */
int data_msg_analytic(uchar *buf, ushort len, data_packet *msg)
{
    int ret;
    uchar *ptr=buf;

    ret = data_packet_sync(buf,len);
    if(ret >= 0)
    {
        ptr = buf+ret;
        msg->hdr = *(ptr++); /*��Ϣͷ*/
        msg->stx = *(ptr++); /*��ʶ��*/

        ptr += data_packet_getCode(ptr, &(msg->code)); /*��ȡ���Ŷ�����*/
        ret = data_packet_getDomain(ptr, msg);	/*��ȡ���ݶ�*/
        if(ret >= 0)
        {
            ptr += ret;
            msg->ED = *ptr;

            return (2+DATA_MSG_CODE_SIZE+2+msg->len+2); /*�������ĳ���*/
        }
    }
    else
    	udp_printf("data packet sync error %d\n",ret);

    return ret;
}
