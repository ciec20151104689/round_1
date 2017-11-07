//
//  robomodule_due_CAN.cpp
//  
//
//  Created by ≥Ø¡¶√» on 2016/11/26.
//
//

#include "robomodule_due_CAN.h"
CRobomodule_due_CAN::CRobomodule_due_CAN()
{
	driver_group = 0;
	driver_number = 0;
	driver_mode = 3;
}
void CRobomodule_due_CAN::resetdriver(int group = 0, int number = 0)
{
	outgoing.id = ((uint32_t)group) << 8 | (uint32_t)((number << 4) & 0xf0);
	outgoing.length = 8;
	outgoing.extended = false;
	outgoing.priority = 4; //0-15 lower is higher priority
	outgoing.data.high = 0x55555555;
	outgoing.data.low = 0x55555555;
	Can0.sendFrame(outgoing);
	delay(500);
}
void CRobomodule_due_CAN::setmode(int group = 0, int number = 0, int mode = 4)
{
	//CAN_FRAME outgoing;
	//outgoing.id = temp;//0x021;
	outgoing.id = ((uint32_t)group) << 8 | (uint32_t)(((number << 4) | 0x01) & 0xff1);
	outgoing.extended = false;
	outgoing.length = 8;
	outgoing.priority = 4; //0-15 lower is higher priority
	outgoing.data.high = 0x55555555;
	outgoing.data.low = 0x55555555;
	outgoing.data.bytes[0] = mode;
	Can0.sendFrame(outgoing);
	delay(100);
}

void CRobomodule_due_CAN::speedwheel(int temp_pwm, int temp_velocity, int group = 0, int number = 0)
{
	//CAN_FRAME outgoing;
	outgoing.id = ((uint32_t)group) << 8 | (uint32_t)(((number << 4) | 0x04) & 0xff4);
	outgoing.extended = false;
	outgoing.length = 8;
	outgoing.priority = 4; //0-15 lower is higher priority
	outgoing.data.high = 0x55555555;
	outgoing.data.low = 0x55555555;
	outgoing.data.bytes[0] = (char)((temp_pwm >> 8) & 0xff);
	outgoing.data.bytes[1] = (char)(temp_pwm & 0xff);
	outgoing.data.bytes[2] = (char)((temp_velocity >> 8) & 0xff);
	outgoing.data.bytes[3] = (char)(temp_velocity & 0xff);
	Can0.sendFrame(outgoing);
}

void CRobomodule_due_CAN::positionwheel(int temp_pwm, int temp_position, int group = 0, int number = 0)
{
	//CAN_FRAME outgoing;
	outgoing.id = ((uint32_t)group) << 8 | (uint32_t)(((number << 4) | 0x05) & 0xff5);
	outgoing.extended = false;
	outgoing.length = 8;
	outgoing.priority = 4; //0-15 lower is higher priority
	outgoing.data.high = 0x55555555;
	outgoing.data.low = 0x55555555;
	outgoing.data.bytes[0] = (char)((temp_pwm >> 8) & 0xff);
	outgoing.data.bytes[1] = (char)(temp_pwm & 0xff);
	outgoing.data.bytes[2] = 0x55;
	outgoing.data.bytes[3] = 0x55;
	outgoing.data.bytes[4] = (char)((temp_position >> 24) & 0xff);
	outgoing.data.bytes[5] = (char)((temp_position >> 16) & 0xff);
	outgoing.data.bytes[6] = (char)((temp_position >> 8) & 0xff);
	outgoing.data.bytes[7] = (char)(temp_position & 0xff);
	Can0.sendFrame(outgoing);
}

void CRobomodule_due_CAN::speedwheel(int temp_pwm, int group = 0, int number = 0)
{
	//CAN_FRAME outgoing;
	outgoing.id = ((uint32_t)group) << 8 | (uint32_t)(((number << 4) | 0x02) & 0xff2);
	outgoing.extended = false;
	outgoing.length = 8;
	outgoing.priority = 4; //0-15 lower is higher priority
	outgoing.data.high = 0x55555555;
	outgoing.data.low = 0x55555555;
	outgoing.data.bytes[0] = (char)((temp_pwm >> 8) & 0xff);
	outgoing.data.bytes[1] = (char)(temp_pwm & 0xff);
	Can0.sendFrame(outgoing);
}
void CRobomodule_due_CAN::initdriver(int baud = CAN_BPS_1000K, int group = 0, int number = 0, int mode = 3)
{
	Can0.begin(baud);
	Can0.watchFor();
	resetdriver(group, number);
	setmode(group, number, mode);
}

