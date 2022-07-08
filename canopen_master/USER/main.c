#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
 
#include "canfestival.h"
#include "canfestival_can.h"
#include "canfestival_timer.h"
#include "canfestival_master.h"

//ALIENTEK ̽����STM32F407������ ʵ��27
//CANͨ��ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

void canopen_send_sdo(uint16_t message_sdo[])
{
	  unsigned long abortCode=0;
    int8_t      nodeID=0;          /* ID       */
    uint16_t    index=0;           /* ����     */
    uint8_t     subIndex=0;        /* ������   */
    uint8_t     dataType=uint8;    /* �������� Ŀǰ�о�����Ӧ����uint8�Ϳ���ͨ�� */
    uint32_t    count=4;           /* ���ݳ��� */
    uint8_t     data[4];    
    
    nodeID=(int8_t)(message_sdo[0]&0xf);  
    index=(uint16_t)((message_sdo[3]<<16)+message_sdo[2]);
    subIndex=(uint8_t)(message_sdo[4]);
    for(int i=0; i<4; i++)
        data[0+i]=(uint8_t)message_sdo[5+i];

    //d; nodeId; index; subIndex; count; dataType; data[4]; useBlockMode;
    writeNetworkDict(&Master_Data, nodeID, index, subIndex, count, dataType, &data, 0);
    while(getWriteResultNetworkDict(&Master_Data, nodeID, &abortCode) != SDO_FINISHED)
    {
        break;
    }
    
}


int main(void)
{ 
	u8 key;
	u8 t=0;
//    Message i = {0,0,8,0};
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	KEY_Init(); 				//������ʼ��  
    TIM3_Int_Init(5000-1,84-1);  //ϵͳʱ����72M,��Ƶ72,Ҳ����1M��Ƶ��,����1��,��ʱ1us.
 	canfestival_can_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps    
    
    
    setNodeId(&Master_Data, 0x7f);
    setState(&Master_Data, Initialisation);
    setState(&Master_Data,Pre_operational);
    setState(&Master_Data, Operational);
    stopSYNC(&Master_Data);

    uint16_t message_sdo[][9]=
    {
			{0x601,0x22,0x00,0x14,0x01,0x01,0x02,0x00,0x80},  // ʧ��pdo�� ����sdo  601,22 00 14 01 01 02 00 80
			{0x601,0x22,0x00,0x16,0x00,0x00,0x00,0x00,0x00},  // ��ȥ����: ����sdo 601,22 00 16 00 00 00 00 00
			{0x601,0x22,0x00,0x16,0x01,0x20,0x00,0xff,0x60},  // д�����: ����sdo 601,22 00 16 01 20 00 ff 60
			{0x601,0x22,0x00,0x14,0x01,0x01,0x02,0x00,0x00},  // ʹ��pdo:  ����sdo 601,22 00 14 01 01 02 00 00
			
			{0x601,0x2f,0x60,0x60,0x00,0x03,0x00,0x00,0x00},  // �л��ٶ�ģʽ 
			
			{0x601,0x2b,0x40,0x60,0x01,0x01,0x02,0x00,0x80},  // �л�������״̬6
			{0x601,0x2b,0x40,0x60,0x01,0x01,0x02,0x00,0x80},  // �л�������״̬7
			{0x601,0x2b,0x40,0x60,0x01,0x01,0x02,0x00,0x80},  // �л�������״̬f
    };
    
    for (int i=0;i<8;i++)
    {
        canopen_send_sdo(message_sdo[i]);
        delay_ms(20);
    }
    
    startSYNC(&Master_Data);    
    while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0����,����һ������
		{

							   
		}		 
		if(key)//���յ�������
		{			

		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			t=0;

		}		   
	} 
	
}
