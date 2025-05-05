#include "ita_chariot.h"

void Class_Chariot::Init()
{
    booster.Init();
    gimbal.Init();
    // 遥控器离线控制 状态机
    FSM_Alive_Control.Chariot = this;
    FSM_Alive_Control.Init(5, 0);
    dr16.Init(&huart3, 0);
    MiniPC.Init(&hcan2);
    Referee.Init(&huart6);
}
void Class_Chariot::Control_Booster()
{
    static uint8_t Shoot_Flag = 0;
    static uint8_t Shoot_Cnt = 0;
    #ifdef DEBUG
    // 遥控器拨杆右上->开启摩擦轮等待射击
    switch (dr16.Get_DR16_Status())
    {
    case DR16_Status_ENABLE:
        if (dr16.Get_Left_Switch() == DR16_Switch_Status_UP)
        {
            booster.Set_Booster_Status(Booster_Enable);
            booster.Set_Fric_Open_Status(Fric_Open);
            booster.Push_Backward_To_Zero_Pos = DISABLE;
            booster.Set_Booster_Control_Type(Booster_Control_Ceasefire);
            if (dr16.Get_Yaw() < 0.2 && dr16.Get_Yaw() > -0.2)
                Shoot_Flag = 0;
            else if (dr16.Get_Yaw() > 0.8 && Shoot_Flag == 0)
            {
                Shoot_Flag = 1;
                booster.Set_Booster_Control_Type(Booster_Control_Single);
            }
        }
        else if (dr16.Get_Left_Switch() == DR16_Switch_Status_DOWN)
        {
            booster.Set_Booster_Status(Booster_Enable);
            booster.Set_Fric_Open_Status(Fric_Close);
            booster.Push_Backward_To_Zero_Pos = ENABLE;
        }
        else
        {
            booster.Set_Booster_Status(Booster_Disable);
        }
        break;
    case DR16_Status_DISABLE:
        booster.Set_Booster_Status(Booster_Disable);
        break;
    default:
        break;
    }
    #endif
    

    #ifdef NORMAL
    booster.Set_Booster_Status(Booster_Enable);
    booster.Set_Fric_Open_Status(Fric_Open);
    booster.Push_Backward_To_Zero_Pos = DISABLE;
    booster.Set_Booster_Control_Type(Booster_Control_Ceasefire);
    #endif // NORMAL
}
float K = 0.04f;
void Class_Chariot::Control_Gimbal()
{
    float dr_r_x;
    float tmp_gimbal_yaw;
    float auto_gimbal_yaw;
    tmp_gimbal_yaw = gimbal.Get_Target_Yaw_Angle();
#ifdef DEBUG
    switch (dr16.Get_DR16_Status())
    {
    case DR16_Status_ENABLE:
        gimbal.Set_Gimbal_Status(Gimbal_Enable);
        // dr_r_x = (Math_Abs(dr16.Get_Right_X() > 0)) ? dr16.Get_Right_X() : 0;
        dr_r_x = dr16.Get_Right_X();
        tmp_gimbal_yaw -= dr_r_x * DR16_Yaw_Angle_Resolution * K;
        Math_Constrain(&tmp_gimbal_yaw, 0.0f, 18.0f);
        if (booster.Actual_Launch_Cnt < 4)
        {
            auto_gimbal_yaw = gimbal.auto_yaw[booster.Actual_Launch_Cnt];
        }
        else if (booster.Actual_Launch_Cnt >= 4)
        {
            auto_gimbal_yaw = gimbal.auto_yaw[0];
        }
        if (MiniPC.Get_MiniPC_Status() == MiniPC_Status_ENABLE)
        {
            auto_gimbal_yaw =
                gimbal.Yaw.Yaw_Now_Angle + MiniPC.yaw_error * MiniPC_Yaw_K + gimbal.yaw_delta[booster.Actual_Launch_Cnt];
        }
        gimbal.Set_Target_Yaw_Angle(auto_gimbal_yaw);
        break;
    case DR16_Status_DISABLE:
        gimbal.Set_Gimbal_Status(Gimbal_Disable);
        break;
    default:
        break;
    }
#endif // DEBUG

#ifndef NORMAL
    gimbal.Set_Gimbal_Status(Gimbal_Enable);
    if (booster.Actual_Launch_Cnt < 4)
    {
        auto_gimbal_yaw = gimbal.auto_yaw[booster.Actual_Launch_Cnt];
    }
    else if (booster.Actual_Launch_Cnt >= 4)
    {
        auto_gimbal_yaw = gimbal.auto_yaw[0];
    }
    if (MiniPC.Get_MiniPC_Status() == MiniPC_Status_ENABLE)
    {
        auto_gimbal_yaw =
            gimbal.Yaw.Yaw_Now_Angle + MiniPC.yaw_error * MiniPC_Yaw_K + gimbal.yaw_delta[booster.Actual_Launch_Cnt];
    }
    gimbal.Set_Target_Yaw_Angle(auto_gimbal_yaw);
#endif // NORMAL
}
void Class_Chariot::TIM_Control_Callback()
{
    Control_Booster();
    Control_Gimbal();
}
void Class_Chariot::TIM_Chariot_PeriodElapsedCallback()
{

    booster.TIM_Booster_PeriodElapsedCallback();
    gimbal.TIM_Gimbal_PeriodElapsedCallback();
    MiniPC.TIM_Write_PeriodElapsedCallback();
}

