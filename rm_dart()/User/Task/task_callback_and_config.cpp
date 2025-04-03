#include "task_callback_and_config.h"
#include "ita_chariot.h"
#include "drv_tim.h"
#include "drv_can.h"
Class_Chariot chariot;
uint32_t init_finished = 0;
uint8_t start_flag = 0;
void Chariot_Device_CAN1_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x201):
    {
        chariot.booster.Fric_Motor[0].CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    case (0x202):
    {
        chariot.booster.Fric_Motor[1].CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    case (0x203):
    {
        chariot.booster.Fric_Motor[2].CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    case (0x204):
    {
        chariot.booster.Fric_Motor[3].CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    case (0x205):
    {
        chariot.gimbal.Yaw.CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    }
}
void Chariot_Device_CAN2_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x201):
    {
        chariot.booster.Push_Motor.CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    
    }
}
void Class_Chariot::TIM1msMod50_Alive_PeriodElapsedCallback()
{
    static uint8_t mod50 = 0;
    static uint8_t mod50_mod3 = 0;
    mod50++;
    if (mod50 == 50)
    {
        mod50_mod3++;
        if(mod50_mod3%3==0)
        {
            dr16.TIM1msMod50_Alive_PeriodElapsedCallback();
            mod50_mod3 = 0;
        }

        for (auto i = 0; i < 4; i++)
        {
            booster.Fric_Motor[i].TIM_Alive_PeriodElapsedCallback();
        }
        booster.Push_Motor.TIM_Alive_PeriodElapsedCallback();
        gimbal.Yaw.TIM_Alive_PeriodElapsedCallback();
        mod50 = 0;
    }    
}
void Task1ms_TIM5_Callback()
{
    init_finished++;
    if(init_finished>2000)
        start_flag=1;
        
    chariot.TIM1msMod50_Alive_PeriodElapsedCallback();
    if(start_flag==1)
    {
        
		chariot.FSM_Alive_Control.Reload_TIM_Statu_PeriodElapsedCallback();
        chariot.TIM_Chariot_PeriodElapsedCallback();
        //CAN发送
        TIM_CAN_PeriodElapsedCallback();
    }
}
void DR16_UART1_Callback(uint8_t *Buffer, uint16_t Length)
{
    chariot.dr16.DR16_UART_RxCpltCallback(Buffer);
    chariot.TIM_Control_Callback();
}
extern "C" void Task_Init()
{
    //can总线
    CAN_Init(&hcan1,Chariot_Device_CAN1_Callback);
    CAN_Init(&hcan2,Chariot_Device_CAN2_Callback);
    //遥控器接收
    UART_Init(&huart3, DR16_UART1_Callback, 18);
    //定时器循环任务
    TIM_Init(&htim5,Task1ms_TIM5_Callback);
    //交互层
    chariot.Init();
	
		HAL_TIM_Base_Start_IT(&htim5);
	
}