#include <message.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <i2c.h> 
#include <panic.h> 
#include <pio.h>
#include "accelerometer.h"
  
static uint8 dcount=0;

void LIS2DH_init(void)
{  
    uint8 read_value = 0;
    read_value = readRegister(LIS2DH_WHO_AM_I);
   printf("Accel Init - WHO_AM_I_VAL : [0x%x]\n", read_value);  
    
    if(read_value == LIS2DH_WHO_AM_I_VAL)
    {
        
        standby();
        setupTap();
        active();  
        /* printf("I am here 1"));*/
       /* MessageSendLater(&theSink.task, SB_I2C_READ, 0, 5); */
       /* printf("I am here 2"));*/
    }
    else 
    {
        standby();
        printf("Accel in Stanby\n");
    }
    
}


void standby(void)
{
    	
    uint8 c = readRegister(LIS2DH_CTRL_REG1);
    printf("calling standby\n");
    printf("control reg b4 writing : [0x%x]\n",c);
    
    writeRegister(LIS2DH_CTRL_REG1, c & (0x00)); /*Clear the ODR bits to go into power down mode*/
    c = readRegister(LIS2DH_CTRL_REG1);
    printf("control reg after writing : [0x%x]\n",c);
}

/*SET ACTIVE MODE
	Sets the LIS2DH to active mode with ODR at 400Hz. Needs to be in this mode to output data*/
void active(void)
{
	uint8 c = readRegister(LIS2DH_CTRL_REG1);
    printf("control reg b4 writing :[0x%x]\n",c);
	writeRegister(LIS2DH_CTRL_REG1, c | 0x77); /*Set the ODR bits to begin at the data rate of 400Hz and enable x, y , z axis*/
    c = readRegister(LIS2DH_CTRL_REG1);
    printf("control reg after writing CTRL_REG1: [0x%x]\n",c);
    
    c = 0;
    c = readRegister(LIS2DH_CTRL_REG4);
    printf("control reg b4 writing CTRL_REG4: [0x%x]\n",c);
	writeRegister(LIS2DH_CTRL_REG4, c | 0x08); /*Set High Resolution Mode for 12 bit output*/
    c = readRegister(LIS2DH_CTRL_REG4);
    printf("control reg after writing CTRL_REG4 : [0x%x]\n",c);
}


uint8 AccAvailable(void)
{
	return (readRegister(LIS2DH_STATUS) & 0x08) >> 3;
}

void AccRead(void)
{
    int Ax, Ay, Az;
    long cx, cy, cz;
	uint8 rawData[6]; /* x/y/z accel register data stored here */
    
    rawData[0] = readRegister(LIS2DH_OUT_X_L);
    rawData[1] = readRegister(LIS2DH_OUT_X_H);    
    rawData[2] = readRegister(LIS2DH_OUT_Y_L);
    rawData[3] = readRegister(LIS2DH_OUT_Y_H);
    rawData[4] = readRegister(LIS2DH_OUT_Z_L);
    rawData[5] = readRegister(LIS2DH_OUT_Z_H);
    
	Ax = (int)(rawData[1]<<8|rawData[0])>>4; /*Output data is 12bit left justified*/
    Ay = (int)(rawData[3]<<8|rawData[2])>>4;
	Az = (int)(rawData[5]<<8|rawData[4])>>4;
     
    cx = ((long)Ax*1); /* changes according to sensitivity parameters check pg. no. 10 data calcculated is in miligravity*/
	cy =  ((long)Ay*1);
	cz =  ((long)Az*1);

    printf("\nCx:%ld\tCy:%ld\tCz:%ld\t",cx,cy,cz);
}



void setupTap(void)
{
    uint8 c = readRegister(LIS2DH_CTRL_REG3);
    printf("reg b4 writing LIS2DH_CTRL_REG3 : [0x%x]\n",c);
	writeRegister(LIS2DH_CTRL_REG3,0x80); /*Configuring click interrupt on INT1 */ 
    c = readRegister(LIS2DH_CTRL_REG3);
    printf("reg after writing LIS2DH_CTRL_REG3: [0x%x]\n",c);
    
    c = readRegister(LIS2DH_CLICK_CFG);
    printf("reg b4 writing LIS2DH_CLICK_CFG : [0x%x]\n",c);
	writeRegister(LIS2DH_CLICK_CFG,0x20); /*Configuring for double tap */ 
    c = readRegister(LIS2DH_CLICK_CFG);
    printf("reg after writing LIS2DH_CLICK_CFG: [0x%x]\n",c);
    
    c = readRegister(LIS2DH_CLICK_THS);
    printf("reg b4 writing LIS2DH_CLICK_THS : [0x%x]\n",c);
    writeRegister(LIS2DH_CLICK_THS,0x59); /* Setting threshold value */
    c = readRegister(LIS2DH_CLICK_THS);
    printf("reg after writing LIS2DH_CLICK_THS: [0x%x]\n",c);
  
    c = readRegister(LIS2DH_TIME_LIMIT);
    printf("reg b4 writing LIS2DH_TIME_LIMIT : [0x%x]\n",c);
    /* Setting the time limit for tap 30m sec*/
    writeRegister(LIS2DH_TIME_LIMIT, 0x04); /*1 LSB = 1/ODR, so now our odr is 400Hz, which means 1 LSB in this register means 2.5msec, so for 127msec value calculated is 0x33*/  
    c = readRegister(LIS2DH_TIME_LIMIT);
    printf("reg after writing LIS2DH_TIME_LIMIT: [0x%x]\n",c);    
    
    
   
   writeRegister(LIS2DH_TIME_LATENCY, 0x50); /* Setting time latency at 637 msec */
   writeRegister(LIS2DH_TIME_WINDOW, 0x50);  /* Setting time window at 637 msec */
   
}

uint8 readTap(void)
{
    uint8 temp;
	temp =(0x7F&readRegister(LIS2DH_CLICK_SRC));
   /* if(temp)
    {
       
	printf("LIS2DH_CLICK_SRC : 0x%x\n",temp);
    }*/
   
    
        if((temp&0x20)>>5)
        {
            printf("Double tap detected [%d]\n",++dcount);
            
        }
      /*  else if((temp&0x10)>>4)
        {
            printf("Single tap detected %d\n",++scount);
            
        }*/
     
     
    
    return 0;
}


uint8 readRegister(uint8 reg)
{
    
    uint8 i2c_data[1]; 
    uint8 i2c_rx_data[2]; 
    i2c_data[0] = reg; 
    
    PanicZero(I2cTransfer(LIS2DH_I2C_ADDR_R, i2c_data, 1, i2c_rx_data, 2)); 
    return ((i2c_rx_data[0])); /* 1st byte is data and 2nd byte is ack */
    
}


void writeRegister(uint8 reg, uint8 data)
{
    uint8 i2c_data[2];  
    i2c_data[0] = reg; 
    i2c_data[1] = data;
 
    /* 
    I2cTransfer(uint16 address, const uint8 *tx, uint16 tx_len, uint8 *rx, uint16 rx_len) 
    If tx_len is non-zero and rx_len is zero then the sequence reduces to: 
    - Start condition (S). 
    - Write slave address and direction byte (address | 0). 
    - Write tx_len data bytes from the buffer at tx. 
    - Stop condition (P). 
    */ 
    PanicZero(I2cTransfer(LIS2DH_I2C_ADDR_W, i2c_data, 2, NULL, 0)); 
    
}


