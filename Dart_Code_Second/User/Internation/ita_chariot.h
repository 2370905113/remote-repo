#ifndef _ITA_CHARIOT_H_
#define _ITA_CHARIOT_H_
#ifdef __cplusplus
#include "crt_booster.h"
#include "crt_gimbal.h"
#include "dvc_dr16.h"
#include "drv_uart.h"
#include "drv_math.h"
#include "alg_fsm.h"
#include "dvc_minipc.h"
class Class_Chariot;
/**
 * @brief 机器人是否离线 控制模式有限自动机
 *
 */
class Class_FSM_Alive_Control : public Class_FSM
{
public:
    Class_Chariot *Chariot;

    void Reload_TIM_Statu_PeriodElapsedCallback();
};

class Class_Chariot
{
public:
    Class_Booster booster;
    Class_Gimbal gimbal;
    Class_DR16 dr16;
    //上位机
    Class_MiniPC MiniPC;
    Class_Referee Referee;
//遥控器离线保护控制状态机
        Class_FSM_Alive_Control FSM_Alive_Control;
        friend class Class_FSM_Alive_Control;
    void Init();
    void TIM_Control_Callback();
    void TIM_Chariot_PeriodElapsedCallback();
    void TIM1msMod50_Alive_PeriodElapsedCallback();
protected:
    //内部函数
    void Control_Booster();
    void Control_Gimbal();
    float MiniPC_Yaw_K=-0.003f;
};
#define DR16_Yaw_Angle_Resolution  0.005f * PI * 57.29577951308232
#endif
#endif
