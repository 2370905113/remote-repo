#include "task.h"
#include "drv_tim.h"
#include "drv_can.h"
#include "drv_dwt.h"
#include "dvc_djimotor.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal.h"

Class_DJI_Motor_GM6020 outpost;

void Task1ms_TIM2_Callback()
{
    #ifdef CHASSIS
        // static uint16_t Referee_Sand_Cnt = 0;
        // //暂无云台tim4任务
        // if(Referee_Sand_Cnt%10)
        //     Task_Loop();

    #elif defined(GIMBAL)
        // 单给IMU消息开的定时器 ims
        chariot.Gimbal.Boardc_BMI.TIM_Calculate_PeriodElapsedCallback();     
    #endif
    //imu_test.TIM_Calculate_PeriodElapsedCallback();
}


void Task1ms_TIM3_Callback()
{
    outpost.TIM_Alive_PeriodElapsedCallback();
    outpost.PID_Omega.Set_Now(outpost.Get_Now_Omega_Radian());
    double value=0.8*PI;
    outpost.PID_Omega.Set_Target(value);
    outpost.PID_Omega.TIM_Adjust_PeriodElapsedCallback();
    outpost.Set_Out(outpost.PID_Omega.Get_Out());
    outpost.outpost_out();
    /****************************** 驱动层回调函数 1ms *****************************************/ 
        //统一打包发送
    TIM_CAN_PeriodElapsedCallback();

}
void callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x205):   //保留can2对M2006编码器的接口
    {
        outpost.CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
	break;
	}
}
extern "C" void Task_Init()
{  

    DWT_Init(168);

    /********************************** 驱动层初始化 **********************************/
    CAN_Init(&hcan1,callback);
    //定时器循环任务
    TIM_Init(&htim2, Task1ms_TIM2_Callback);
    TIM_Init(&htim3, Task1ms_TIM3_Callback);

    /********************************* 设备层初始化 *********************************/

     //设备层集成在交互层初始化中，没有显视地初始化

    /********************************* 交互层初始化 *********************************/

    // imu_test.Init();
    // yaw_test.Init(&hcan1,DJI_Motor_ID_0x202);
    // yaw_test.PID_Angle.Init(2,0,0,0,5,180,0);
    // yaw_test.PID_Omega.Init(2,0,0,0,5,8000,0);
    outpost.Init(&hcan1,DJI_Motor_ID_0x205);
    //ID未确定
    outpost.PID_Omega.Init(2000,2000,0.1,0,5000,16000,0);
    /********************************* 使能调度时钟 *********************************/

    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);

}