/***********************************************************************
Original Author : jian chen
Modify Author   : yusnows
Modify Date     : 2017.8.2
File            : AD9854.c
version         : v0.1.0
************************************************************************/
#include "AD9854.h"
#include "sys.h"
#include "delay.h"

const	uint64_t AD9854_MaxFreqWord=0x0000FFFFFFFFFFFF;			//48-bit

u8 FreqSingle[6]={0xA,0xAA,0xAB,0x0,0x0,0x0};


//******************************************************************


//******************************************************************

//��������:ɨƵ����
//�������:��
//˵����ͷ�ļ��п��޸���ʼƵ�ʣ�����ֵ����ɨ������

//******************************************************************
void Freq_SW(void)
{
	AD9854WriteCtR(ctrsw);
	AD9854WriteFreqWord1(freqbg);	//��ʼƵ��
	AD9854WriteFerqWord2(freqend);	//��ֹƵ��
	AD9854WriteFreqStep(freqstep);	//����Ƶ��
	AD9854WriteFreqStay(freqstay);	//פ��ʱ��
	Update_AD9854();
}



//�������ܣ�����������ú궨���еĿ����ֱ������Ƶ���ģʽ�����Ƶ��
//�������: freq1,1~600
//Serial Register Address :2
//д���ַ�ֽ���:1 Byte
//д�������ֽ���:6 Bytes 
//******************************************************************

void AD9854WriteFreqSingle(double freq)
{
	CalculateFreqWord(freq);
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE1);              				// 0x02��ӦдFrq1
	AD9854WriteByte(FreqSingle);            	// д6���ֽڵ�����,�ȷ���λ
	AD9854WriteByte(FreqSingle+1);
	AD9854WriteByte(FreqSingle+2);
	AD9854WriteByte(FreqSingle+3);
	AD9854WriteByte(FreqSingle+4);
	AD9854WriteByte(FreqSingle+5);
	delay_us(1);
	AD9854_CS_H;
	delay_us(1);
	Update_AD9854();
}


double AD9854ReadFreqSingle(void)
{
	double FreqRead=0;
	uint64_t temp;
	temp=AD9854ReadFreqWord1();
	FreqRead=temp;
	FreqRead=FreqRead/AD9854_MaxFreqWord*AD9854_FREQ_REF;
	return FreqRead;
}


void CalculateFreqWord(double freq)
{
	double temp=AD9854_FREQ_REF;
	double FreqWord=freq*AD9854_MaxFreqWord/temp;
	uint64_t FreqWord_i=FreqWord;
	int i=0;
	for(i=0;i<6;i++)
	{
		FreqSingle[5-i]=FreqWord_i&0x00000000000000FF;
		FreqWord_i>>=8;   
	}	
}
//******************************************************************

//��������:д���Զ���Ƶ�ʿ����֣�������Ƶ�ʿ����ּ���������
//�������:char *freq
//******************************************************************
void AD9854WriteFreqWord1(unsigned char *freq)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE1) ;              // 0x02��ӦдFrq1
	AD9854WriteByte(freq);            					// д6���ֽڵ�����,�ȷ���λ
	AD9854WriteByte(freq+1);
	AD9854WriteByte(freq+2);
	AD9854WriteByte(freq+3);
	AD9854WriteByte(freq+4);
	AD9854WriteByte(freq+5);
	delay_us(1);
	AD9854_CS_H;
}

void AD9854WriteFerqWord2(unsigned char *freq)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE2);              // 0x03��ӦдFrq2
	AD9854WriteByte(freq);            					// д6���ֽڵ�����,�ȷ���λ
	AD9854WriteByte(freq+1);
	AD9854WriteByte(freq+2);
	AD9854WriteByte(freq+3);
	AD9854WriteByte(freq+4);
	AD9854WriteByte(freq+5);
	delay_us(1);
	AD9854_CS_H;
}

uint64_t AD9854ReadFreqWord1(void )
{
	uint64_t temp=0;
	uint64_t DataRead=0;
	int i=0;
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addrRead+AD9854_FRE1);
	for(i=0;i<6;i++)
	{
		DataRead>>=8;
		temp=AD9854ReadByte();
		DataRead=(temp<<40)&0x0000FF0000000000;////���������ɣ�����
	}
	return DataRead;
}

