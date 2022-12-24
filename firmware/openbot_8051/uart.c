//�����������մ����ַ���
#include<AT89X52.H>		      //����51��Ƭ��ͷ�ļ����ڲ��и��ּĴ�������
//#include <reg52.h>
#include "uart.h"
#include "stdlib.h"
#include "string.h"


typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

char msg_buf[MAX_MSG_SZ];     //�������飬���淵�����


//���ڳ�ʼ������  9600
void ConfigUART(unsigned int baud)
{
//    SCON  = 0x50;  //���ô���Ϊģʽ1 ��Ƭ������ͨѶ ������ѡ��ʽ1
//    TH1 = 256 - (11059200/12/32)/baud;  //���㶨ʱ��T1����ֵ	���ò��������ʵĶ�ʱ��T1
//    TL1 = TH1;     //��ֵ��������ֵ
//	ES = 1;			//���� �ж� ����
//	PS=1;       	//�����ж���������ȼ�
//
//    TR1 = 1;       //������ʱ��T1
//	TMOD &= 0x0F;  //����T1�Ŀ���λ
//    TMOD |= 0x20;  //����T1Ϊģʽ2
//	ET1 = 0;		// ��ʱ��T1 �ж�  �ر�

/*******************************************************************************************/

	TMOD = 0x21;                   //T0��ʱ��Ϊ������ʽһ��T1��ʱ��Ϊ������ʽ��
	PCON = 0x00;                   //���ڳ�ʼ����أ�������
	SCON = 0x50;                   //���ڳ�ʼ����أ����ڹ�����ʽһ���������
	TH1  = 0xFD;                   //���ó�ֵ
	TL1  = 0xFD;                   //���ó�ֵ
	if(baud == 115200){ }
	else{
    TH1 = 256 - (11059200/12/32)/baud;  //���㶨ʱ��T1����ֵ	���ò��������ʵĶ�ʱ��T1
	 }
    TL1 = TH1;     					//��ֵ��������ֵ
	TR1  = 1;                      //������ʱ��T1
	
	
	TH0 = 0xF4;                    //���ó�ֵ
	TL0 = 0x48;                    //���ó�ֵ
	TR0 = 1;                       //������ʱ��T0
	ES  = 1;                       //���Ŵ����ж�
	PT0 = 1;                       //��ʱ��0�ж�����
	ET0 = 1;                       //���Ŷ�ʱ��T0�ж�
	ET1 = 0;						//�ر� ��ʱ��T1 �ж�  		��Ϊ������ʽ�� ����򿪶�ʱ��1 �ж�
	EA  = 1;                       //�������ж�

}
 
//����һ���ַ�
void Uart_Send_Byte(unsigned char dat)
{
 	SBUF = dat;
	while(!TI);
	TI=0;
}

//����һ���ַ���
void Uart_Send_String(unsigned char *p)
{
 	while(*p != '\0')
	{
	 	Uart_Send_Byte(*p++);	
	} 
}

void process_ctrl_msg()
{
  char *tmp;                   // this is used by strtok() as an index
  tmp = strtok(msg_buf, ","); // replace delimiter with ","
  Uart_Send_String("P_l:");
  Uart_Send_String(tmp);
  ctrl_left = atoi(tmp);       // convert to int

  tmp = strtok(NULL, ",");    // continues where the previous call left off
  Uart_Send_String("P_r:");
  Uart_Send_String(tmp);
  ctrl_right = atoi(tmp);      // convert to int
     
  Uart_Send_String("\n");

}

bit Deal_UART_RecData()   //�����ڽ������ݰ��������ɹ��������ݰ��򷵻�1�����򷵻�0��
{
	char head =  RX_DAT[0];
  	Uart_Send_String("m1:");	
	Uart_Send_String(RX_DAT);
	Uart_Send_String("\n");
	strncpy(msg_buf, RX_DAT+1, strlen(RX_DAT));
	Uart_Send_String("m2:");
	Uart_Send_String(msg_buf);
	Uart_Send_String("\n");
    switch (head)
  {

    case 'c':
      process_ctrl_msg();
      break;
//    case 'f':
//      process_feature_msg();
//      break;
//    case 'h':
//      process_heartbeat_msg();
//      break;
    default :
	  break;
	}
//  	Uart_Send_String("\r\nover\r\n");
	memset(RX_DAT,0,sizeof(char)*MAX_MSG_SZ);//��ʱRX_DAT�е�����ȫ����0��
	RX_DAT[0] = '\0';
	RX_OVER=0;
    return 0;
}

u8 ch,i;
u8 RX_BUF[20],RX_DAT[20],RX_CNT=0,RX_OVER=0;
//���ڽ����ж�
void Uart_IRQ() interrupt 4
{

 if(RI)
 {  
   ch=SBUF;
   if(ch!='\n')
   {
	  RX_BUF[RX_CNT++]=ch;
   }
   else	//����
   {
	 for(i=0;i<RX_CNT;i++)
	 {
	  	RX_DAT[i]=RX_BUF[i];
	 }
	 RX_DAT[RX_CNT+1] = '\0';
	 RX_CNT=0;
	 RX_OVER=1;

	 Deal_UART_RecData();
   }
   RI=0;//���־λ 
 } 	 
}

