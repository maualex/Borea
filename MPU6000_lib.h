#ifndef __33FJ256GP710A_H
#include <p33FJ256GP710A.h>
#endif

               /**** SPI PROTOTYPE FUNCTIONS ****/

void SPI2_config( unsigned int,  unsigned int);
extern void __delay32(unsigned long cycles); //min value = 12;



                     
                      /**** DEFINES ****/

#define  FCY 40000000UL                   // CLOCK FREQUENCY FOR DELAY32
#define  SEC_PRESCAL_1_1        0xffff    // Secondary Prescale 1:1   
#define  SEC_PRESCAL_2_1        0xfffb    // Secondary Prescale 2:1   
#define  SEC_PRESCAL_3_1        0xfff7    // Secondary Prescale 3:1   
#define  SEC_PRESCAL_4_1        0xfff3    // Secondary Prescale 4:1   
#define  SEC_PRESCAL_5_1        0xffef    // Secondary Prescale 5:1   
#define  SEC_PRESCAL_6_1        0xffeb    // Secondary Prescale 6:1   
#define  SEC_PRESCAL_7_1        0xffe7    // Secondary Prescale 7:1   
#define  SEC_PRESCAL_8_1        0xffe3    // Secondary Prescale 8:1   
#define  PRI_PRESCAL_1_1        0xffff    // Primary Prescale 1:1     
#define  PRI_PRESCAL_4_1        0xfffe    // Primary Prescale 4:1     
#define  PRI_PRESCAL_16_1       0xfffd    // Primary Prescale 16:1    
#define  PRI_PRESCAL_64_1       0xfffc    // Primary Prescale 64:1    

#define INT_PRI_SPI2    5                 // SPI2 PRIORITY
#define INT_PRI_INT3    5                 // INT3 PIN PRIORITY (MPU6000 INTERRUPT PIN)

#define _TRISMPUINT TRISAbits.TRISA14     // INT3 PIN AS INPUT


#define SPI2_SS              _LATG9        // CHIP SELECT VALUE
#define SPI2_TRIS            _TRISG9       // CHIP SELECT DIRECTION
#define MPUSPI_SS            SPI2_SS  
#define MPUSPI_TRIS          SPI2_TRIS 



                   /**** MPU-6000 REGISTERS ****/

#define MPUREG_AUX_VDDIO            0x01
#define MPUREG_XG_OFFS_TC           0x00
#define MPUREG_YG_OFFS_TC           0x01
#define MPUREG_ZG_OFFS_TC           0x02
#define MPUREG_X_FINE_GAIN          0x03
#define MPUREG_Y_FINE_GAIN          0x04
#define MPUREG_Z_FINE_GAIN          0x05
#define MPUREG_XA_OFFS_H            0x06
#define MPUREG_XA_OFFS_L            0x07
#define MPUREG_YA_OFFS_H            0x08
#define MPUREG_YA_OFFS_L            0x09
#define MPUREG_ZA_OFFS_H            0x0A
#define MPUREG_ZA_OFFS_L            0x0B
#define MPUREG_PRODUCT_ID           0x0C
#define MPUREG_XG_OFFS_USRH         0x13
#define MPUREG_XG_OFFS_USRL         0x14
#define MPUREG_YG_OFFS_USRH         0x15
#define MPUREG_YG_OFFS_USRL         0x16
#define MPUREG_ZG_OFFS_USRH         0x17
#define MPUREG_ZG_OFFS_USRL         0x18
#define MPUREG_SMPLRT_DIV           0x19
#define MPUREG_CONFIG               0x1A
#define MPUREG_GYRO_CONFIG          0x1B
#define MPUREG_ACCEL_CONFIG         0x1C
#define MPUREG_INT_PIN_CFG          0x37
#define MPUREG_INT_ENABLE           0x38
#define MPUREG_ACCEL_XOUT_H         0x3B
#define MPUREG_ACCEL_XOUT_L         0x3C
#define MPUREG_ACCEL_YOUT_H         0x3D
#define MPUREG_ACCEL_YOUT_L         0x3E
#define MPUREG_ACCEL_ZOUT_H         0x3F
#define MPUREG_ACCEL_ZOUT_L         0x40
#define MPUREG_TEMP_OUT_H           0x41
#define MPUREG_TEMP_OUT_L           0x42
#define MPUREG_GYRO_XOUT_H          0x43
#define MPUREG_GYRO_XOUT_L          0x44
#define MPUREG_GYRO_YOUT_H          0x45
#define MPUREG_GYRO_YOUT_L          0x46
#define MPUREG_GYRO_ZOUT_H          0x47
#define MPUREG_GYRO_ZOUT_L          0x48
#define MPUREG_USER_CTRL            0x6A
#define MPUREG_PWR_MGMT_1           0x6B
#define MPUREG_PWR_MGMT_2           0x6C
#define MPUREG_BANK_SEL             0x6D
#define MPUREG_MEM_START_ADDR       0x6E
#define MPUREG_MEM_R_W              0x6F
#define MPUREG_DMP_CFG_1            0x70
#define MPUREG_DMP_CFG_2            0x71
#define MPUREG_FIFO_COUNTH          0x72
#define MPUREG_FIFO_COUNTL          0x73
#define MPUREG_FIFO_R_W             0x74
#define MPUREG_WHOAMI               0x75



             /**** MPU-6000 CONFIGURATION BITS ****/

#define BIT_SLEEP                   0x40
#define BIT_H_RESET                 0x80
#define BITS_CLKSEL                 0x07
#define MPU_CLK_SEL_PLLGYROX        0x01
#define MPU_CLK_SEL_PLLGYROZ        0x03
#define MPU_EXT_SYNC_GYROX          0x02
#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18
#define BITS_FS_2G                  0x00
#define BITS_FS_4G                  0x08
#define BITS_FS_8G                  0x10
#define BITS_FS_16G                 0x18
#define BITS_FS_MASK                0x18
#define BITS_DLPF_CFG_256HZ_NOLPF2  0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07
#define BIT_I2C_IF_DIS              0x10

// Register 55 - INT Pin / Bypass Enable Configuration (INT_PIN_CFG)
#define BIT_INT_LEVEL               0x80
#define BIT_INT_OPEN                0x40
#define BIT_LATCH_INT_EN            0x20
#define BIT_INT_RD_CLEAR            0x10
#define BIT_FSYNC_INT_LEVEL         0x08
#define BIT_FSYNC_INT_EN            0x04
#define BIT_I2C_BYPASS_EN           0x02
#define BIT_CLKOUT_EN               0x01

// Register 56 - Interrupt Enable (INT_ENABLE)
#define BIT_FF_EN                   0x80
#define BIT_MOT_EN                  0x40
#define BIT_ZMOT_EN                 0x20
#define BIT_FIFO_OFLOW_EN           0x10
#define BIT_I2C_MST_INT_EN          0x08
#define BIT_DATA_RDY_EN             0x01

// Register 58 - Interrupt Status (INT_STATUS)
#define BIT_FF_INT                  0x80
#define BIT_MOT_INT                 0x40
#define BIT_ZMOT_INT                0x20
#define BIT_FIFO_OFLOW_INT          0x10
#define BIT_I2C_MST_INT             0x08
#define BIT_DATA_RDY_INT            0x01


// DMP output rate constants
#define MPU6000_200HZ               0    // default value
#define MPU6000_100HZ               1
#define MPU6000_66HZ                2
#define MPU6000_50HZ                3



                       /**** MPU-6000 PROTOTYPE FUNCTIONS ****/

void MPU6000_read(void);                     
void MPU6000_conf(void);
void MPU6000_write(unsigned int,  unsigned int data);

