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

#define AMP	1000

//int adc1_raw[391]={0};
//int adc2_raw[391]={0};

//int adc1data,adc2data;

int adc1;
int adc2;
float vi;
float vq;
float H_raw;
float H_dB;
float phase;
float phase_d;
int lcd_x_temp=2*X_1M;
int lcd_x;
int lcd_y1;
int lcd_y2;

float freq;
float xMHz=1;
float fit;

extern u16 Get_Adc_Average(u8 ch,u8 times);
extern u16 Get_Adc2_Average(u8 ch,u8 times);

int scan_single(void);
	
int main(void)
{ 
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
		delay_init();	    	 //延时函数初始化	  
		uart_init(9600);	 	//串口初始化为9600
		//SPIx_Init();
		//LED_Init();		  		//初始化与LED连接的硬件接口
		LCD_Init();			   	//初始化LCD 	
		AD9854Init();
		//Freq_SW();
		Adc_Init();
		Adc2_Init();
	 
	 	LCD_DrawLine(1,160,240,160);
		LCD_DrawLine(20,140,220,140);
		LCD_DrawLine(20,140,20,20);
		LCD_DrawLine(20,300,220,300);
		LCD_DrawLine(20,300,20,180);
	 
		xMHz=1;
		freq=xMHz*1000000;
		fit=864000/(267-2.35*xMHz);		//强行调整输出
		if(fit>4095)
			fit=4095;
		AD9854SetAmp(fit,fit);
		AD9854WriteFreqSingle(freq);
		xMHz+=0.1;
		
		while(1)
		{			
			scan_single();		
		}
}

int scan_single(void)
{
	adc1=Get_Adc_Average(ADC_Channel_1,5);
	adc2=Get_Adc2_Average(ADC_Channel_2,5);
	
	
	freq=xMHz*1000000;
	if(xMHz<25)
	{
		fit=864000/(267-2.35*xMHz);		//强行调整输出
	}
	else if(xMHz>29)
	{
		fit=864000/(146+2.33*xMHz);
	}
	else
	{
		fit=4075;
	}
	if(fit>4095)
		fit=4095;
	AD9854SetAmp(fit,fit);
	AD9854WriteFreqSingle(freq);
	xMHz+=0.1;
	if(xMHz>40)
		xMHz=1;
	
	
	vi=3300*(adc1/4095);
	vq=3300*(adc2/4095);
	H_raw=(2*sqrt(vi*vi+vq*vq))/(AMP*AMP);
	H_dB=20*log(H_raw);
	phase=atan(vq/vi);
	phase_d=phase*(180/3.14);
	lcd_x=lcd_x_temp/2;
	lcd_x_temp++;
	
	lcd_y1=20-3*H_dB;
	lcd_y2=200-2*phase_d;
	//LCD_ShowNum(40,40,lcd_y1,4,16);
	//LCD_ShowNum(40,80,lcd_y2,4,16);
	//LCD_ShowNum(40,40,adc1,4,16);
	//LCD_ShowNum(40,80,adc2,4,16);
	//printf("%d %d %d \n",lcd_x,lcd_y1,lcd_y2);
	//printf("%f %f %f %f \n",H_raw,H_dB,phase,phase_d);
	LCD_DrawPoint(lcd_x,lcd_y1);
	LCD_DrawPoint(lcd_x,lcd_y2);
	LCD_Fill(lcd_x+1,0,lcd_x+30,139,WHITE);
	LCD_Fill(lcd_x+1,161,lcd_x+30,299,WHITE);
	if(lcd_x>=X_40M)
	{
		lcd_x_temp=2*X_1M;
		LCD_Fill(21,161,51,299,WHITE);
		//LCD_Clear(WHITE);
		//LCD_DrawLine(1,160,240,160);
		//LCD_DrawLine(20,140,220,140);
		//LCD_DrawLine(20,140,20,20);
		//LCD_DrawLine(20,300,220,300);
		//LCD_DrawLine(20,300,20,180);
	}
	return 0;
}
