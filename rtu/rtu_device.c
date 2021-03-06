/*
 * rtus.c
 *
 *  Created on: 2019年10月21日
 *      Author: luozhiyong
 */
#include "rtu_device.h"

static int rtu_write(sRtu *rtu)
{
	//rt_pin_write(rtu->pin, PIN_LOW); msleep(1);
	int ret = rt_device_write(rtu->dev, 0, rtu->sentBuf, rtu->sentLen);
	if(ret > 0) {
		// msleep(UART_DELAY); rt_pin_write(rtu->pin, PIN_HIGH);
		rtu->recvLen = 0;
		//rtu->sentLen = 0;
	}

	return ret;
}

static int rtu_read(rt_device_t dev, uchar *buf)
{
	int ret, rtn=0;

	do {
		ret = rt_device_read(dev, 0, buf, UART_RECV_SIZE); //RT_SERIAL_RB_BUFSZ;
		if(ret > 0) {
			buf[ret] = 0;
			buf += ret;
			rtn += ret;
			msleep(UART_DELAY);
		}
	} while(ret > 0);

	return rtn;
}

static int rtu_recv(sRtu *rtu, uchar ms)
{
	short i, rtn=0, delay_ms=ms*UART_TIMEOUT/UART_DELAY;

	for(i=0; i<delay_ms; ++i) {
		rtn = rtu_read(rtu->dev, rtu->recvBuf);
		if(rtn > 0) {
			rtu->recvLen = rtn;
			break;
		} else {
			msleep(UART_DELAY);
		}
	}

	return rtn;
}



static void rtu_lock(sRtu *rtu)
{
	while(rtu->lock) {
		msleep(UART_DELAY);
	}
	rtu->lock = true;
}

static void rtu_unlock(sRtu *rtu)
{
	//msleep(UART_DELAY);
	rtu->lock = false;
}

int rtu_trans(sRtu *rtu)
{
	rtu_lock(rtu);
	int ret = rtu_write(rtu);
	if(ret > 0) {
		ret = rtu_recv(rtu, 1);
	}
	rtu_unlock(rtu);

	return ret;
}

int rtu_sent(sRtu *rtu)
{
	rtu_lock(rtu);
	int ret = rtu_write(rtu);
	if(ret > 0) {
		ret = rtu_recv(rtu, 3);
	}
	rtu_unlock(rtu);

	return ret;
}


rt_err_t rtu_control(rt_device_t dev)
{
	struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 配置参数 */

	config.baud_rate = BAUD_RATE_9600;
	config.data_bits = DATA_BITS_8;
	config.stop_bits = STOP_BITS_1;
	config.parity = PARITY_NONE;

	return rt_device_control(dev, RT_DEVICE_CTRL_CONFIG, &config);
}

rt_device_t rtu_open(char *name)
{
	char rtu_name[RT_NAME_MAX];
	rt_strncpy(rtu_name, name, RT_NAME_MAX);

	rt_device_t  serial = rt_device_find(rtu_name);  /* 查找串口设备 */
	if (serial) {
		rt_device_open(serial, RT_DEVICE_FLAG_DMA_RX); /* 以 DMA 接收及轮询发送方式打开串口设备 */
		//rt_device_set_rx_indicate(serial, rtu_input); /* 设置接收回调函数 */
		rtu_control(serial);
	} else {
		rt_kprintf("find %s failed!\n", rtu_name);
		return NULL;
	}

	return serial;
}





