

#include "MPU6000_lib.h"
#include "sensors.h"

float Gyro_Scale_Factor;

extern int imu_buf_ax[IMU_BUFFER_LENGTH]; 
extern int imu_buf_ay[IMU_BUFFER_LENGTH]; 
extern int imu_buf_az[IMU_BUFFER_LENGTH]; 
extern int imu_buf_gx[IMU_BUFFER_LENGTH];  			
extern int imu_buf_gy[IMU_BUFFER_LENGTH];
extern int imu_buf_gz[IMU_BUFFER_LENGTH];
extern int imu_buf_te[IMU_BUFFER_LENGTH];

unsigned int short SPI2_high, SPI2_low;
unsigned int SPI2_i, SPI2_j;
int idx = 0;

void MPU6000_conf(void)
{
	MPUSPI_SS = 1;      									// Deassert MPU SS
	MPUSPI_TRIS = 0;    									// Make MPU SS  an output


    SPI2_config(SEC_PRESCAL_4_1, PRI_PRESCAL_16_1);
	__delay32(2400000);  									// Need at least 60 msec delay here

	MPU6000_write(MPUREG_PWR_MGMT_1, BIT_H_RESET);
	__delay32(400000); 										// 10msec delay 


	// Wake up device and select GyroZ clock (better performance)
	MPU6000_write(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);


	// Disable I2C bus (recommended on datasheet)
	MPU6000_write(MPUREG_USER_CTRL, BIT_I2C_IF_DIS);


	// Sample rate
	MPU6000_write(MPUREG_SMPLRT_DIV, 0); // SAMPLE RATE = GYRO FREQ


	// Scaling & DLPF
	MPU6000_write(MPUREG_CONFIG, BITS_DLPF_CFG_188HZ);


#if(GYRO_RANGE == 250)
	MPU6000_write(MPUREG_GYRO_CONFIG, BITS_FS_250DPS);  // Gyro scale 250  º/s
    Gyro_Scale_Factor = 131.0;
#elif(GYRO_RANGE == 500)
	MPU6000_write(MPUREG_GYRO_CONFIG, BITS_FS_500DPS);  // Gyro scale 500  º/s
    Gyro_Scale_Factor = 65.5;
#elif(GYRO_RANGE == 1000)
	MPU6000_write(MPUREG_GYRO_CONFIG, BITS_FS_1000DPS);  // Gyro scale 1000 º/s
    Gyro_Scale_Factor = 32.8;
#elif(GYRO_RANGE == 2000)
	MPU6000_write(MPUREG_GYRO_CONFIG, BITS_FS_2000DPS); // Gyro scale 2000 º/s
    Gyro_Scale_Factor = 16.4;
#else
#error "Invalid GYRO_RANGE"
#endif


    // Accelerometer range = 2g 
	MPU6000_write(MPUREG_ACCEL_CONFIG, BITS_FS_2G);  
    

	// INT CFG => Interrupt on Data Ready, totem-pole (push-pull) output
	MPU6000_write(MPUREG_INT_PIN_CFG, BIT_INT_LEVEL | BIT_INT_RD_CLEAR); // INT: Clear on any read
	MPU6000_write(MPUREG_INT_ENABLE, BIT_DATA_RDY_EN); // INT: Raw data ready



   // AFTER THE CONFIGURATION WE CAN BOOST THE SPEED OF SPI2 BUS UP TO 10MHz 
    
    SPI2_config(SEC_PRESCAL_1_1, PRI_PRESCAL_4_1);
	__delay32(2400000);  	// need at least 60 msec delay here



    _TRISMPUINT = 1;									 // MPU INTERRUPT PIN AS INPUT
	_INT3EP = 1; 									     // Set INT3 pin to interrupt on falling edge
	_INT3IP = INT_PRI_INT3;
	_INT3IF = 0; 										 // Reset INT3 interrupt flag
	

};

void MPU6000_read()  	//Burst Read
{
	
	unsigned int dummy_read;
    unsigned int addr = MPUREG_ACCEL_XOUT_H;
 


	SPI2_SS = 0;  														// Assert chip select

	SPI2_i = 0;
	SPI2_j = 0;
    addr |= 0x80;       		  										// Address modification for reading
	dummy_read = SPI2BUF;    	  										// Dummy read to clear the buffer
	SPI2BUF = addr << 8;    	  										// This cause the interrupt below

// Scrivo l'indirizzo di acc_x sul buffer, dopodichè appena il sensore ha i dati pronti parte l'interrupt routine la prima volta (MPUINT sul pin RA14)
};

