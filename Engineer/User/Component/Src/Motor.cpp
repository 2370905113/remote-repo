#include "Motor.h"

void Class_MG513X::Init(TIM_HandleTypeDef *htim)
{
    /* 编码器模式初始化 */
    __HAL_TIM_SET_COUNTER(TIM_Handle,1000);
    HAL_TIM_Encoder_Start(htim,TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(htim,TIM_CHANNEL_2);

    TIM_Handle = htim;

}
void Class_MG513X::Encode_Counter()
{
    Delta_Count = __HAL_TIM_GET_COUNTER(TIM_Handle) - 1000;   
    __HAL_TIM_SET_COUNTER(TIM_Handle,1000);
}

void Class_MG513X::Velocity_Caculate()
{
    tim_flag++;
    if(tim_flag >= 10)
    {
        tim_flag = 0;

        Encode_Counter();

        Encode_Count += Delta_Count;
    }

    /* 编码器模式4倍频 */ 
    Omega = 2*PI*  Delta_Count/(PPR*Reduction_Ratio*Dt*4);
    
    
    Velocity = Omega*(Diameter/2)*100;// 单位cm/s

}

float Class_MG513X::Get_Velocity()
{
    return Velocity;
}