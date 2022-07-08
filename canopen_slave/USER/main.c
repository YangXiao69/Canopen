#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
 
#include "canfestival.h"
#include "canfestival_can.h"
#include "canfestival_timer.h"
#include "slave.h"

//ALIENTEK ̽����STM32F407������ ʵ��27
//CANͨ��ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


int main(void)
{ 
	u8 t=0;
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
    TIM3_Int_Init(5000-1,84-1);  //ϵͳʱ����72M,��Ƶ72,Ҳ����1M��Ƶ��,����1��,��ʱ1us.
 	canfestival_can_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps    
    
    
    setNodeId(&slave_Data, 0x01);
    setState(&slave_Data, Initialisation);
    setState(&slave_Data,Pre_operational);
    setState(&slave_Data, Operational);
//    stopSYNC(&slave_Data);


//    startSYNC(&slave_Data);    
    while(1)
	{
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			t=0;

		}		   
	} 
	
}
