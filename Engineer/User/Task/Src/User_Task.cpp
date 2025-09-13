#include "User_Task.h"
#include "User_Tim.h"
#include "Class_Car.h"


Class_Car car;

uint32_t short_flag=0;

//float Distance = 0.0f;

void Task_1ms_Callback()
{
    short_flag++;

    //Distance = Get_Distance();
    car.Set_Distance(Get_Distance_Main());
    car.Class_Car_Callback();
    
}
extern "C" void Task_Init()
{


    
    car.Init();

    TIM_Init(&htim4,Task_1ms_Callback);

    //imu.MPU6050_INIT();
    HAL_TIM_Base_Start_IT(&htim4);
}