void Class_FSM_Alive_Control::Reload_TIM_Statu_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Time++;

    switch (Now_Status_Serial)
    {
    // 离线检测状态
    case (0):
    {
        // 遥控器中途断联导致错误离线 跳转到 遥控器串口错误状态
        if (huart3.ErrorCode)
        {
            Status[Now_Status_Serial].Time = 0;
            Set_Status(4);
        }

        // 转移为 在线状态
        if (Chariot->dr16.Get_DR16_Status() == DR16_Status_ENABLE)
        {
            Status[Now_Status_Serial].Time = 0;
            Set_Status(2);
        }

        // 超过一秒的遥控器离线 跳转到 遥控器关闭状态
        if (Status[Now_Status_Serial].Time > 1000)
        {
            Status[Now_Status_Serial].Time = 0;
            Set_Status(1);
        }
    }
    break;
    // 遥控器关闭状态
    case (1):
    {
        #ifdef DEBUG
        // 离线保护
        Chariot->booster.Set_Booster_Status(Booster_Disable);
        Chariot->gimbal.Set_Gimbal_Status(Gimbal_Disable);
        #endif

        #ifdef NORMAL
        Chariot->booster.Set_Booster_Status(Booster_Enable);
        Chariot->gimbal.Set_Gimbal_Status(Gimbal_Enable);
        #endif
        if (Chariot->dr16.Get_DR16_Status() == DR16_Status_ENABLE)
        {
            Chariot->booster.Set_Booster_Status(Booster_Enable);
            Chariot->gimbal.Set_Gimbal_Status(Gimbal_Enable);
            // Chariot->Chassis.Set_Chassis_Control_Type(Chariot->Get_Pre_Chassis_Control_Type());
            // Chariot->Gimbal.Set_Gimbal_Control_Type(Chariot->Get_Pre_Gimbal_Control_Type());
            Status[Now_Status_Serial].Time = 0;
            Set_Status(2);
        }

        // 遥控器中途断联导致错误离线 跳转到 遥控器串口错误状态
        if (huart3.ErrorCode)
        {
            Status[Now_Status_Serial].Time = 0;
            Set_Status(4);
        }
        
    }
    break;
    // 遥控器在线状态
    case (2):
    {
        // 转移为 刚离线状态
        if (Chariot->dr16.Get_DR16_Status() == DR16_Status_DISABLE)
        {
            Status[Now_Status_Serial].Time = 0;
            Set_Status(3);
        }
    }
    break;
    // 刚离线状态
    case (3):
    {
        // 记录离线检测前控制模式
        //  Chariot->Set_Pre_Chassis_Control_Type(Chariot->Chassis.Get_Chassis_Control_Type());
        //  Chariot->Set_Pre_Gimbal_Control_Type(Chariot->Gimbal.Get_Gimbal_Control_Type());

        // 无条件转移到 离线检测状态
        Status[Now_Status_Serial].Time = 0;
        Set_Status(0);
    }
    break;
    // 遥控器串口错误状态
    case (4):
    {
        HAL_UART_DMAStop(&huart3); // 停止以重启
        // HAL_Delay(10); // 等待错误结束
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);

        // 处理完直接跳转到 离线检测状态
        Status[Now_Status_Serial].Time = 0;
        Set_Status(0);
    }
    break;
    }
}
