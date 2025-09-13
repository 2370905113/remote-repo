#ifndef _USER_TIM_H
#define _USER_TIM_H

// c兼容c++
#ifdef __cplusplus
extern "C"
{
#endif

    #include "stm32f1xx_hal.h"
    /* 自定义定时器句柄及回调函数结构体 */
    typedef void (*TIM_Callback_Function)();


 typedef struct
 {
     TIM_HandleTypeDef *TIM_Handler;
     TIM_Callback_Function Callback_Function;
 }Struct_TIM_Object;


 extern TIM_HandleTypeDef htim1;
 extern TIM_HandleTypeDef htim2;
 extern TIM_HandleTypeDef htim3;
 extern TIM_HandleTypeDef htim4;

 extern Struct_TIM_Object TIM1_Object;


 void TIM_Init(TIM_HandleTypeDef *htim, TIM_Callback_Function Callback_Function);
    
#ifdef __cplusplus
}
#endif

#endif /* _USER_TIM_H */


