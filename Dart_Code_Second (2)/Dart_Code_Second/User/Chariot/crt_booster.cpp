#include "crt_booster.h"

void Class_Fric_Motor::TIM_PID_PeriodElapsedCallback()
{
    switch (DJI_Motor_Control_Method)
    {
    case (DJI_Motor_Control_Method_OPENLOOP):
    {
        //默认开环扭矩控制
        Out = Target_Torque / Torque_Max * Output_Max;
    }
    break;
    break;
    case (DJI_Motor_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega_Rpm);
        PID_Omega.Set_Now(Data.Now_Omega_Rpm);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Out = PID_Omega.Get_Out();
    }
    break;
    default:
    {
        Out = 0.0f;
    }
    break;
    }
    Output();
}
void Class_Push_Motor::TIM_PID_PeriodElapsedCallback()
{
    switch (DJI_Motor_Control_Method)
    {
    case (DJI_Motor_Control_Method_OPENLOOP):
    {
        //默认开环扭矩控制
        Out = Target_Torque / Torque_Max * Output_Max;
    }
    break;
    break;
    case (DJI_Motor_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega_Rpm);
        PID_Omega.Set_Now(Data.Now_Omega_Rpm);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Out = PID_Omega.Get_Out();
    }
    break;
    default:
    {
        Out = 0.0f;
    }
    break;
    }
    Output();
}
void Class_Booster::Init()
{
    // 初始化发射机构使能状态
    booster_status = Booster_Disable;
    //初始化发射状态机
    booster_fsm.Init(3,0);
    // 摩擦轮电机初始化
    Fric_Motor[0].Init(&hcan1, DJI_Motor_ID_0x201, DJI_Motor_Control_Method_OMEGA, 1.0f);
    Fric_Motor[0].PID_Omega.Init(5.0f, 0.01f, 0.0f, 0.0f, 2000.0f, 16500.0f, 0.0f, 0.0f, 0.0f, 0.001);

    Fric_Motor[1].Init(&hcan1, DJI_Motor_ID_0x202, DJI_Motor_Control_Method_OMEGA, 1.0f);
    Fric_Motor[1].PID_Omega.Init(5.0f, 0.01f, 0.0f, 0.0f, 2000.0f, 16500.0f, 0.0f, 0.0f, 0.0f, 0.001);

    Fric_Motor[2].Init(&hcan1, DJI_Motor_ID_0x203, DJI_Motor_Control_Method_OMEGA, 1.0f);
    Fric_Motor[2].PID_Omega.Init(22.0f, 0.0f, 0.0f, 0.0f, 2000.0f, 16500.0f, 0.0f, 0.0f, 0.0f, 0.001);

    Fric_Motor[3].Init(&hcan1, DJI_Motor_ID_0x204, DJI_Motor_Control_Method_OMEGA, 1.0f);
    Fric_Motor[3].PID_Omega.Init(5.0f, 0.01f, 0.0f, 0.0f, 2000.0f, 16500.0f, 0.0f, 0.0f, 0.0f, 0.001);
    // Push电机初始化
    Push_Motor.Init(&hcan2, DJI_Motor_ID_0x201,DJI_Motor_Control_Method_OMEGA);
    Push_Motor.PID_Omega.Init(15.0f, 0.00f, 0.0f, 0.0f, 2000.0f, 11000.0f, 0.0f, 0.0f, 0.0f, 0.001);
}
void Class_Push_Motor::Calculate_Push_Length()
{
    Push_Now_Length = -Get_Now_Radian() / 2.0f / PI * PUSH_ROUND_TO_LENGTH;
}
void Class_Booster::OutPut()
{
    float fric_speed=0.0f;
    switch (booster_status)
    {
    case Booster_Disable:
    {// 摩擦轮
        for (auto i = 0; i < 4; i++)
        {
            Fric_Motor[i].fric_status = Fric_Disable;
            Fric_Motor[i].Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OPENLOOP);
            Fric_Motor[i].Set_Target_Torque(0.0f);
        }
        // Push机构
        Push_Motor.push_status = Push_Disable;
        Push_Motor.Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OPENLOOP);
        Push_Motor.Set_Target_Torque(0.0f);
    }
    break;

    case Booster_Enable:
    {
        // 摩擦轮
        for (auto i = 0; i < 4; i++)
        {
            Fric_Motor[i].fric_status = Fric_Enable;
        }
        for (auto i = 0; i < 4; i++)
        {
            Fric_Motor[i].Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OMEGA);
        }
        if(fric_open_status==Fric_Open)
        {
            if(Actual_Launch_Cnt<4)
            {
                fric_speed = autofric[Actual_Launch_Cnt];
            }
            if(Actual_Launch_Cnt==4)
            {
                fric_speed = autofric[0];
            }
            Fric_Speed_High = fric_speed;
            Fric_Speed_Low = fric_speed;
            Fric_Motor[0].Set_Target_Omega_Rpm(-Fric_Speed_High);
            Fric_Motor[1].Set_Target_Omega_Rpm(-Fric_Speed_Low);
            Fric_Motor[2].Set_Target_Omega_Rpm(Fric_Speed_Low);
            Fric_Motor[3].Set_Target_Omega_Rpm(Fric_Speed_High);
        }
        else
        {
            for (auto i = 0; i < 4; i++)
            {
                Fric_Motor[i].Set_Target_Omega_Rpm(0);
            }
            
        }
        //Push机构
        // switch (booster_control_type)
        // {
        // case Booster_Control_Ceasefire:
        // { // 0:判断cnt数量：if==4 -> 清0 , 转到2; if(！=4) -> 1;
        //     // 1转到判断cnt=1,2,3,4,是否到达目标位置，if(到目标位置)->cnt=cnt;转到0
        //     // 2:恢复飞镖Push零位，if(恢复到0位)；转到
        // break;

        // case Booster_Control_Single:
        // {
        //     cnt++;
        //     Push_Motor.Push_Target_length = (float)cnt * 0.145f;
        // }
        // break;
        // }
        Push_Motor.push_status = Push_Enable;
        Push_Motor.Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OMEGA);

        switch (booster_fsm.Get_Now_Status_Serial())
        {
        case 0:
        {
            #ifdef DEBUG
            if(booster_control_type==Booster_Control_Single&&Target_Launch_Cnt<=4)
            {
                Target_Launch_Cnt++;
                booster_fsm.Set_Status(1);
            }
            #endif
            
            #ifdef NORMAL
            if(Dart_Launch_Status==Dart_30s&&Target_Launch_Cnt<2)
            {
                Target_Launch_Cnt++;
                booster_fsm.Set_Status(1);
            }
            if(Dart_Launch_Status==Dart_4min&&Target_Launch_Cnt<=4&&Target_Launch_Cnt>=2)
            {
                Target_Launch_Cnt++;
                booster_fsm.Set_Status(1); 
            }
            #endif
            if(Target_Launch_Cnt>4||(Push_Backward_To_Zero_Pos==ENABLE))
            {
                booster_fsm.Set_Status(2);
            }
        }
        break;
        case 1:
        {
            Push_Motor.Push_Target_length = Target_Launch_Cnt*0.147f;
            if(fabsf(Push_Motor.Push_Target_length - Push_Motor.Push_Now_Length) < 0.001f)
            {
                Actual_Launch_Cnt = Target_Launch_Cnt;
                Set_Booster_Control_Type(Booster_Control_Ceasefire);
                booster_fsm.Set_Status(0);
            }
        }
        break;
        case 2:
        {
            Push_Motor.Push_Target_length = Zero_Length;
            if(fabsf(Push_Motor.Push_Target_length - Push_Motor.Push_Now_Length) < 0.001f)
            {
                Target_Launch_Cnt = 0;
                Actual_Launch_Cnt = 0;
                Set_Booster_Control_Type(Booster_Control_Ceasefire);
                booster_fsm.Set_Status(0);
            }
        }
        break;
        default:
            break;
        }

        if (fabsf(Push_Motor.Push_Target_length - Push_Motor.Push_Now_Length) > 0.001f)
        {
            if (Push_Motor.Push_Target_length > Push_Motor.Push_Now_Length)
            {
                Push_Motor.push_move_type = Push_Forward;
                Push_Motor.Set_Target_Omega_Rpm(-12500);
            }
            else if (Push_Motor.Push_Target_length < Push_Motor.Push_Now_Length)
            {
                Push_Motor.push_move_type = Push_Backward;
                Push_Motor.Set_Target_Omega_Rpm(8000);
            }
        }
        else
        {
            Push_Motor.push_move_type = Push_Stop;
            Push_Motor.Set_Target_Omega_Rpm(PUSH_MOTOR_STOP);
        }
        break;
    }
    break;

    }
}
void Class_Booster::TIM_Booster_PeriodElapsedCallback()
{
    //计算push丝杆的行程
    Push_Motor.Calculate_Push_Length();

    OutPut();
    for (auto i = 0; i < 4; i++)
    {
        Fric_Motor[i].TIM_PID_PeriodElapsedCallback();
    }
    Push_Motor.TIM_PID_PeriodElapsedCallback();

}