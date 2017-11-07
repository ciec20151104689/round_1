#include"robomodule_due_CAN.h"
CRobomodule_due_CAN can;

unsigned int echowallleft = 3; //��Arduino��Pin3������US-100��Echo/RX
unsigned int trigwallleft = 2; //��Arduino��Pin2������US-100��Trig/TX
unsigned int trigwallright = 4;
unsigned int echowallright = 5;
unsigned int trigwallfront_left = 8;
unsigned int echowallfront_left = 9;
unsigned int trigwallfront_right = 6;
unsigned int echowallfront_right = 7;
int lightswitchleft = 12;
int lightswitchright = 13;
int valueleft;
int valueright;
int robotarmoutput = 11;
int robotarminput = 10;
int robotfinish;

int position_1, position_2, position_3, position_4, turn_position = 0, straight_position = 0;

int start = 0;
int tobridge = 0;
int grabzone = 0;

int turned = 0;

unsigned long Time_Echo_us_left = 0;
unsigned long Time_Echo_us_right = 0;
unsigned long Time_Echo_us_front_left = 0;
unsigned long Time_Echo_us_front_right = 0;

unsigned long wallright = 0;
unsigned long wallleft = 0;
unsigned long wallfront_left = 0;
unsigned long wallfront_right = 0;
unsigned long distance_mm;

int rounds = 1;



void setup() {
	// put your setup code here, to run once:
	can.initdriver(CAN_BPS_1000K, 0, 0, 1);//can.initdriver(CAN_BPS_1000K, 0, 0, 4λ��ģʽ);
	Serial.begin(9600);
	//Serial1.begin(9600);
	pinMode(echowallleft, INPUT);
	pinMode(trigwallleft, OUTPUT);
	pinMode(trigwallright, OUTPUT);
	pinMode(echowallright, INPUT);
	pinMode(trigwallfront_left, OUTPUT);
	pinMode(echowallfront_left, INPUT);
	pinMode(trigwallfront_right, OUTPUT);
	pinMode(echowallfront_right, INPUT);
	pinMode(lightswitchleft, INPUT);
	pinMode(lightswitchright, INPUT);
	pinMode(robotarmoutput, OUTPUT);
	pinMode(robotarminput, INPUT);
	digitalWrite(robotarminput, LOW);

}


