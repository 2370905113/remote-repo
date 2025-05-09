#ifndef __FLASH_SAFE_H__
#define __FLASH_SAFE_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum {
    FLASH_OK = 0,
    FLASH_TIMEOUT,
    FLASH_INVALID_ADDRESS,
    FLASH_WRITE_ERROR
} FlashStatus;

void FLASH_Unlock(void);
void FLASH_Lock(void);

FlashStatus FLASH_ErasePage(uint32_t PageAddress);
FlashStatus FLASH_WriteHalfWord(uint32_t Address, uint16_t Data);
FlashStatus FLASH_ReadHalfWord(uint32_t Address, uint16_t* Data);

uint32_t FindNextWriteAddress();
uint32_t FindNextReadAddress();
#ifdef __cplusplus
extern "C" {
#endif

uint32_t WriteDataBlock(uint16_t data[12]);
void ReadLatestData(uint16_t data[12]);

#ifdef __cplusplus
}
#endif
#endif
