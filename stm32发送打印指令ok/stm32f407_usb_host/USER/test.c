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

//ALIENTEK ̽����STM32F407������ ʵ��53
//USB U��(Host)ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;
extern u8 config_finish;
extern u8 send_finish;
//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	Show_Str(30,140,200,16,"�豸���ӳɹ�!.",16,0);	 
  	f_mount(fs[2],"2:",1); 	//���¹���U��
	res=exf_getfree("2:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16);//��ʾU�������� MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
	
	//add write u��(12.05)
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
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//�豸���ӳɹ�
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	LED1=1;//�ر�LED1
  	f_mount(0,"2:",1); 	//ж��U��
	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	Show_Str(30,140,200,16,"�豸������...",16,0);
	LCD_Fill(30,160,239,220,WHITE);
	return res;
} 

int main(void)
{        
	u8 t;
	Stm32_Clock_Init(336,8,2,7);//����ʱ��
	delay_init(168);		//��ʼ����ʱ
	uart_init(84,115200);	//115200������
	LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//����
  	LCD_Init();				//��ʼ��LCD 
	W25QXX_Init();			//SPI FLASH��ʼ��
	usmart_dev.init(84); 	//��ʼ��USMART	 
	my_mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
 	exfuns_init();			//Ϊfatfs��ر��������ڴ� 
	piclib_init();			//��ʼ����ͼ
  	f_mount(fs[0],"0:",1); 	//����SD��  
  	f_mount(fs[1],"1:",1); 	//�����ⲿSPI FLASH��   
	POINT_COLOR=RED;      
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}
	Show_Str(30,50,200,16,"̽����STM32F407������",16,0);				    	 
	Show_Str(30,70,200,16,"USB U��ʵ��",16,0);					    	 
	Show_Str(30,90,200,16,"2014��7��22��",16,0);	    	 
	Show_Str(30,110,200,16,"����ԭ��@ALIENTEK",16,0); 
	Show_Str(30,140,200,16,"�豸������...",16,0);			 		
	//��ʼ��USB����
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
















