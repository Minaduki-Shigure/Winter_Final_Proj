#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "AD9854.h"
#include "spix.h"
#include "adc.h"

#include <math.h>

#define X_1M 25
#define X_200k 1
#define X_40M 211

#define Y_0dB 20
#define Y_10dB 50
#define Y_20dB 80
#define Y_30dB 110
#define Y_40dB 140

#define Y_10 180
#define Y_5 190
#define Y_0	200
#define Y_5_ 210
#define Y_10_ 220
#define Y_15_ 230
#define Y_20_ 240
#define Y_25_ 250
#define Y_30_ 260
#define Y_35_ 270
#define Y_40_ 280
#define Y_45_ 290
#define Y_50_ 300

int adc_raw[391]={0};
int adc1data,adc2data;
	
 int main(void)
 { 
		delay_init();	    	 //延时函数初始化	  
		uart_init(9600);	 	//串口初始化为9600
		//SPIx_Init();
		//LED_Init();		  		//初始化与LED连接的硬件接口
		LCD_Init();			   	//初始化LCD 	
		//AD9854Init();
		//Freq_SW();
		Adc_Init();
		Adc2_Init();
	 
	 	LCD_DrawLine(1,160,240,160);
		LCD_DrawLine(20,140,220,140);
		LCD_DrawLine(20,140,20,20);
		LCD_DrawLine(20,300,220,300);
		LCD_DrawLine(20,300,20,180);
	 
		while(1)
		{
			adc1data=Get_Adc_Average(ADC_Channel_1,5);
			adc2data=Get_Adc2_Average(ADC_Channel_10,5);
			LCD_ShowNum(40,40,adc1data,4,16);
			LCD_ShowNum(40,80,adc2data,4,16);
		}
}	