void loop() {
	// put your main code here, to run repeatedly:

	if (start == 0)
	{
		digitalWrite(robotarminput, LOW);
		delay(5000);
		start = start + 1;
	}
	if (start == 1)//出发
	{

		detectwallfront_right();
		can.speedwheel(5000, 0, 0);
		//delay(200);
		if (wallfront_right <= 50)//////////////////////到桥
		{

			can.initdriver(CAN_BPS_1000K, 0, 0, 4);

			start = start + 1;
		}
		if (rounds == 1)
		{
			detectwallleft();
			detectwallright();
		}

	}
	if (start == 2)//转弯
	{
		if (turned == 0 && (wallleft < wallright))
		{
			turnleft();
			delay(4000);
		}
		if (turned == 0 && (wallleft > wallright))
		{
			turnright();
			delay(4000);
		}
		if (turned == 1)
		{
			start = start + 1;

		}




	}
	if (start == 3)//开环模式
	{

		can.initdriver(CAN_BPS_1000K, 0, 0, 1);
		delay(100);
		start = start + 1;

	}
	if (start == 4)//光电过桥
	{
		valueleft = digitalRead(lightswitchleft);
		valueright = digitalRead(lightswitchright);
		detectwallfront_right();
		if (valueleft == 1)
		{
			can.speedwheel(3000, 0, 1);
			can.speedwheel(3000, 0, 4);//��
			can.speedwheel(3550, 0, 2);
			can.speedwheel(3550, 0, 3);//��

		}
		if (valueright == 1)
		{
			can.speedwheel(3550, 0, 1);
			can.speedwheel(3550, 0, 4);//��
			can.speedwheel(3000, 0, 2);
			can.speedwheel(3000, 0, 3);//��
		}
		if (valueright == 0 && valueleft == 0)
		{
			can.speedwheel(3000, 0, 0);

		}
		if (wallfront_right<30) {////////////////////////////墙

			start = start + 1;


		}

	}
	if (start == 5)//下桥
	{
		delay(100);
		can.initdriver(CAN_BPS_1000K, 0, 0, 4);
		start = start + 1;

	}
	if (start == 6)//转向取物台
	{
		if (turned == 1 && (wallleft < wallright))
		{
			can.positionwheel(3000, -211120, 0, 1);
			can.positionwheel(3000, 211120, 0, 2);
			can.positionwheel(3000, 211120, 0, 3);
			can.positionwheel(3000, -211120, 0, 4);
			turned++;
			delay(4000);

		}
		if (turned == 1 && (wallleft > wallright))
		{
			turnright();
			delay(4000);
		}
		if (turned == 2)
		{
			start = start + 1;

		}

	}
	if (start == 7)//转换开环
	{

		can.initdriver(CAN_BPS_1000K, 0, 0, 1);
		delay(100);
		start = start + 1;

	}
	if (start == 8)//取物台
	{
		detectwallfront_right();
		can.speedwheel(2000, 0, 0);
		//delay(200);
		if (wallfront_right <= 16)///////////取物台
		{

			start = start + 1;
		}


	}
	if (start == 9)//停车等待
	{
		stopwheels();
		//digitalWrite(robotarmoutput, HIGH);
		start = start + 1;

		Serial.print("                          ");
		Serial.print(robotfinish);
		Serial.print("                          ");
	}
	if (start == 10)//信号
	{

		digitalWrite(robotarmoutput, LOW);
		if (digitalRead(robotarmoutput) == LOW) {
			digitalWrite(robotarmoutput, HIGH);
			//delay(2000);
			digitalWrite(robotarmoutput, LOW);
			//delay(18000);
			start = start + 1;

		}
		//digitalWrite(robotarmoutput, LOW);
		/*robotfinish == digitalRead(robotarminput);
		Serial.println("                          ");
		Serial.print(robotfinish);
		Serial.print("                          ");
		*/
		/*while (Serial.available() > 0)
		{
		comdata += char(Serial.read());
		delay(2);
		}
		if (comdata.length() > 0)
		{
		Serial.println(comdata);
		STATE = !STATE;
		digitalWrite(13, STATE);    //ͨ����ƽ��ת��ֱ�ۿ�����������С�
		comdata = "";
		}*/
		//if (robotfinish == 1)
		//{
		//}
	}

	if (start == 11)//后退
	{


		detectwallfront_right();
		can.speedwheel(-1000, 0, 0);
		//delay(200);
		if (wallfront_right >= 55)///////////后退..
		{

			start = start + 1;
			can.initdriver(CAN_BPS_1000K, 0, 0, 4);
			//delay(2000);
		}



	}
	if (start == 12)//无用
	{
		start = start + 1;




	}
	if (start == 13)//转向置物台turntable2
	{
		if (turned == 2 && (wallleft < wallright))
		{
			turnleft();

			delay(3000);
		}
		if (turned == 2 && (wallleft > wallright))
		{
			turnright();

			delay(3000);
		}
		if (turned == 3)
		{
			can.initdriver(CAN_BPS_1000K, 0, 0, 1);

			start = start + 1;

		}
	}
	if (start == 14)//走向置物台totable
	{
		detectwallfront_right();
		if (wallfront_right > 150)/////////////放
		{
			can.speedwheel(2100, 0, 1);
			can.speedwheel(2100, 0, 2);
			can.speedwheel(2100, 0, 3);
			can.speedwheel(2100, 0, 4);
		}



		//delay(200);
		if (wallfront_right <= 110)////////////停在置物台
		{

			can.initdriver(CAN_BPS_1000K, 0, 0, 4);
			delay(2000);
			start = start + 1;

		}
	}
	if (start == 15)//转向桌子turntotable
	{
		if (turned == 3 && (wallleft < wallright))
		{

			can.positionwheel(3000, 226120, 0, 1);
			can.positionwheel(3000, -226120, 0, 2);//226120
			can.positionwheel(3000, -226120, 0, 3);
			can.positionwheel(3000, 226120, 0, 4);
			turned++;


			delay(3000);
		}
		if (turned == 3 && (wallleft > wallright))
		{
			turnleft();

			delay(3000);
		}
		if (turned == 4)
		{
			can.initdriver(CAN_BPS_1000K, 0, 0, 1);

			start = start + 1;

		}
	}
	if (start == 16)//stopnearDesk
	{
		detectwallfront_right();
		can.speedwheel(1000, 0, 0);
		//delay(200);
		if (wallfront_right <= 20)////////////////停在桌前
		{

			stopwheels();
			start = start + 1;

		}
	}
	if (start == 17)//SecondWait
	{
		stopwheels();
		//digitalWrite(robotarmoutput, 1);
		start = start + 1;

		Serial.print("                          ");
		Serial.print(robotfinish);
		Serial.print("                          ");
	}
	if (start == 18)//等待信号
	{

		digitalWrite(robotarmoutput, HIGH);
		delay(1000);
		digitalWrite(robotarmoutput, LOW);
		delay(2000);
		/*robotfinish == digitalRead(robotarminput);
		Serial.println("                          ");
		Serial.print(robotfinish);
		Serial.print("                          ");
		*/
		/*while (Serial.available() > 0)
		{
		comdata += char(Serial.read());
		delay(2);
		}
		if (comdata.length() > 0)
		{
		Serial.println(comdata);
		STATE = !STATE;
		digitalWrite(13, STATE);    //ͨ����ƽ��ת��ֱ�ۿ�����������С�
		comdata = "";
		}*/
		//if (robotfinish == 1)
		//{
		start = start + 1;
		//}
	}
	if (start == 19)//后退
	{
		detectwallfront_right();
		can.speedwheel(-1000, 0, 0);
		//delay(200);
		if (wallfront_right >= 30)
		{

			start = start + 1;
			can.initdriver(CAN_BPS_1000K, 0, 0, 4);
			delay(2000);
		}



	}
	if (start == 20)//无用
	{
		start = start + 1;




	}
	if (start == 21)//转向终点turntofinishi
	{
		if (turned == 4 && (wallleft < wallright))
		{
			turnleft();

			delay(3000);
		}
		if (turned == 4 && (wallleft > wallright))
		{
			turnright();

			delay(3000);
		}
		if (turned == 5)
		{
			can.initdriver(CAN_BPS_1000K, 0, 0, 1);

			start = start + 1;

		}
	}
	if (start == 22)//走向终点
	{
		detectwallfront_right();
		can.speedwheel(1000, 0, 0);
		//delay(200);
		if (wallfront_right <= 25)///////////终点墙
		{

			can.initdriver(CAN_BPS_1000K, 0, 0, 4);
			delay(2000);
			start = start + 1;

		}
	}
	if (start == 23)//转向出发点turntotable
	{
		if (turned == 5 && (wallleft < wallright))
		{
			turnright();

			delay(3000);
		}
		if (turned == 5 && (wallleft > wallright))
		{
			turnleft();

			delay(3000);
		}
		if (turned == 6)
		{
			can.initdriver(CAN_BPS_1000K, 0, 0, 1);

			start = start + 1;

		}
	}
	if (start == 24)//stopFinish
	{
		detectwallfront_right();
		can.speedwheel(1000, 0, 0);
		//delay(200);
		if (wallfront_right <= 20)///////////停在终点
		{

			stopwheels();
			start = start + 1;

		}
	}



	Serial.println(valueleft);
	Serial.println(valueleft);

	rounds++;
	Serial.print("                    ");
	Serial.print(start);
	Serial.print("rs");


	Serial.println(rounds);
}
void gostraight() {
	can.positionwheel(1000, position_1, 0, 1);
	can.positionwheel(1000, position_2, 0, 2);
	can.positionwheel(1000, position_3, 0, 3);
	can.positionwheel(1000, position_4, 0, 4);

	/*can.speedwheel(-1000, 0, 1);
	can.speedwheel(-1000, 0, 2);
	can.speedwheel(-1000, 0, 3);
	can.speedwheel(-1000, 0, 4);*/
}
void turnleft() {

	/*for (turn_position = 0; turn_position <= 15; turn_position++) {
	can.positionwheel(1000, position_1 = position_1 - 13820, 0, 1);
	can.positionwheel(1000, position_4 = position_4 - 13820, 0, 4);
	can.positionwheel(1000, position_2 = position_2 + 13820, 0, 2);
	can.positionwheel(1000, position_3 = position_3 + 13820, 0, 3);
	delay(200);

	}
	if (turn_position == 16)
	{
	turned = turned + 1;
	}*/
	//221120
	can.positionwheel(3000, -210120, 0, 1);
	can.positionwheel(3000, 210120, 0, 2);
	can.positionwheel(3000, 210120, 0, 3);

	can.positionwheel(3000, -210120, 0, 4);

	turned = turned + 1;




	/*can.speedwheel(1000, 0, 1);
	can.speedwheel(-1000, 0, 2);
	can.speedwheel(-1000, 0, 3);
	can.speedwheel(1000, 0, 4);
	*/
}
void turnright() {
	/*
	can.speedwheel(-1000, 0, 1);
	can.speedwheel(1000, 0, 2);
	can.speedwheel(1000, 0, 3);
	can.speedwheel(-1000, 0, 4);*/

	can.positionwheel(3000, 210120, 0, 1);//216120
	can.positionwheel(3000, -210120, 0, 2);
	can.positionwheel(3000, -210120, 0, 3);
	can.positionwheel(3000, 210120, 0, 4);

	turned = turned + 1;


}
void stopwheels()
{
	can.speedwheel(0, 0, 1);
	can.speedwheel(0, 0, 2);
	can.speedwheel(0, 0, 3);
	can.speedwheel(0, 0, 4);
}
void detectwallleft() {
	digitalWrite(trigwallleft, LOW);
	delayMicroseconds(20);
	digitalWrite(trigwallleft, HIGH); //��ʼͨ��Trig/Pin��������
	delayMicroseconds(50);       //����������Ϊ50us (>10us)
	digitalWrite(trigwallleft, LOW);//��������

	Time_Echo_us_left = pulseIn(echowallleft, HIGH); //����US-100���ص�������

	if ((Time_Echo_us_left < 60000) && (Time_Echo_us_left > 1)) //������Ч��Χ(1, 60000).
	{
		// distance_mm = (Time_Echo_us * 0.34mm/us) / 2 (mm)
		distance_mm = (Time_Echo_us_left * 34 / 100) / 2; //ͨ�������ȼ������.
		wallleft = distance_mm;
		Serial.print("left: ");//�����������ڼ�����
		Serial.print(distance_mm, DEC);          //�����������ڼ�����
		Serial.println("mm");               //�����������ڼ�����
	}
	delay(90);

}
void detectwallright() {
	digitalWrite(trigwallright, LOW);
	delayMicroseconds(20);
	digitalWrite(trigwallright, HIGH);
	delayMicroseconds(50);
	digitalWrite(trigwallright, LOW);

	Time_Echo_us_right = pulseIn(echowallright, HIGH);

	if ((Time_Echo_us_right < 60000) && (Time_Echo_us_right > 1)) //������Ч��Χ(1, 60000).
	{
		// distance_mm = (Time_Echo_us * 0.34mm/us) / 2 (mm)
		distance_mm = (Time_Echo_us_right * 34 / 100) / 2; //ͨ�������ȼ������.
		wallright = distance_mm;
		Serial.print("          right: ");//�����������ڼ�����
		Serial.print(distance_mm, DEC);          //�����������ڼ�����
		Serial.println("mm");               //�����������ڼ�����
	}


}
void detectwallfront_left() {
	digitalWrite(trigwallfront_left, LOW);
	delayMicroseconds(20);
	digitalWrite(trigwallfront_left, HIGH);
	delayMicroseconds(50);
	digitalWrite(trigwallfront_left, LOW);

	Time_Echo_us_front_left = pulseIn(echowallfront_left, HIGH);

	if ((Time_Echo_us_front_left < 60000) && (Time_Echo_us_front_left > 1)) //������Ч��Χ(1, 60000).
	{
		// distance_mm = (Time_Echo_us * 0.34mm/us) / 2 (mm)
		distance_mm = (Time_Echo_us_front_left * 34 / 100) / 2; //ͨ�������ȼ������.
		wallfront_left = distance_mm;
		Serial.print("                     front left: ");//�����������ڼ�����
		Serial.print(wallfront_left, DEC);          //�����������ڼ�����
		Serial.println("mm");               //�����������ڼ�����
	}
	delay(90);
}
void detectwallfront_right() {
	/*digitalWrite(trigwallfront_right, LOW);
	delayMicroseconds(20);
	digitalWrite(trigwallfront_right, HIGH);
	delayMicroseconds(50);
	digitalWrite(trigwallfront_right, LOW);

	Time_Echo_us_front_right = pulseIn(echowallfront_right, HIGH);

	if ((Time_Echo_us_front_right < 60000) && (Time_Echo_us_front_right > 1))
	{
	distance_mm = (Time_Echo_us_front_right * 34 / 100) / 2;
	wallfront_right = distance_mm;
	Serial.print("                                  front right");
	Serial.print(distance_mm, DEC);
	Serial.println("mm");
	}*/
	digitalWrite(trigwallfront_right, LOW);
	delayMicroseconds(20);
	digitalWrite(trigwallfront_right, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigwallfront_right, LOW);
	wallfront_right = pulseIn(echowallfront_right, HIGH) / 58.0;
	Serial.print(wallfront_right);
	Serial.println("cm");
	Serial.println();
	delay(20);

}
