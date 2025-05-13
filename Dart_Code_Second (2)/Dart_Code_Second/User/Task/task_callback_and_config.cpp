#include "task_callback_and_config.h"
#include "ita_chariot.h"
#include "drv_tim.h"
#include "drv_can.h"
#include "flash.h"
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
    case (0x205):
    {
        chariot.gimbal.Yaw.CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    case (0xA1):
    {
        chariot.MiniPC.CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
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
        if (mod50_mod3 % 3 == 0)
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
        MiniPC.TIM1msMod50_Alive_PeriodElapsedCallback();
        mod50 = 0;
    }
}
int last_launch_flag = 0;
int now_launch_flag = 0;
int shoot_temp = 0;

uint16_t flash_Write_data[12];
uint16_t flash_Read_data[12];
float yaw_temp[12];
bool Flash_Ready = false;
int yaw_to_tagert_cnt=0;

void Task1ms_TIM5_Callback()
{
    init_finished++;
    if (init_finished > 2000)
        start_flag = 1;
    // if (chariot.Referee.Get_Dart_Command_Status() == Referee_Data_Robot_Dart_Command_Status_CLOSED)
    // {
    //     last_launch_flag = now_launch_flag;
    //     now_launch_flag = 0;
    // }
    // if (chariot.Referee.Get_Dart_Command_Status() == Referee_Data_Robot_Dart_Command_Status_OPEN)
    // {
    //     last_launch_flag = now_launch_flag;
    //     now_launch_flag = 1;
    // }
    // if (chariot.Referee.Get_Dart_Command_Status() == Referee_Data_Robot_Dart_Command_Status_EXECUTING)
    // {
    //     last_launch_flag = now_launch_flag;
    //     now_launch_flag = 2;
    // }
    // if (last_launch_flag == 2 && now_launch_flag == 1)
    // {
    //     shoot_temp++;
    // }
    
    if(chariot.Referee.Get_Dart_Command_Status()==Referee_Data_Robot_Dart_Command_Status_OPEN
    &&chariot.Referee.Get_Dart_Change_Target_Timestamp()!=0
    &&chariot.Referee.Get_Game_Stage()==Referee_Game_Status_Stage_BATTLE
    &&chariot.init_flag==true)
    {
        if(chariot.Referee.Get_Remaining_Time()<=390&&chariot.Referee.Get_Remaining_Time()>=240)
        {
            shoot_temp=1;
        }else if(chariot.Referee.Get_Remaining_Time()<240&&chariot.Referee.Get_Remaining_Time()>1 )
        {
            shoot_temp=2;
        }
        if(chariot.Referee.Get_Game_Stage()!=Referee_Game_Status_Stage_BATTLE){
            shoot_temp = 3;
        }
    }
    else
    {
        shoot_temp=0;
    }
    chariot.booster.autofric = chariot.booster.autofric_16m;
    chariot.gimbal.yaw_delta = chariot.gimbal.yaw_delta_16m;
    // if (chariot.Referee.Get_Dart_Command_Target() == Referee_Data_Robot_Dart_Command_Target_OUTPOST)
    // {
    //     chariot.booster.autofric = chariot.booster.autofric_16m;

    //     chariot.gimbal.yaw_delta = chariot.gimbal.yaw_delta_16m;
    // }
    // else if (chariot.Referee.Get_Dart_Command_Target() == Referee_Data_Robot_Dart_Command_Target_BASE || chariot.Referee.Get_Dart_Command_Target() == Referee_Data_Robot_Dart_Command_Target_Randam_BASE)
    // {
    //     chariot.booster.autofric = chariot.booster.autofric_25m;

    //     chariot.gimbal.yaw_delta = chariot.gimbal.yaw_delta_25m;
    // }
    chariot.booster.Dart_Launch_Status = (Enum_Dart_Launch_Status)(shoot_temp);

    // flash存autoyaw`

    for (int i = 0; i < 1; i++)
    {
        flash_Write_data[i] = Math_Abs(chariot.gimbal.yaw_angle_16m) * 100;
    }

    if (Flash_Ready && chariot.booster.Push_Motor.Push_Now_Length < 0.002)
    {
        WriteDataBlock(flash_Write_data);
        Flash_Ready=false;
    }
    if(chariot.booster.Actual_Launch_Cnt==4)
    {
        Flash_Ready=true;
        init_finished++;
    }
    ReadLatestData(flash_Read_data);
    for (int i = 0; i < 12; i++)
    {
        yaw_temp[i] = -flash_Read_data[i] / 100.0f;
    }




    //计算行程
    if(Math_Abs(chariot.gimbal.Yaw.Yaw_Now_Angle-chariot.gimbal.yaw_angle_16m-chariot.gimbal.yaw_delta[chariot.booster.Actual_Launch_Cnt])<0.08)
    {
        yaw_to_tagert_cnt++;
    }
    chariot.booster.Yaw_To_Target_Bool = false;
    if(yaw_to_tagert_cnt>150)
    {
        chariot.booster.Yaw_To_Target_Bool = true;
        yaw_to_tagert_cnt=0;
    }
    
    chariot.TIM1msMod50_Alive_PeriodElapsedCallback();
    // if(huart6.ErrorCode!=0){
    //     HAL_UART_Init(&huart6);
    // }
#ifdef NORMAL
    chariot.TIM_Control_Callback();
#endif
    if (start_flag == 1)
    {

        chariot.FSM_Alive_Control.Reload_TIM_Statu_PeriodElapsedCallback();
        chariot.TIM_Chariot_PeriodElapsedCallback();
        // CAN发送
        TIM_CAN_PeriodElapsedCallback();
    }
}
void DR16_UART1_Callback(uint8_t *Buffer, uint16_t Length)
{
    chariot.dr16.DR16_UART_RxCpltCallback(Buffer);
    chariot.TIM_Control_Callback();
}

void Referee_UART6_Callback(uint8_t *Buffer, uint16_t Length)
{
    chariot.Referee.UART_RxCpltCallback(Buffer, Length);
}
/**
 * @brief USB MiniPC回调函数
 *
 * @param Buffer USB收到的消息
 *
 * @param Length 长度
 */
void MiniPC_USB_Callback(uint8_t *Buffer, uint32_t Length)
{
    chariot.MiniPC.USB_RxCpltCallback(Buffer);
}

extern "C" void Task_Init()
{
    // can总线
    CAN_Init(&hcan1, Chariot_Device_CAN1_Callback);
    CAN_Init(&hcan2, Chariot_Device_CAN2_Callback);
    // 遥控器接收
    UART_Init(&huart3, DR16_UART1_Callback, 18);
    // 裁判系统
    UART_Init(&huart6, Referee_UART6_Callback, 128); // 并未使用环形队列 尽量给长范围增加检索时间 减少丢包

    // USB初始化
    USB_Init(&MiniPC_USB_Manage_Object, MiniPC_USB_Callback);
    // 定时器循环任务
    TIM_Init(&htim5, Task1ms_TIM5_Callback);
    // 交互层
    chariot.Init();

    HAL_TIM_Base_Start_IT(&htim5);
}