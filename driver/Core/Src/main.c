/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "main.h"
#include <stdbool.h>

// 卡尔曼滤波参数
#define Q 0.01  // 过程噪声协方差
#define R 1.0  // 观测噪声协方差
#define A 1.0  // 状态转移矩阵
#define H 1.0  // 观测矩阵

// 卡尔曼滤波器结构体
typedef struct 
{
    float x;  // 当前估计值
    float P;  // 误差协方差
    float K;  // 卡尔曼增益
} KalmanFilter;

// 初始化卡尔曼滤波器
void kalman_init(KalmanFilter *kf, float initial_value) {
    kf->x = initial_value;  // 初始估计值
    kf->P = 1000.0;  // 初始误差协方差，较大值表示初始的不确定性
}

// 卡尔曼滤波更新函数
void kalman_update(KalmanFilter *kf, float measurement) {
    // 预测步骤
    float x_pred = A * kf->x;  // 预测的值
    float P_pred = A * kf->P * A + Q;  // 预测的协方差

    // 更新步骤
    kf->K = P_pred * H / (H * P_pred * H + R);  // 计算卡尔曼增益
    kf->x = x_pred + kf->K * (measurement - H * x_pred);  // 更新估计值
    kf->P = (1 - kf->K * H) * P_pred;  // 更新协方差
}
#define SCK_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define SCK_LOW()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define READ_DT()  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)

// HX711 读取 24 位数据函数
uint32_t HX711_Read(void)
{
    uint32_t data = 0;
    while (READ_DT()); // 等待DT变为低电平（数据准备好）

    for (int i = 0; i < 24; i++) {
        SCK_HIGH();                // 产生时钟上升沿
        data = (data << 1) | READ_DT(); // 读取DT引脚电平
        SCK_LOW();                 // 时钟下降沿
    }

    // 提供额外的时钟脉冲，选择增益
    SCK_HIGH();
    SCK_LOW();

    return data ^ 0x800000; // 如果最高位为负数标志，转化为有符号值
}

#define  DEM_CR      *(volatile uint32_t *)0xE000EDFC
#define  DWT_CR      *(volatile uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(volatile uint32_t *)0xE0001004
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)
 
 
void DelayInit()
{
    DEM_CR  |=  DEM_CR_TRCENA; 
    DWT_CR  |=  DWT_CR_CYCCNTENA;
    DWT_CYCCNT = 0;
}

void Delayus(uint32_t usec)
{
     uint32_t start,end,ts;
     start = DWT_CYCCNT;
     ts =  usec * (8);      
     end = start + ts;      
     while(DWT_CYCCNT < end){;} 
}


void Delayms(uint32_t usec)
{
	Delayus(usec*1000);
}


/* 用于保存HX711的增益 */
typedef enum {
    GAIN_128 = 1,
    GAIN_32,
    GAIN_64,
} ENUM_HX711_GAIN_TYPEDEF;
 
/* 结构体 用于实现HX711本身的属性 */
typedef struct {
    ENUM_HX711_GAIN_TYPEDEF gain; /* 增益 */
    bool isTare;    /* 判断传感器数据是否是皮重 true-是 false-否 */
    float k;        /* 比例系数 */
    float b;        /* 补偿值 */
    float tare;     /* 皮重 */
    float actual;   /* 实重 */
	float tare_get_data[30];
} STRUCT_HX711_TYPEDEF;
 
/* HX711传感器引脚定义 */
#define     HX711_DOUT_GPIO_CLK    RCC_APB2Periph_GPIOC
#define     HX711_DOUT_GPIO_PORT   GPIOC
#define     HX711_DOUT_GPIO_PIN    GPIO_Pin_0
 
#define     HX711_SCK_GPIO_CLK     RCC_APB2Periph_GPIOC
#define     HX711_SCK_GPIO_PORT    GPIOC
#define     HX711_SCK_GPIO_PIN     GPIO_Pin_1
 
/* ---------------- 函数清单 --------------- */
void        hx711_init(void);       				/* HX711初始化 */
void        hx711_power_off(void);   				/* HX711断电 */
void        hx711_reset(void);      				/* HX711断电后复位 */
void        hx711_set_convert_ratio(float ratio); 	/* 设置HX711重量比例系数 */
void        hx711_set_offset_value(float offset); 	/* 设置HX711重量补偿值 */
void        hx711_set_gain(ENUM_HX711_GAIN_TYPEDEF gain); /* 设置HX711增益系数 */
float       hx711_get_tare_weight(void); 			/* 获取皮重 */
float       hx711_get_actual_weight(void); 			/* 获取实重 */

#define     HX711_DOUT(x)          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (GPIO_PinState)x)
#define     HX711_SCK(x)           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, (GPIO_PinState)x)
#define 		HX711_SCK_STATE  		 	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)
#define 		HX711_DOUT_STATE  	 	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)
 
uint32_t raw_data;
uint16_t updata_cnt;
uint16_t updata_cnt_pre;

static STRUCT_HX711_TYPEDEF hx711;
KalmanFilter kf_instance;
 
