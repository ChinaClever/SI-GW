/*
 * modbus_read.c
 *
 *  Created on: 2019年10月24日
 *      Author: luozhiyong
 */
#include "modbus_read.h"


ushort *mb_read_Currentdata(ushort *reg, sDataUnit *unit,uchar size)
{
	for(uchar i=0; i<size; ++i) {
	*reg++ = unit->value[i];
	}
	return reg;
}

ushort *mb_read_apPowdata(ushort *reg, sObjData *obj)
{
	for(uchar i=0; i<obj->size; ++i) {
	*reg++ = obj->apPow[i];
	}
	return reg;
}

ushort *mb_read_eledata(ushort *reg, sObjData *obj)
{
	for(uchar i=0; i<obj->size; ++i) {
	*reg++ = (obj->ele[i] >> 16);
	*reg++ = (obj->ele[i]&0xffff);
	}
	return reg;
}

ushort *mb_read_minmaxdata(ushort *reg, sDataUnit *unit,uchar size)
{
	for(uchar i=0; i<size; ++i) {
	*reg++ = unit->min[i];
	*reg++ = unit->max[i];
	}
	return reg;
}

ushort *mb_read_maxmindata(ushort *reg, sDataUnit *unit,ushort size)
{
	for(ushort i=0; i<size; ++i) {
		*reg = unit->max[i]<<8;
		*reg = *reg + unit->min[i];
		reg++;
	}
	return reg;
}

ushort *mb_read_powData(ushort *reg, sObjData *obj,ushort size)
{
	for(ushort i=0; i<size; ++i) {
	*reg++ =obj->pow[i];
	}
	return reg;
}

ushort *mb_read_pfswData(ushort *reg, sObjData *obj,ushort size)
{
	if(size == 1){
		*reg = obj->pf[0]<<8;
		*reg = *reg + obj->sw[0];
		reg++;
	}else if(size == 3){
		*reg = obj->pf[0]<<8;
		*reg = *reg + obj->pf[1];
		reg++;
		*reg = obj->pf[2]<<8;
		*reg = *reg + obj->sw[0];
		reg++;
		*reg = obj->sw[1]<<8;
		*reg = *reg + obj->sw[2];
		reg++;
	}
	return reg;
}

ushort *mb_read_objData(ushort *reg, sObjData *obj)
{
	uchar size = obj->size;

	reg = mb_read_Currentdata(reg, &(obj->vol),size);
	reg = mb_read_Currentdata(reg, &(obj->cur),size);

	reg = mb_read_apPowdata(reg, obj);
	reg = mb_read_eledata(reg, obj);

	reg = mb_read_minmaxdata(reg, &(obj->vol),size);
	reg = mb_read_minmaxdata(reg, &(obj->cur),size);

	//*reg++ = obj->sw[i];
	//*reg++ = obj->pow[i];
	//*reg++ = obj->pf[i];
	return reg;
}

ushort *mb_read_envData(ushort *reg, sEnvData *env)
{
	ushort i;
	env->size = 1;
	uchar size = env->size;

	for(i=0; i<size; ++i) {
		*reg = (&(env->tem))->value[0];
		*reg = *reg<<8;
		*reg = *reg + (&(env->hum))->value[0];
		reg++;
	}
	
	reg = mb_read_maxmindata(reg,&(env->tem),size);
	reg = mb_read_maxmindata(reg,&(env->hum),size);

	return reg;
}


int mb_read_packet(uchar addr, modbus_mapping_t *mb)
{
	//ushort *ptr = &(mb->tab_registers[2]);
	ushort *ptr = &(mb->tab_registers[0]);
	sDataPacket *packet = data_packet_get(addr);

	if(packet->offLine) {
		sObjData *line = &(packet->data.line); // 相数据
		sEnvData *env =  &(packet->data.env);
		ptr = mb_read_objData(ptr, line);
		ptr = mb_read_envData(ptr, env);

		*ptr = packet->devSpec;
		*ptr = *ptr<<8;
		*ptr = *ptr + packet->br;
		ptr++;
		ptr = mb_read_powData(ptr, line, line->size);

		ptr = mb_read_pfswData(ptr, line, line->size);

		if(packet->dc) {
			ptr++;
			*ptr = packet->hz;
		} else {
			*ptr = line->pl[0];
		}
	}

	return packet->offLine;
}


