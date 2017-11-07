//
//  robomodule_due_CAN.h
//  
//
//  Created by 朝力�?on 2016/11/26.
//
//

#ifndef robomodule_due_CAN_h
#define robomodule_due_CAN_h
#include "variant.h"
#include "due_can.h"
#endif /* robomodule_due_CAN_h */

class CRobomodule_due_CAN
{
private:
	int driver_group;
	int driver_number;
	int driver_mode;

	CAN_FRAME outgoing;
	void resetdriver(int group, int number);
	void setmode(int group, int number, int mode);
public:
	CRobomodule_due_CAN();
	void initdriver(int baud, int group, int number, int mode);
	void positionwheel(int temp_pwm, int temp_positionwheel, int group, int number);
	void speedwheel(int temp_pwm, int temp_velocity, int group, int number);
	void speedwheel(int temp_pwm, int group, int number);
};
