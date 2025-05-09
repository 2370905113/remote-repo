#include "flash.h"
#include "stm32f4xx.h"
#include <string.h>

#define FLASH_SECTOR_ADDR   0x080E0000  // 使用 Sector 11（最后一个128KB）
#define FLASH_SECTOR        11
#define FLASH_SECTOR_SIZE   (128 * 1024)

#define DATA_BLOCK_SIZE     24  // 12 * uint16_t
#define DATA_BLOCK_COUNT    (FLASH_SECTOR_SIZE / DATA_BLOCK_SIZE)

void FLASH_Unlock(void) {
    if (FLASH->CR & FLASH_CR_LOCK) {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

void FLASH_Lock(void) {
    FLASH->CR |= FLASH_CR_LOCK;
}

void FLASH_ClearFlags(void) {
    // Before
    // FLASH->SR |= FLASH_SR_EOP | FLASH_SR_OPERR | FLASH_SR_WRPERR |
    //              FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR;
    
    // After: Use correct flag names from HAL
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     | 
                          FLASH_FLAG_OPERR   |
                          FLASH_FLAG_WRPERR  |
                          FLASH_FLAG_PGAERR  |
                          FLASH_FLAG_PGPERR  |
                          FLASH_FLAG_PGSERR);
}

// 擦除一个扇区
uint8_t FLASH_EraseSector(uint8_t sector) {
    uint32_t timeout = 0xFFFFF;
    FLASH_Unlock();
    FLASH_ClearFlags();
    FLASH->CR &= ~FLASH_CR_SNB;
    FLASH->CR |= FLASH_CR_SER | (sector << 3);
    FLASH->CR |= FLASH_CR_STRT;

    while ((FLASH->SR & FLASH_SR_BSY) && --timeout);
    if (timeout == 0) return 1;

    FLASH->CR &= ~FLASH_CR_SER;
    FLASH_Lock();
    return 0;
}

// 写入一个半字
uint8_t FLASH_WriteHalfWord(uint32_t addr, uint16_t data) {
    uint32_t timeout = 0xFFFFF;
    FLASH_Unlock();
    FLASH_ClearFlags();
    FLASH->CR |= FLASH_CR_PG;
    FLASH->CR |= FLASH_CR_PSIZE_0; // PSIZE = 16-bit

    *(volatile uint16_t*)addr = data;
    while ((FLASH->SR & FLASH_SR_BSY) && --timeout);
    if (timeout == 0) return 1;

    FLASH->CR &= ~FLASH_CR_PG;
    FLASH_Lock();
    return 0;
}

// 查找下一个可写地址
uint32_t FindNextWriteAddress(void) {
    for (uint32_t i = 0; i < DATA_BLOCK_COUNT; i++) {
        uint32_t addr = FLASH_SECTOR_ADDR + i * DATA_BLOCK_SIZE;
        if (*(volatile uint16_t*)addr == 0xFFFF) {
            return addr;
        }
    }
    return FLASH_SECTOR_ADDR; // 表示已满
}

// 写入 12 个 uint16_t 数据
uint32_t WriteDataBlock(uint16_t data[12]) {
    uint32_t addr = FindNextWriteAddress();

    if (addr == FLASH_SECTOR_ADDR && *(uint16_t*)addr != 0xFFFF) {
        if (FLASH_EraseSector(FLASH_SECTOR) != 0) return 1;
        addr = FLASH_SECTOR_ADDR;
    }

    for (int i = 0; i < 12; i++) {
        if (FLASH_WriteHalfWord(addr + i * 2, data[i]) != 0) {
            return 1; // 写失败
        }
    }
    return 0;
}

// 读取最后一次写入的数据
void ReadLatestData(uint16_t data[12]) {
    uint32_t addr = FLASH_SECTOR_ADDR;
    for (uint32_t i = 0; i < DATA_BLOCK_COUNT; i++) {
        uint16_t marker = *(volatile uint16_t*)(addr + i * DATA_BLOCK_SIZE);
        if (marker == 0xFFFF) {
            if (i == 0) {
                memset(data, 0xFF, 24);
            } else {
                memcpy(data, (void*)(addr + (i - 1) * DATA_BLOCK_SIZE), 24);
            }
            return;
        }
    }
    // 全满，读取最后一块
    memcpy(data, (void*)(addr + (DATA_BLOCK_COUNT - 1) * DATA_BLOCK_SIZE), 24);
}
