#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "usmart.h"  
#include "sram.h"   
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"	 
#include "usbh_usr.h" 

#include "Fattester.h"
#include "string.h"

#include "usbusr.h"    //my_debug

//ALIENTEK 探索者STM32F407开发板 实验53
//USB U盘(Host)实验 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;
extern u8 config_finish;
extern u8 send_finish;
//用户测试主程序
//返回值:0,正常
//       1,有问题
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	Show_Str(30,140,200,16,"设备连接成功!.",16,0);	 
  	f_mount(fs[2],"2:",1); 	//重新挂载U盘
	res=exf_getfree("2:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//设置字体为蓝色	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16);//显示U盘总容量 MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
	
	//add write u盘(12.05)
	#if 0
	if(HCD_IsDeviceConnected(&USB_OTG_Core))
	{
		f_open(file,"2:/test.txt",7);
		f_write(file,"danpi's u test file",20,&bw);
		f_close(file);
	}
#endif

	#if 1
		if(HCD_IsDeviceConnected(&USB_OTG_Core))
		{
			
		}
	#endif
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	LED1=1;//关闭LED1
  	f_mount(0,"2:",1); 	//卸载U盘
	POINT_COLOR=RED;//设置字体为红色	   
	Show_Str(30,140,200,16,"设备连接中...",16,0);
	LCD_Fill(30,160,239,220,WHITE);
	return res;
} 

int main(void)
{        
	u8 t;
	Stm32_Clock_Init(336,8,2,7);//设置时钟
	delay_init(168);		//初始化延时
	uart_init(84,115200);	//115200波特率
	LED_Init();				//初始化与LED连接的硬件接口
	KEY_Init();				//按键
  	LCD_Init();				//初始化LCD 
	W25QXX_Init();			//SPI FLASH初始化
	usmart_dev.init(84); 	//初始化USMART	 
	my_mem_init(SRAMIN);	//初始化内部内存池	
 	exfuns_init();			//为fatfs相关变量申请内存 
	piclib_init();			//初始化画图
  	f_mount(fs[0],"0:",1); 	//挂载SD卡  
  	f_mount(fs[1],"1:",1); 	//挂载外部SPI FLASH盘   
	POINT_COLOR=RED;      
 	while(font_init()) 				//检查字库
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示	     
		delay_ms(200);				  
	}
	Show_Str(30,50,200,16,"探索者STM32F407开发板",16,0);				    	 
	Show_Str(30,70,200,16,"USB U盘实验",16,0);					    	 
	Show_Str(30,90,200,16,"2014年7月22日",16,0);	    	 
	Show_Str(30,110,200,16,"正点原子@ALIENTEK",16,0); 
	Show_Str(30,140,200,16,"设备连接中...",16,0);			 		
	//初始化USB主机
    USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
	USB_OTG_Core.config=1;
	while(1)
	{
		if(config_finish)
		{
			delay_ms(9000);
			USB_OTG_Core.config=0;
			config_finish=0;
			printf("config over\r\n");
		}
		USBH_Process(&USB_OTG_Core, &USB_Host);
		SendMechine=1;
		Receive_Retry=2;
		delay_ms(1);
		t++;
		if(t==200)
		{
			LED0=!LED0;
			t=0;
		}
	}	
}
















