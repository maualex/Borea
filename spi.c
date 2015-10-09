#include "spi.h"

unsigned long SPI_send(unsigned long input) //trasmette e riceve 32 bit
{
    unsigned int MSB_out, LSB_out, MSB_in, LSB_in;
    unsigned long out;
    
    MSB_out = input>>16;
    LSB_out = input;
    
    while(SPI_T==1) {};     //put timeout
    SPI1BUF = MSB_out;
    while(SPI_R==0) {}; //wait for receive the first word   //put timeout
    MSB_in  = SPI1BUF;
    
    while(SPI_T==1) {};  //put timeout
    SPI1BUF = LSB_out;
    while(SPI_R==0) {}; //wait for receive the second word  //put timeout
    LSB_in  = SPI1BUF;
    
    out = MSB_in;
    out = out<<16;
    out |= LSB_in;
    
    return out;
};

unsigned char read_gyro(unsigned char channel, int *measure )
{
    unsigned long respond = 0;
    unsigned char status, FAULT0, SQ, error_code;
    
    *measure    = -1;	
    error_code  = 1;
    
    if(channel==0)
    {
    CS_X    = 0;
    respond = SPI_send(sensor_data);
    CS_X    = 1;
    }
    else if(channel==1)
    {
    CS_Y    = 0;
    respond = SPI_send(sensor_data);
    CS_Y    = 1;
    }
    else if(channel==2)
    {
    CS_Z    = 0;
    respond = SPI_send(sensor_data);
    CS_Z    = 1;
    }
    
    
    
    /* message unpacking */
    status  = respond>>26;      //Status bits
    SQ      = status>>3;        //Sequence bits
    status &= 0b00000011;       //mask
    FAULT0 = respond>>1;
  
     /* check state */
    switch(status)
    {
        case 0 :                  // invalid data for sensor data response
        error_code = 2;  
        break;
        
        case 1 :                  // valid sensor data
        *measure      = respond>>10;
        break;
        
        case 2 :                  // sensor self-test data 
        
        
        case 3 :                  // R/W response
        if(SQ==0) error_code = 3;
  
        break;
    }
    return error_code;
};


void config_SPI(void)
{
    /***  SPI1 configuration  ***/
    SPI1STATbits.SPIEN      = 0;    //module SPI1 disabled
    SPI1STATbits.SPISIDL    = 0;    //continue in Idle mode
      
    SPI1STATbits.SPIROV     = 0;    //clear Receive Overflow Flag
    
    SPI1CON1bits.MODE16     = 1;    //communication is word-wide
    //clock at 5MHz
    
    SPI1CON1bits.PPRE       = 0b10;  //primary prescaler 4:1
    SPI1CON1bits.SPRE       = 0b110; //secondary prescaler 2:1
    SPI1CON1bits.MSTEN      = 1;    //Master Mode enabled
    SPI1CON1bits.SMP        = 0;    //input sampled in the middle
    SPI1CON1bits.CKP        = 0;    
    SPI1CON1bits.CKE        = 0;
    SPI1CON1bits.SSEN       = 0;    //SS pin not used by SPI1 module
    SPI1CON1bits.DISSCK     = 0;    //internal SPI clock enabled
    SPI1CON1bits.DISSDO     = 0;    //SDO2 pin is controlled by the SPI1 module
    
    SPI1CON2bits.FRMEN      = 0;    //framed support disabled
    
    //interrupt controller configuration
    IFS0bits.SPI1IF         = 0;    //interrupt flag cleared
    IEC0bits.SPI1IE         = 0;    //interrupt disabled
    IPC2bits.SPI1IP         = 2;    //interrupt priority
    
    
    //pin di uscita per i chip select
    _TRISA4  = 0;
    _TRISA1  = 0;
    _TRISA5  = 0;
    
    //all Chip Select signals high (gyroscopes disabled)
    CS_X    = 1;
    CS_Y    = 1;
    CS_Z    = 1;
};
