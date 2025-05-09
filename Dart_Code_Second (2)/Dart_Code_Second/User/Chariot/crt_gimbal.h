#ifndef _CRT_GIMBAL_H_
#define _CRT_GIMBAL_H_
#ifdef __cplusplus
#include "dvc_djimotor.h"
#include "alg_pid.h"
enum Enum_Gimbal_Status
{
    Gimbal_Disable,
    Gimbal_Enable,
};
enum Enum_Yaw_Status
{
    Yaw_Disable,
    Yaw_Enable,
};
class Class_Yaw_Motor : public Class_DJI_Motor_GM6020
{
public:
    //变量
    float Yaw_Target_Angle;
    float Yaw_Now_Angle;
    int16_t Yaw_Target_Encoder; 
    Enum_Yaw_Status yaw_status;
    float Now_Image;
    //函数
    void Yaw_Target_Angle_To_Encoder();
    void Calculate_Yaw_Now_Angle();
    inline void Set_Yaw_Target_Angle(float _Yaw_Target_Angle);
    void TIM_PID_PeriodElapsedCallback();
    float Calibration_Angle=0.0f;
};
class Class_Gimbal
{
public:
    //变量
    Class_Yaw_Motor Yaw;
    //函数
    inline void Set_Target_Yaw_Angle(float __Target_Yaw_Angle);
    inline float Get_Target_Yaw_Angle();
    inline float Get_16m_Yaw_Angle(){
        return (yaw_angle_16m);
    };
    inline void Set_16m_Yaw_Angle(float yaw_16m){
        yaw_angle_16m=yaw_16m;
    };
    inline float Get_25m_Yaw_Angle(){
        return (yaw_angle_25m);
    };
    inline void Set_25m_Yaw_Angle(float yaw_25m){
        yaw_angle_25m=yaw_25m; 
    }
    inline void Set_Gimbal_Status(Enum_Gimbal_Status _gimbal_status);
    void Init();
    void TIM_Gimbal_PeriodElapsedCallback();
    float *auto_yaw;
    float *yaw_delta;
    float yaw_angle_16m = -2.26f;  //前哨战参考初始角度
    float yaw_angle_25m;  //基地参考初始角度
    float auto_yaw_16m[4]=
    {
        -2.5,-2.75,-2.75,2.5
    };
    float auto_yaw_25m[4]=
    {
        -3.5f,
        -3.5f,
        -3.5f,
        -3.5f
    };
    float yaw_delta_16m[4]=
    {
        -0.5,-0.5,-0.25,-0.25    
    };
    float yaw_delta_25m[4]=
    {
        0.0f,
        0.0f,
        0.0f,
        0.0f,  
    };
protected:
    //变量
    Enum_Gimbal_Status gimbal_status;
    float Target_Yaw_Angle;
    //函数
    void OutPut();

};
#define  YAW_GEAR_RATIO  (72.f/1052.f)

void Class_Yaw_Motor::Set_Yaw_Target_Angle(float _Yaw_Target_Angle)
{
    Yaw_Target_Angle = _Yaw_Target_Angle;
}
void Class_Gimbal::Set_Target_Yaw_Angle(float __Target_Yaw_Angle)
{
    Target_Yaw_Angle = __Target_Yaw_Angle;
}
float Class_Gimbal::Get_Target_Yaw_Angle()
{
    return (Target_Yaw_Angle);
}
void Class_Gimbal::Set_Gimbal_Status(Enum_Gimbal_Status _gimbal_status)
{
    gimbal_status = _gimbal_status;
}
#endif
#endif
