#ifndef _MPU6050_H
#define _MPU6050_H
/* 头文件 */
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "math.h"



    

extern I2C_HandleTypeDef hi2c1;


typedef struct 
{
float gx, gy, gz;
float ax, ay, az; 
}ImuData;


class MPU6050
{
    public:

        void MPU6050_INIT();

        void ReadData();

        void Euler_Caculte(float& yaw , float& roll , float& pitch);

        void MPU6050_Callback();

        inline float Get_Yaw_Angle();

    private:
        
        float angle = 0.0f;

        float Yaw = 0.0f ,
              Roll = 0.0f,
              Pitch = 0.0f;


        ImuData imudata;
        

        uint8_t MPU_Data[14] = {0X00, 0X00,0X00, 0X00,0X00, 0X00,0X00, 0X00,0X00, 0X00,0X00, 0X00,0X00, 0X00};

        uint8_t preg1_Data = 0x3B;
        
        
        float gz_bassis=0.0f;

        uint16_t calibration_flag=0;
        bool bool_calibration_flag=false;
};


float MPU6050::Get_Yaw_Angle()
{
    return Yaw;
}
#endif /* _MPU6050_H */