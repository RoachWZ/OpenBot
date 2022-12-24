/* C51 for OpenBot 1.22*/
//���ڷֱ����2��pwm 	
//�д����� �������Ӳ�����⵼����������ٶȲ�һ��	   
//��ʱ�俴�� ֱ��������� pwm �� pid �㷨	https://blog.csdn.net/luzire/article/details/83144381
#include<AT89X52.H>		      //����51��Ƭ��ͷ�ļ����ڲ��и��ּĴ�������
//#include <reg52.h>
#include"uart.h"
#define SPEED 128	   //�ٶȵ��ڱ��� 0-255������0��С��255���
#define HIGH 20		  //threshold value ��ֵ
#define LOW -HIGH		  //threshold value ��ֵ
#define HZ 100	// ���� Լ0.1ms һ���ж�
    #define Left_1_led        P3_4	 //�󴫸���  
	
    #define Right_1_led       P3_5	 //�Ҵ�����    
   
//	#define Left_moto_pwm	  P1_6	 //PWM�źŶ�
//
//	#define Right_moto_pwm	  P1_7	 //PWM�źŶ�
	
	#define Left_moto_front   {P1_2=0,P1_3=1;}  //������ǰ��
	#define Left_moto_back    {P1_2=1,P1_3=0;} 	//��ߵ�����ת
	#define Left_moto_stop    {P1_2=0,P1_3=0;}         //��ߵ��ͣת                     
	#define Right_moto_front  {P1_4=0,P1_5=1;}	//�ұߵ����ǰ��
	#define Right_moto_back   {P1_4=1,P1_5=0;}	//�ұߵ�������
	#define Right_moto_stop   {P1_4=0,P1_5=0;}      	//�ұߵ��ͣת
//L293D���Ŷ���      
//sbit in1 = P1^2;	 //������ҵ��λ�ò��ԣ�34 �� 12 ����λ��
//sbit in2 = P1^3;
//sbit in3 = P1^4;
//sbit in4 = P1^5;
//sbit ena = P0^7; 	 //	������û������˳�����Ű��ߣ���˵��ô�ֱ����2��pwmһֱ�����ԣ��������ԣ��»���������Ҳ�����
//sbit enb = P0^6;      
sbit ena = P1^6;	
sbit enb = P1^7;	
 
int ctrl_left  = 0;
int ctrl_right = 0;
uint PWMA = 20;         
uint PWMB = 20; 
unsigned char MA = 0,MB = 0;           //pwm������


/************************************************************************/	
//��ʱ����	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}

//void ConfigPWM(long cl, long cr)
//{
//	PWMA = cl;
//	PWMB = cr;
//}

void update_right_motors()
{
  if (ctrl_right < LOW)
  {
	if(ctrl_right < -255) ctrl_right = -255;//����Χ���ݴ���
  	Right_moto_back;
	PWMB = -ctrl_right;
	
  }
  else if (ctrl_right > HIGH)
  {
  	if(ctrl_right > 255) ctrl_right = 255;//����Χ���ݴ���
    Right_moto_front;  
	PWMB = ctrl_right;
  }
  else
  {	
    Right_moto_stop;
	PWMB = 20;
  }
}


void update_left_motors()
{
  if (ctrl_left < LOW)
  {
	if(ctrl_left < -255) ctrl_left = -255;//����Χ���ݴ���
    Left_moto_back;
	PWMA = -ctrl_left;
  }
  else if (ctrl_left > HIGH)
  {
  	if(ctrl_left > 255) ctrl_left = 255;//����Χ���ݴ���
    Left_moto_front;
	PWMA = ctrl_left;
  }
  else
  {
	Left_moto_stop;
	PWMA = 20;
  }
}

/************************************************************************/
//ֹͣ
     void  stop(void)
{
     ctrl_left = 0;	 //�ٶȵ��ڱ��� 0-255������0��С��255���
	 ctrl_right = 0;
	 update_left_motors();
   	 update_right_motors();
}
//ǰ��
     void  run(void)
{
     ctrl_left = SPEED;	 //�ٶȵ��ڱ��� 0-255������0��С��255���
	 ctrl_right = SPEED;
	 update_left_motors();
   	 update_right_motors();
}

//���� 
     void  backrun(void)
{
     ctrl_left = -SPEED;	 
	 ctrl_right = -SPEED;
	 update_left_motors();
   	 update_right_motors();
}

//��ת
     void  leftrun(void)
{	 
     ctrl_left = -SPEED;	 
	 ctrl_right = SPEED;
	 update_left_motors();
   	 update_right_motors();
}

//��ת
     void  rightrun(void)
{ 
	 ctrl_left = SPEED;	 
	 ctrl_right = -SPEED;
	 update_left_motors();
   	 update_right_motors();	
}

//������ 
void main()
{   
	EA=1;
    ConfigUART(9600);  //���ò�����Ϊ9600
	//ConfigUART(115200);  //���ò�����Ϊ115200
	Uart_Send_String("8051 for OpenBot 1.22 \r\n");//OpenBot�ֻ��˽���δ�涨������ͷ�ᱨ��,�ҽ�����ֻ���USB�����쳣���ݵĴ���Ҳ���԰�����Ĵ������ע�͵�
	P1=0X00; //�ص糵���

    while (1)
    {
//	  ConfigPWM(ctrl_left,ctrl_right);//pwm����	 

	if(Left_1_led==1&&Right_1_led==1){//������� ���ź�Ϊ0  û���ź�Ϊ1
	  update_left_motors();
   	  update_right_motors();
	 }else{			  
		if(Left_1_led==1&&Right_1_led==0)	    //�ұ߼�⵽�����ź�
	 	 {
		 	   leftrun();	  //����С����ת����
			   delay(40);

	     }
	   
		if(Right_1_led==1&&Left_1_led==0)		//��߼�⵽�����ź�
		  {	  
		      
			 
			   rightrun();	 //����С����ת����
			  delay(40);

		  }
		if(Right_1_led==0&&Left_1_led==0)		//���ߴ�����ͬʱ��⵽����
		  {	  
		    backrun();		//���õ�����˺���
			delay(40);		//����050����
			rightrun();		//���õ����ת����
			delay(90);
		  }

		  stop(); //�ص糵���
	}
//	  Uart_Send_String(" PWMA:");
//	  Uart_Send_Byte(PWMA/100+0x30);	//��λ
//	  Uart_Send_Byte((PWMA-PWMA/100*100)/10+0x30);	 //ʮλ
//	  Uart_Send_Byte(PWMA%10+0x30);					//	 ��λ
//	  Uart_Send_String(" PWMB:");
//	  Uart_Send_Byte(PWMB/100+0x30);	//��λ
//	  Uart_Send_Byte((PWMB-PWMB/100*100)/10+0x30);	 //ʮλ
//	  Uart_Send_Byte(PWMB%10+0x30);					//	 ��λ
//	  Uart_Send_String("\r\n");
	delay(1);
	}
}
 

void InterruptTimer0() interrupt 1
{
TR0 = 0;  
			    
	TH0 = (65536-HZ)/256;			   //65536 = ffff = 16λ
	TL0 = (65536-HZ)%256;			   //256 ff 8λ

MB++;         
if(MB < PWMB)  
{   
enb = 1;                             //ʹ��enb������pwm������B�˵��
}  
else
  enb = 0;  
if(MB == 255)
{   
  MB = 0;  
}

MA++;         
if(MA < PWMA)  
{   
ena = 1;                                 //ʹ��ena������pwm������A�˵��
}  
else
  ena = 0;  
if(MA == 255)
{   
  MA = 0;  
}  

TR0 = 1;

}