void SPI2_config(unsigned int priPre, unsigned int secPre)
{
	

    SPI2STATbits.SPIEN = 0;        										// Disable the SPI module
	             
    IEC2bits.SPI2IE    = 0;        										// Disable the Interrupt

    
    SPI2CON1bits.DISSDO = 0;     										// SDO2 pin is on	
    SPI2CON1bits.MODE16 = 1;       										// Communication is word-wide
    SPI2CON1bits.DISSCK = 0;       										// SCK2 pin bit is on
    SPI2CON1bits.CKE    = 0; 
    SPI2CON1bits.SSEN   = 0; 	   										// Slave enable off
    SPI2CON1bits.CKP    = 1; 
    SPI2CON1bits.MSTEN  = 1;
    SPI2CON1bits.SMP    = 1;       										// Data sampled at the end (ALWAYS AFTER MSTEN)
    SPI2CON1bits.SPRE = secPre;							   
    SPI2CON1bits.PPRE = priPre;    
    SPI2CON2          = 0x0000;    										// Frame mode off
    
      
    SPI2STATbits.SPIEN   = 1;       									// Ensable the SPI module
    SPI2STATbits.SPISIDL = 0;       									// Continue operation in idle mode 
    SPI2STATbits.SPIROV  = 0;       									// No overflow
    
    
    IPC8bits.SPI2IP	   = INT_PRI_SPI2;		   							// Interrupt priority
    IFS2bits.SPI2IF    = 0;            									// Int flag cleared  
    IEC2bits.SPI2IE    = 1;            									// Enable the Interrupt  
   
};

void MPU6000_write(unsigned int addr, unsigned int data)
{
	int k;
	
	SPI2_SS = 0;  														// Assert chip select (ON)

    k = SPI2BUF;
	SPI2BUF = addr << 8 | data; 										// Send address and data (8+8 bit)


	for (k = 0; k < 200; k++)
	if (SPI2STATbits.SPIRBF) break;

	k = SPI2BUF;

 
	SPI2_SS = 1; 														// Deassert chip select (OFF)
	__delay32(40); 														// 1us

    // This delay of 1us is necessary; it appears that SS must be deasserted for one or
	// more SPI clock cycles between writes
};

void __attribute__((interrupt, no_auto_psv)) _INT3Interrupt(void)
{
	_INT3IF = 0; 										 // Clear the INT3 interrupt flag
	MPU6000_read();
};


void __attribute__((__interrupt__, __no_auto_psv__)) _SPI2Interrupt(void)
{

	_SPI2IF = 0;  
	 
	unsigned int SPIBUF;
	
//	int current_cpu_ipl;
 
    //SET_AND_SAVE_CPU_IPL(current_cpu_ipl,7);
    if (SPI2_i == 0)    
          {
		   SPIBUF    = SPI2BUF;         
		   SPI2BUF   = 0x0000;             		// THIS WRITE CALLS AGAIN THE ISR AND AT THE END OF THIS "IF" THE ROUTINE STARTS AGAIN,THIS TIME WITH SPI2_i =1;
		   SPI2_high = 0xFF & SPIBUF;    		// SPIBUF HAS THE FOLLOWING STRUCTURE : SOMETHING WE IGNORE(8 bit) + ACCEL_XOUT_H (8 bit)  
       	   SPI2_i    = 1;	                 
          }

    else if (SPI2_i < 7)
    {
	    SPIBUF    = SPI2BUF;
		SPI2BUF   = 0x0000;
		SPI2_low  = SPIBUF >> 8; 				// 16 bit received. The first 8 bits are the LOW bits of the previous measure. The second 8 bits are the HIGH bits for the current one	    
           
        switch(SPI2_j)
  		{
	  		case 0:
         	*(imu_buf_ax + idx)	= SPI2_high << 8 | SPI2_low;      	//THIS STORES THE REBUILT 16 BIT MEASURE IN VECTOR "data" IN THE Jth POSITION   
 			break;

        	case 1 :
         	*(imu_buf_ay + idx)	= SPI2_high << 8 | SPI2_low;          
        	break;
        
        	case 2 :
         	*(imu_buf_az + idx) = SPI2_high << 8 | SPI2_low;   
        	break;
         
        	case 3 :
         	*(imu_buf_te + idx) = SPI2_high << 8 | SPI2_low;    
        	break;
      
        	case 4:
         	*(imu_buf_gx + idx) = SPI2_high << 8 | SPI2_low;           
        	break;
        
        	case 5 :
         	*(imu_buf_gy + idx) = SPI2_high << 8 | SPI2_low;       
        	break;
  		} 
        SPI2_high = 0xFF & SPIBUF;
        SPI2_i++;    											// USED TO REPEAT THIS STRUCTURE
		SPI2_j++;    											// USED TO CHANGE THE ELEMENT OF "data" IN WICH THE MEASURE IS STORED (IN ORDER TO "FILL" THE ARRAY "data")
    }
    else
    {
	    SPIBUF = SPI2BUF;
		SPI2_low = SPIBUF >> 8; 								// GYRO_ZOUT_L

        if(SPI2_j==6)
		{
			*(imu_buf_gz + idx) = SPI2_high << 8 | SPI2_low;
           	idx++;
	 		idx = idx % 20; 									// When idx = 20, idx % 20 is equal to 1 with "resto" = 0 so idx =0;
        }
      	SPI2_SS = 1;	
    }	 
    //RESTORE_CPU_IPL(current_cpu_ipl);  
	       
};

/*
Parte l'ISR non appena il sensore ha il primo dato pronto : 
1) SPI2_i=0 quindi viene eseguito il relativo if
2) Esco dalla routine e attendo che il nuovo dato sia pronto
3) Il sensore ha i nuovi dati e rientro nell'ISR
4) SPI2_i = 1 quindi eseguo l'else if
5) Uscita e rientro dalla ISR si ripetono fino a quando non acquisisco le 7 misure
*/ 
