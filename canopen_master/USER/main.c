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

//ALIENTEK 探索者STM32F407开发板 实验27
//CAN通信实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

void canopen_send_sdo(uint16_t message_sdo[])
{
	  unsigned long abortCode=0;
    int8_t      nodeID=0;          /* ID       */
    uint16_t    index=0;           /* 索引     */
    uint8_t     subIndex=0;        /* 子索引   */
    uint8_t     dataType=uint8;    /* 数据类型 目前感觉这里应该是uint8就可以通用 */
    uint32_t    count=4;           /* 数据长度 */
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
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	KEY_Init(); 				//按键初始化  
    TIM3_Int_Init(5000-1,84-1);  //系统时钟是72M,分频72,也就是1M的频率,计数1次,耗时1us.
 	canfestival_can_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps    
    
    
    setNodeId(&Master_Data, 0x7f);
    setState(&Master_Data, Initialisation);
    setState(&Master_Data,Pre_operational);
    setState(&Master_Data, Operational);
    stopSYNC(&Master_Data);

    uint16_t message_sdo[][9]=
    {
			{0x601,0x22,0x00,0x14,0x01,0x01,0x02,0x00,0x80},  // 失能pdo： 发送sdo  601,22 00 14 01 01 02 00 80
			{0x601,0x22,0x00,0x16,0x00,0x00,0x00,0x00,0x00},  // 消去个数: 发送sdo 601,22 00 16 00 00 00 00 00
			{0x601,0x22,0x00,0x16,0x01,0x20,0x00,0xff,0x60},  // 写入参数: 发送sdo 601,22 00 16 01 20 00 ff 60
			{0x601,0x22,0x00,0x14,0x01,0x01,0x02,0x00,0x00},  // 使能pdo:  发送sdo 601,22 00 14 01 01 02 00 00
			
			{0x601,0x2f,0x60,0x60,0x00,0x03,0x00,0x00,0x00},  // 切换速度模式 
			
			{0x601,0x2b,0x40,0x60,0x01,0x01,0x02,0x00,0x80},  // 切换到就绪状态6
			{0x601,0x2b,0x40,0x60,0x01,0x01,0x02,0x00,0x80},  // 切换到就绪状态7
			{0x601,0x2b,0x40,0x60,0x01,0x01,0x02,0x00,0x80},  // 切换到就绪状态f
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
		if(key==KEY0_PRES)//KEY0按下,发送一次数据
		{

							   
		}		 
		if(key)//接收到有数据
		{			

		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//提示系统正在运行	
			t=0;

		}		   
	} 
	
}
