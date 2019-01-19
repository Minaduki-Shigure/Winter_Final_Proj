#ifndef __SPIX_H
#define __SPIX_H 			   
#include <sys.h>

#define SPI_PORT 	GPIOA
#define SPI_CS   	GPIO_Pin_2
#define SPI_SCLK 	GPIO_Pin_3
#define SPI_MISO 	GPIO_Pin_4
#define SPI_MOSI 	GPIO_Pin_5
#define SPI_FLAG 	GPIO_Pin_6
#define RCC_SPI  	RCC_APB2Periph_GPIOA

void SPIx_Init(void);
uint64_t SPIx_SendReadByte36(uint64_t byte);
uint64_t SPIx_SendReadByte16(uint64_t byte);

#endif
