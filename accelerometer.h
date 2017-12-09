#ifndef __SB_ACCELEROMETER_H
#define __SB_ACCELEROMETER_H

#define ACC_INT_1        (1 << 5)        /* PIO5 is INT1 */
#define READ_ACC         (0x4545)

/* LIS2DH I2C address */
#define LIS2DH_I2C_ADDR_R       (0x33) 
#define LIS2DH_I2C_ADDR_W       (0x32) 

/* LIS2DH Register addresses */
#define LIS2DH_WHO_AM_I       (0x0F)   

/* LIS2DH Who am I register value */
#define LIS2DH_WHO_AM_I_VAL   (0x33)   

#define LIS2DH_CTRL_REG1 (0x20)          
#define LIS2DH_STATUS (0x27)  

#define LIS2DH_OUT_X_L (0x28)
#define LIS2DH_OUT_X_H (0x29)
#define LIS2DH_OUT_Y_L (0x2A)
#define LIS2DH_OUT_Y_H (0x2B)
#define LIS2DH_OUT_Z_L (0x2C)
#define LIS2DH_OUT_Z_H (0x2D)

#define LIS2DH_TEMP_L (0x0C) 
#define LIS2DH_TEMP_H (0x0D) 


#define LIS2DH_CTRL_REG3 (0x22)
#define LIS2DH_CTRL_REG4 (0x23)
#define LIS2DH_CTRL_REG5 (0x24)



#define LIS2DH_INT1_CFG (0x30)
#define LIS2DH_INT1_SRC (0x31)
#define LIS2DH_INT1_DURATION (0x33)

#define LIS2DH_INT1_THS (0x32)

#define LIS2DH_CLICK_CFG  (0x38)
#define LIS2DH_CLICK_SRC  (0x39)
#define LIS2DH_CLICK_THS  (0x3A)

#define LIS2DH_TIME_LIMIT   (0x3B)
#define LIS2DH_TIME_LATENCY (0x3C) 
#define LIS2DH_TIME_WINDOW  (0x3D)
        



#define LIS2DH_CTRL_REG6 (0x25)



#define LIS2DH_IG_CFG2 (0x36)
#define LIS2DH_IG_SRC2 (0x37)
#define LIS2DH_IG_DUR2 (0x39)


void LIS2DH_init(void);
void standby(void);
void active(void);

void setupTap(void);
uint8 readTap(void);

uint8 AccAvailable(void);
void AccRead(void);

/*void TempRead(void);*/

uint8 readRegister(uint8 reg);
void writeRegister(uint8 reg, uint8 data);

typedef struct
{
TaskData task;
} AccReadTask;



#endif
