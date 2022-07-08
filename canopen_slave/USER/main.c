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

//ALIENTEK 探索者STM32F407开发板 实验27
//CAN通信实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


int main(void)
{ 
	u8 t=0;
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
    TIM3_Int_Init(5000-1,84-1);  //系统时钟是72M,分频72,也就是1M的频率,计数1次,耗时1us.
 	canfestival_can_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps    
    
    
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
			LED0=!LED0;//提示系统正在运行	
			t=0;

		}		   
	} 
	
}
