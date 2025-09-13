
#include "mpu6050.h"

#define RAD_TO_DEG 57.29578f;


float dt = 0.0001f; // 采样时间间隔

void MPU6050::MPU6050_INIT()
{
    int i = 0, j = 0;

    // 延时
    HAL_Delay(100);

    uint8_t SendAddress = 0x6b;

    uint8_t SendData = 0x00; // 解除休眠状态

    HAL_I2C_Mem_Write(&hi2c1, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x19; // 采样率分频器

    SendData = 0x07;

    HAL_I2C_Mem_Write(&hi2c1, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x1A; // 低通滤波器

    SendData = 0x06;

    HAL_I2C_Mem_Write(&hi2c1, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x1B; // 陀螺仪

    SendData = 0x08; // ± 500 °/s

    HAL_I2C_Mem_Write(&hi2c1, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x1C; // 加速度计

    SendData = 0x00; // ± 2g

    HAL_I2C_Mem_Write(&hi2c1, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

}

void MPU6050::ReadData()
{
    HAL_I2C_Mem_Read(&hi2c1, 0xD1, preg1_Data, I2C_MEMADD_SIZE_8BIT, MPU_Data, 14, 50);

    float Temp = (MPU_Data[6] << 8) | MPU_Data[7];

    // 防止数据溢出
    if (Temp > 32768)
    {
        Temp -= 65536;
    }

    Temp = (36.53 + Temp / 340);

    short int a_x = ((MPU_Data[0] << 8) | MPU_Data[1]);

    imudata.ax = (double)a_x / 16384;// 单位g

    short int a_y = ((MPU_Data[2] << 8) | MPU_Data[3]);

    imudata.ay = (double)a_y / 16384;

    short int a_z = ((MPU_Data[4] << 8) | MPU_Data[5]);

    imudata.az = (double)a_z / 16384;

    short int g_x = ((MPU_Data[8] << 8) | MPU_Data[9]);

    imudata.gx = (double)g_x / 65.5;// 单位°/s

    short int g_y = ((MPU_Data[10] << 8) | MPU_Data[11]);

    imudata.gy = (double)g_y / 65.5;

    short int g_z = ((MPU_Data[12] << 8) | MPU_Data[13]);

    imudata.gz = (double)g_z / 65.5;
}

void MPU6050::Euler_Caculte(float &yaw, float &roll, float &pitch)
{

    if (!bool_calibration_flag)
    {
        calibration_flag++;
        gz_bassis += imudata.gz;
    }

    if (calibration_flag >= 100)
    {
        calibration_flag = 0;
        bool_calibration_flag = true;
        gz_bassis /= 100;
    }

    if (bool_calibration_flag)
    {
        pitch = atan2f(-imudata.ax, sqrtf(imudata.ay * imudata.ay + imudata.az * imudata.az)) * RAD_TO_DEG;

        roll = atan2(imudata.ax, imudata.az) * RAD_TO_DEG;

        yaw += (imudata.gz - gz_bassis) * dt;

        if (yaw >= 180.0f)
        {
            yaw -= 360;
        }
        else if (yaw <= -180.0f)
        {
            yaw += 360;
        }
    }
}

void MPU6050 ::MPU6050_Callback()
{
    ReadData();

    Euler_Caculte(Yaw, Roll, Pitch);

    
}