uint64_t AD9854ReadFreqWord2(void )
{
	uint64_t temp=0;
	int i=0;
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE2);
	for(i=0;i<6;i++)
	{
		temp<<=8;
		temp=AD9854ReadByte();
	}
	return temp;
}

void AD9854WritePhaseWord1(unsigned char *phase)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_PHA1); 
	AD9854WriteByte(phase); 
	AD9854WriteByte(phase+1);
	delay_us(1);
	AD9854_CS_H;
}

void AD9854WritePhaseWord2(unsigned char *phase)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_PHA2);              
	AD9854WriteByte(phase);
	AD9854WriteByte(phase+1);
	delay_us(1);
	AD9854_CS_H;
}

 //******************************************************************

 //��������:ɨƵģʽ��д����Ƶ��
 //�������:char *freqst
 //******************************************************************
void AD9854WriteFreqStep(unsigned char *freqstep)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_DELTA);
	AD9854WriteByte(freqstep);
	AD9854WriteByte(freqstep+1);
	AD9854WriteByte(freqstep+2);
	AD9854WriteByte(freqstep+3);
	AD9854WriteByte(freqstep+4);
	AD9854WriteByte(freqstep+5);
	delay_us(1);
	AD9854_CS_H;
}
//******************************************************************

  //******************************************************************

  //��������:ɨƵģʽ��дÿ��Ƶ�ʵ�פ��ʱ��
  //�������:char *freqstay
  //******************************************************************
void AD9854WriteFreqStay(unsigned char *freqstay)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_RAMP_CLK);
	AD9854WriteByte(freqstay);
	AD9854WriteByte(freqstay+1);
	AD9854WriteByte(freqstay+2);	
	delay_us(1);
	AD9854_CS_H;
}
 //******************************************************************

//��������:һ�θ�������
//�������:��
//******************************************************************
void Update_AD9854(void)
{
	AD9854_UPDATE_H;
	delay_us(2);
	AD9854_UPDATE_L;
}
//******************************************************************


//��������:IO��λ������ͨ��ͬ��
//�������:��
//******************************************************************
void AD9854IOReset(void)  //һ��IO��λ
{
	AD9854_IO_RESET_H;
	delay_us(20);
	AD9854_IO_RESET_L;  
}
//******************************************************************



//��������:���ÿ��ƼĴ��� CTR_REG (control function register)
//�������:ctr(ָ��,ָ��unsigned char��������׵�ַ,���鳤��Ϊ32λ,4���ֽ�)
//******************************************************************

void AD9854WriteCtR(unsigned char *ctr)   //���üĴ�����ÿ�ζ�Ҫ��д��ַ���д���ݬ
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_CTR_REG);//��д��ַ
	AD9854WriteByte(ctr);         // д4���ֽڵ�����,�ȷ���λ
	AD9854WriteByte(ctr+1);
	AD9854WriteByte(ctr+2);
	AD9854WriteByte(ctr+3);	
	delay_us(1);
	AD9854_CS_H;
}
//******************************************************************


//��������:AD9854��ʼ��
//�������:��
//Ӳ��˵����
//******************************************************************
void AD9854Init(void)
{
	AD9854PortInit();
	AD9854Reset();
	AD9854WriteCtR(ctr);
   delay_ms(50);
}

//******************************************************************

//��������:���ų�ʼ��
//�������:��
//Ӳ��˵������ͨ���궨���޸ģ�����ĸ�IO�ڶ�����

