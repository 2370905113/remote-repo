#include "Alg_PID.h"
#include "stm32f1xx_hal.h"
#include "User_Tim.h"

class Class_MG513X
{
    public:

        Class_PID Pid;

        void Init(TIM_HandleTypeDef *htim);

        void Encode_Counter();

        void Velocity_Caculate();

        float Get_Velocity();

    private:

        TIM_HandleTypeDef *TIM_Handle;

        float Diameter = 0.065; //单位m,轮径65mm,轴径6mm

        uint8_t Reduction_Ratio = 28; //减速比 1 ：28

        uint8_t PPR = 13;

        int32_t Encode_Count = 0;

    
        float Velocity = 0.0f;

        float Omega = 0.0f ; //rad
    
        float Dt = 0.01;//1ms检测时间过短

        int16_t Delta_Count= 0; 
        
        uint8_t tim_flag=0;
};