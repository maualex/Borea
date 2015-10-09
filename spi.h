#ifndef __33FJ256GP710A_H
#include <p33FJ256GP710A.h>
#endif

//macro per SPI
#define SPI_R SPI1STATbits.SPIRBF
#define SPI_T SPI1STATbits.SPITBF
#define SPI_O SPI1STATbits.SPIROV
#define SPI_MODULE SPI1STATbits.SPIEN
#define CS_X _LATA5
#define CS_Y _LATA1
#define CS_Z _LATA4
//build SPI command
#define sensor_data 0x20000000 //0xF0000000


void config_SPI(void);
unsigned long SPI_send(unsigned long input); 
unsigned char read_gyro(unsigned char channel, int *measure ); 
