#ifndef __SPIF_H
#define __SPIF_H

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"

// --- USER CONFIGURATION ---
// Define the GPIO port and pins used for the flash memory.
#define SPI_FLASH_PORT        GPIOA
#define SPI_FLASH_SCK_PIN     GPIO_Pin_5
#define SPI_FLASH_MISO_PIN    GPIO_Pin_6
#define SPI_FLASH_MOSI_PIN    GPIO_Pin_7
#define SPI_FLASH_CS_PIN      GPIO_Pin_4

// Pin Sources for Alternate Function Configuration
#define SPI_FLASH_SCK_PINSOURCE  GPIO_PinSource5
#define SPI_FLASH_MISO_PINSOURCE GPIO_PinSource6
#define SPI_FLASH_MOSI_PINSOURCE GPIO_PinSource7


// --- FLASH MEMORY COMMANDS (COMMON FOR W25Q SERIES) ---
#define CMD_WRITE_ENABLE      0x06
#define CMD_WRITE_DISABLE     0x04
#define CMD_READ_STATUS_REG   0x05
#define CMD_PAGE_PROGRAM      0x02
#define CMD_SECTOR_ERASE      0x20
#define CMD_READ_DATA         0x03
#define CMD_JEDEC_ID          0x9F

// --- CONSTANTS ---
#define SPI_FLASH_PAGE_SIZE   256
#define DUMMY_BYTE            0x00



//function
uint8_t spi_flash_send_byte(uint8_t);
uint8_t spi_flash_read_byte(void);
void spi_flash_select(void);
void spi_flash_deselect(void);
void spi_flash_wait_busy(void);
void spi_flash_write_enable(void);
void spi_flash_init(void);
void spi_flash_erase_sector(uint32_t);
void spi_flash_write_data(uint32_t , uint8_t* , uint32_t );
void spi_flash_read_data(uint32_t , uint8_t*, uint32_t );




#endif
