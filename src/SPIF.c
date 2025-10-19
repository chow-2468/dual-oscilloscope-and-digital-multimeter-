#include "SPIF.h"

uint8_t spi_flash_send_byte(uint8_t data) {
    // Wait until the transmit buffer is empty (TXE flag is set)
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // Send the byte
    SPI_I2S_SendData(SPI1, data);
    // Wait until a byte is received (RXNE flag is set)
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // Return the received byte
    return SPI_I2S_ReceiveData(SPI1);
}

/**
 * @brief Pulls the Chip Select (CS) pin low to select the flash chip.
 */
void spi_flash_select(void) {
    GPIO_ResetBits(SPI_FLASH_PORT, SPI_FLASH_CS_PIN);
}

/**
 * @brief Pulls the Chip Select (CS) pin high to deselect the flash chip.
 */
void spi_flash_deselect(void) {
    GPIO_SetBits(SPI_FLASH_PORT, SPI_FLASH_CS_PIN);
}


// --- HELPER COMMAND FUNCTIONS ---

/**
 * @brief Polls the status register of the flash chip until the busy bit is cleared.
 */
void spi_flash_wait_busy(void) {
    spi_flash_select();
    spi_flash_send_byte(CMD_READ_STATUS_REG);
    // Continue sending dummy bytes while the busy bit (bit 0) is 1
    while (spi_flash_send_byte(DUMMY_BYTE) & 0x01);
    spi_flash_deselect();
}

/**
 * @brief Sends the Write Enable (WREN) command to the flash chip.
 */
void spi_flash_write_enable(void) {
    spi_flash_select();
    spi_flash_send_byte(CMD_WRITE_ENABLE);
    spi_flash_deselect();
}


// --- CORE API FUNCTIONS ---

/**
 * @brief Initializes the SPI peripheral and GPIO pins for the flash memory.
 * @note This function is based on the STM32F4xx SPL.
 */
void spi_flash_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    // 1. Enable peripheral clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // Enable clock for GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  // Enable clock for SPI1

    // 2. Configure SPI pins (SCK, MOSI, MISO) for Alternate Function
    GPIO_InitStructure.GPIO_Pin = SPI_FLASH_SCK_PIN | SPI_FLASH_MOSI_PIN | SPI_FLASH_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SPI_FLASH_PORT, &GPIO_InitStructure);

    // Connect GPIO pins to the SPI alternate function
    GPIO_PinAFConfig(SPI_FLASH_PORT, SPI_FLASH_SCK_PINSOURCE, GPIO_AF_SPI1);
    GPIO_PinAFConfig(SPI_FLASH_PORT, SPI_FLASH_MISO_PINSOURCE, GPIO_AF_SPI1);
    GPIO_PinAFConfig(SPI_FLASH_PORT, SPI_FLASH_MOSI_PINSOURCE, GPIO_AF_SPI1);

    // 3. Configure the Chip Select (CS) pin as a standard GPIO output
    GPIO_InitStructure.GPIO_Pin = SPI_FLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(SPI_FLASH_PORT, &GPIO_InitStructure);

    // Ensure the chip is deselected initially
    spi_flash_deselect();

    // 4. Configure the SPI peripheral
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // Clock polarity
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // Clock phase
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     // We control the CS pin with software
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // Set SPI clock speed
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7; // Not used
    SPI_Init(SPI1, &SPI_InitStructure);

    // 5. Enable the SPI peripheral
    SPI_Cmd(SPI1, ENABLE);
}

/**
 * @brief Erases a 4KB sector of the flash memory.
 * @param sector_address The address of the sector to erase. Any address within
 * the target sector is valid.
 */
void spi_flash_erase_sector(uint32_t sector_address) {
    spi_flash_write_enable();
    spi_flash_select();
    spi_flash_send_byte(CMD_SECTOR_ERASE);
    // Send 24-bit address
    spi_flash_send_byte((sector_address >> 16) & 0xFF);
    spi_flash_send_byte((sector_address >> 8) & 0xFF);
    spi_flash_send_byte(sector_address & 0xFF);
    spi_flash_deselect();
    // Wait for the erase operation to complete (can take milliseconds)
    spi_flash_wait_busy();
}

/**
 * @brief Writes a block of data to the flash memory, handling page boundaries.
 * @param address The 24-bit starting address to write to.
 * @param data Pointer to the buffer containing data.
 * @param length Number of bytes to write.
 */
void spi_flash_write_data(uint32_t address, uint8_t* data, uint32_t length) {
    uint32_t remaining = length;

    while (remaining > 0) {
        // Calculate space left on the current page
        uint32_t bytes_on_page = SPI_FLASH_PAGE_SIZE - (address % SPI_FLASH_PAGE_SIZE);
        uint32_t chunk_size = (remaining > bytes_on_page) ? bytes_on_page : remaining;

        spi_flash_write_enable();
        spi_flash_select();
        spi_flash_send_byte(CMD_PAGE_PROGRAM);
        spi_flash_send_byte((address >> 16) & 0xFF);
        spi_flash_send_byte((address >> 8) & 0xFF);
        spi_flash_send_byte(address & 0xFF);

        for (uint32_t i = 0; i < chunk_size; i++) {
            spi_flash_send_byte(data[i]);
        }

        spi_flash_deselect();
        spi_flash_wait_busy();

        address += chunk_size;
        data += chunk_size;
        remaining -= chunk_size;
    }
}

/**
 * @brief Reads a block of data from the flash memory.
 * @param address The 24-bit starting address to read from.
 * @param buffer Pointer to the buffer where data will be stored.
 * @param length Number of bytes to read.
 */
void spi_flash_read_data(uint32_t address, uint8_t* buffer, uint32_t length) {
    spi_flash_select();
    spi_flash_send_byte(CMD_READ_DATA);
    // Send address
    spi_flash_send_byte((address >> 16) & 0xFF);
    spi_flash_send_byte((address >> 8) & 0xFF);
    spi_flash_send_byte(address & 0xFF);

    // Read data
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = spi_flash_send_byte(DUMMY_BYTE);
    }

    spi_flash_deselect();
}
