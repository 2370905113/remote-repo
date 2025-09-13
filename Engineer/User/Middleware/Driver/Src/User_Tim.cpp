/* 头文件引用 */
#include "User_Tim.h"


uint32_t flag=0;



Struct_TIM_Object TIM1_Object;
Struct_TIM_Object TIM2_Object;
Struct_TIM_Object TIM3_Object;
Struct_TIM_Object TIM4_Object;

void TIM_Init(TIM_HandleTypeDef *htim, TIM_Callback_Function Callback_Function)
{
  if(htim->Instance==TIM1)
  {
    TIM1_Object.TIM_Handler = htim;
    TIM1_Object.Callback_Function = Callback_Function;
  }
  if(htim->Instance==TIM4)
  {
    TIM4_Object.TIM_Handler = htim;
    TIM4_Object.Callback_Function = Callback_Function;
  }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
  /* Prevent unused argument(s) compilation warning */
  if(htim->Instance == TIM1)
  {
    flag++;
    
    TIM1_Object.Callback_Function();
  }
  if(htim->Instance == TIM4)
  {
    flag++;
    
    TIM4_Object.Callback_Function();
  }
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