/* 传感器 gpio引脚配置 */
static void hx711_gpio_config(void)
{
    HX711_SCK(0); HX711_DOUT(1);
}
 
/* 传感器结构体参数初始化 */
static void hx711_struct_config(void)
{
	hx711.gain = GAIN_128;	/* 当前增益 128 */
    hx711.isTare = true;	/* 默认第一次称重为皮重 */
    hx711.k = 0.004757f;	/* 传感器转换系数 */
    hx711.b = 0.0f;			/* 传感器误差校正/补偿值 */
    hx711.tare = 0.f;		/* 保存皮重数据 */
    hx711.actual = 0.f;		/* 保存净重数据 */
}
//     unsigned long count = 0;		/* 需要读取25-27个0/1数据 */
//    unsigned char timeOut_cnt = 0; 	/* 检测超时计数值 */
///* 读取传感器返回的ADC数据值（数字量） 假定为一阶线性关系 y=kx+b */
//static unsigned long hx711_readCount(void)
//{

//    
//    HX711_SCK(0);
//	/* 添加超时检测 防止一直未识别到传感器导致程序卡死在这里 */
//    while(HX711_DOUT_STATE) { /* 等待DOUT从高电平到低电平跳变 */
//        timeOut_cnt ++;
//        Delayms(1);
//        if(timeOut_cnt > 100) return 0;
//    }
//    /* 读取24位的ADC数字量数据 */
//    for(unsigned char i=0; i<24; i++) {
//        HX711_SCK(1);
//        Delayus(1);
//        HX711_SCK(0);
//        count |= (HX711_DOUT_STATE) << (24-i); /* 上升沿读取数据 */
//        Delayus(1);
//    }
//    /* 用于设置输出增益和输出通道 */
//    for(unsigned char j=0; j<hx711.gain; j++) {
//        HX711_SCK(1);
//        Delayus(1);
//        HX711_SCK(0);
//        Delayus(1);
//    }
//    return (count ^ 0x800000);
//}
 
/* ----------------------------- 接口函数 ---------------------- */
/* 传感器 初始化 */
void hx711_init(void)
{
    hx711_gpio_config();
    hx711_struct_config();
}
 
/* 传感器断电 */
void hx711_power_off(void)
{
    HX711_SCK(0);
    Delayus(1);
    HX711_SCK(1);
    Delayus(100);
}
 
/* 传感器上电 */
void hx711_reset(void)
{
    if(HX711_SCK_STATE == 1) {
        HX711_SCK(0);
    }
}
 
/* 设置传感器转换系数 */
void hx711_set_convert_ratio(float ratio)
{
    hx711.k = ratio;
}
 
/* 设置传感器误差校正/补偿值 */
void hx711_set_offset_value(float offset)
{
    hx711.b = offset;
}
 
/* 设置传感器增益 */
void hx711_set_gain(ENUM_HX711_GAIN_TYPEDEF gain)
{
    hx711.gain = gain;
}
 
/* 获取传感器皮重数据 */\
float hx711_get_tare_weight(void)
{
		float temp;
    hx711.isTare = true; /* 获取皮重的时候 将该标志位置位 */
    if(hx711.isTare == true) {
        hx711.isTare = false;
			for(int i=0;i<30;i++)
			{
				
				hx711.tare_get_data[i]=HX711_Read();
				temp+=hx711.tare_get_data[i]-hx711.tare_get_data[0];
				temp=temp/30;
			}
			
        hx711.tare = (float)(hx711.tare_get_data[0]+temp) * hx711.k + hx711.b; /* 此时获取到的重量为皮重 */
    }
    return (hx711.tare);
}

/* 获取传感器实际重量 */
float hx711_get_actual_weight(void)
{
     float weight = 0.f;
    if(hx711.isTare == false) { /* 当该标志位复位 代表此时应计算净重 */
        weight = (float)HX711_Read() * hx711.k + hx711.b;
        hx711.actual = (weight - hx711.tare)/23.91304347f;
    }
    return (hx711.actual);
}
 

uint8_t tare_cal = 0;
float fps;
void cal_speed()
{
	 static uint32_t start,end,derta;
	 static uint8_t cnt_flag  = 0;
	if(cnt_flag==0)
	{
		start = DWT_CYCCNT;
		derta =  100000 * (8);  //100000us*8Mhz   0.1s
		end = start + derta;
		updata_cnt_pre = updata_cnt;
		cnt_flag = 1;
	}
	if(DWT_CYCCNT>=end)
	{
		fps = (updata_cnt-updata_cnt_pre)/0.1;
		cnt_flag = 0;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	DelayInit();
	hx711_init();
  kalman_init(&kf_instance,0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//		raw_data = HX711_Read();
		updata_cnt++;
		cal_speed();
		
		
		if(!tare_cal)
		{
			hx711_get_tare_weight();
			tare_cal = 1;
		}
		float measurement;
		measurement = hx711_get_actual_weight();
    kalman_update(&kf_instance,measurement);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