//******************************************************************
void AD9854PortInit(void)
{
	GPIO_InitTypeDef GPIO_AD9854;
	RCC_APB2PeriphClockCmd(RCC_AD9854_CS, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AD9854_SCLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AD9854_UPDATE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AD9854_IO_RESET, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AD9854_SDIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AD9854_RST, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AS9854_SDO, ENABLE);

	GPIO_AD9854.GPIO_Mode=GPIO_Mode_Out_PP;    //�������
	//GPIO_AD9854.GPIO_OType=GPIO_OType_PP;      
	GPIO_AD9854.GPIO_Speed=GPIO_Speed_50MHz;
	//GPIO_AD9854.GPIO_PuPd = GPIO_PuPd_DOWN;  //�����������������Ӱ��

	GPIO_AD9854.GPIO_Pin=AD9854_CS_Pin;
	GPIO_Init(AD9854_CS_GPIO,&GPIO_AD9854);  //CS���

	GPIO_AD9854.GPIO_Pin=AD9854_SCLK_Pin;
	GPIO_Init(AD9854_SCLK_GPIO,&GPIO_AD9854);  //SCLK���

	GPIO_AD9854.GPIO_Pin=AD9854_SDIO_Pin;
	GPIO_Init(AD9854_SDIO_GPIO,&GPIO_AD9854);  //SDIO���

	GPIO_AD9854.GPIO_Pin=AD9854_UPDATE_Pin;
	GPIO_Init(AD9854_UPDATE_GPIO,&GPIO_AD9854);  //UODATE���

	GPIO_AD9854.GPIO_Pin=AD9854_IO_RESET_Pin;
	GPIO_Init(AD9854_IO_RESET_GPIO,&GPIO_AD9854);   //RESET���
	
	GPIO_AD9854.GPIO_Pin=AD9854_RST_Pin;
	GPIO_Init(AD9854_RST_GPIO,&GPIO_AD9854);  //RST���

	GPIO_AD9854.GPIO_Pin=AD9854_SDO_Pin;
	GPIO_AD9854.GPIO_Mode=GPIO_Mode_IPD;  //SDO��������
	GPIO_Init(AD9854_SDO_GPIO,&GPIO_AD9854);
	
	AD9854_CS_L;     
	AD9854_SDIO_L;
	AD9854_SCLK_L;
	AD9854_UPDATE_L;
	AD9854_IO_RESET_L;
	AD9854_RST_H;
}
//******************************************************************

//��������:����ģʽ������I��Qͨ����������ȣ��˺�������Ҫ���ƼĴ����п������μ��ز���Ч
//�������:char *IA,char *QA
//˵�����ֱ�Ϊ12λ��������д�룬��д��λ

//******************************************************************
void AMP_SETUP(unsigned char *IA,unsigned char *QA)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_I_MUL);
	AD9854WriteByte(IA);
	AD9854WriteByte(IA+1);
	Update_AD9854();
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_Q_MUL);
	AD9854WriteByte(QA);
	AD9854WriteByte(QA+1);
	delay_us(1);
	Update_AD9854();
  AD9854_CS_H;
}


void AD9854SetAmp(uint16_t channel1Amp,uint16_t channel2Amp)
{
   u8 IAmp[2],QAmp[2];
   IAmp[1]=channel1Amp&0xff;  //��8λ��
   IAmp[0]=(channel1Amp>>8)&0xff;  //��8λ
   QAmp[1]=(channel2Amp)&0xff;     //��8λ��
   QAmp[0]=(channel2Amp>>8)&0xff;  //��8λ
   AMP_SETUP(IAmp,QAmp);
}

void AD9854Reset(void)
{
	AD9854_RST_L;
	delay_ms(2);
	AD9854_RST_H;
	delay_ms(10);
	AD9854_RST_L;
}


//��������:д8λ�����ֻ�������
//MSB ���λ����(9854Ĭ��)
//SDIOΪ˫��������(9854Ĭ��)
//******************************************************************
void AD9854WriteByte(unsigned char *data)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		if((*data<<i)&0x80)		//MSB first Mode
			AD9854_SDIO_H;
		else   
			AD9854_SDIO_L;   //��dataд��SDIO
		delay_us(1);
		AD9854_SCLK_H;
		delay_us(1);
		AD9854_SCLK_L;  //ÿдһ�ο���һ��SCLK
	}
}

u8 AD9854ReadByte(void)
{
	u8 i=0;
	u8 ByteRead=0;
	for(i=0;i<8;i++)
	{
		ByteRead=ByteRead<<1;
		delay_us(1);
		AD9854_SCLK_H;
		delay_us(1);
		if(AD9854_SDO_INPUT)
			ByteRead|=0x01;
		else
			ByteRead|=0x00;
		AD9854_SCLK_L;
	}
	return ByteRead;
}

//******************************************************************

