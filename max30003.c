#include "max30003.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_eusart.h"
#include "sl_sleeptimer.h"
#include <stdio.h>

#define MAX30003_CS_PORT    gpioPortA
#define MAX30003_CS_PIN     7

// SPI transfer function
static uint8_t spi_transfer_simple(uint8_t data) {
    // Clear receive buffer
    while (EUSART1->STATUS & EUSART_STATUS_RXFL) {
        (void)EUSART1->RXDATA;
    }

    // Send data
    EUSART1->TXDATA = data;

    // Wait for transmit complete
    while (!(EUSART1->STATUS & EUSART_STATUS_TXC));

    // Wait for receive complete
    while (!(EUSART1->STATUS & EUSART_STATUS_RXFL));

    return EUSART1->RXDATA;
}

// Write register
void max30003_write_register(uint8_t reg_addr, uint32_t data) {
    // CS high to prepare
    GPIO_PinOutSet(MAX30003_CS_PORT, MAX30003_CS_PIN);
    sl_sleeptimer_delay_millisecond(2);

    // CS low to start transfer
    GPIO_PinOutClear(MAX30003_CS_PORT, MAX30003_CS_PIN);
    sl_sleeptimer_delay_millisecond(1);

    // Send 32 clock cycles
    spi_transfer_simple((reg_addr << 1) | 0x00);
    spi_transfer_simple((data >> 16) & 0xFF);
    spi_transfer_simple((data >> 8) & 0xFF);
    spi_transfer_simple(data & 0xFF);

    // Ensure 32nd clock completes before raising CS
    sl_sleeptimer_delay_millisecond(1);
    GPIO_PinOutSet(MAX30003_CS_PORT, MAX30003_CS_PIN);

    // Wait for chip to process
    sl_sleeptimer_delay_millisecond(10);
}

// Read register
uint32_t max30003_read_register(uint8_t reg_addr) {
    GPIO_PinOutSet(MAX30003_CS_PORT, MAX30003_CS_PIN);
    sl_sleeptimer_delay_millisecond(1);

    GPIO_PinOutClear(MAX30003_CS_PORT, MAX30003_CS_PIN);
    sl_sleeptimer_delay_millisecond(1);

    // Send read command
    spi_transfer_simple((reg_addr << 1) | 0x01);

    // Read 3 bytes of data
    uint8_t b0 = spi_transfer_simple(0x00);
    uint8_t b1 = spi_transfer_simple(0x00);
    uint8_t b2 = spi_transfer_simple(0x00);

    sl_sleeptimer_delay_millisecond(1);
    GPIO_PinOutSet(MAX30003_CS_PORT, MAX30003_CS_PIN);

    return ((uint32_t)b0 << 16) | ((uint32_t)b1 << 8) | b2;
}

// Read ECG sample (FIFO Burst Read)
int32_t max30003_read_ecg_sample(void) {
    GPIO_PinOutClear(MAX30003_CS_PORT, MAX30003_CS_PIN);
    sl_sleeptimer_delay_millisecond(1);

    // ECG_FIFO_BURST command (0x21 << 1 | 0x01 = 0x43)
    spi_transfer_simple(0x43);

    uint8_t b0 = spi_transfer_simple(0x00);
    uint8_t b1 = spi_transfer_simple(0x00);
    uint8_t b2 = spi_transfer_simple(0x00);

    sl_sleeptimer_delay_millisecond(1);
    GPIO_PinOutSet(MAX30003_CS_PORT, MAX30003_CS_PIN);

    // Combine 24-bit data and sign extend
    int32_t value = ((uint32_t)b0 << 16) | ((uint32_t)b1 << 8) | b2;
    if (value & 0x800000) {
        value |= 0xFF000000;
    }

    return value;
}

// Convert to millivolts
float max30003_convert_to_mv(int32_t raw_value) {
    // Extract ECG data (bit 6-23, 18 bits)
    uint32_t unsigned_raw = (uint32_t)raw_value;
    uint32_t ecg_data_unsigned = (unsigned_raw >> 6) & 0x3FFFF;

    // Convert to signed number
    int32_t ecg_data;
    if (ecg_data_unsigned & 0x20000) {
        ecg_data = (int32_t)ecg_data_unsigned - 0x40000;
    } else {
        ecg_data = (int32_t)ecg_data_unsigned;
    }

    // Convert to mV
    // VREF = 1.0V, ADC = 18-bit, Gain = 20V/V (default)
    return (float)ecg_data * 1000.0f / (131072.0f * 20.0f);
}

// Initialize MAX30003
bool max30003_init(void) {
    GPIO_PinOutSet(MAX30003_CS_PORT, MAX30003_CS_PIN);
    sl_sleeptimer_delay_millisecond(500);

    printf("\n=== MAX30003 Init (125 sps) ===\n\n");

    // Test SPI
    uint32_t info = max30003_read_register(0x0F);
    if(info == 0x000000 || info == 0xFFFFFF) {
        printf("[FAIL] SPI error\n");
        return false;
    }
    printf("[OK] SPI connected\n");

    // Software Reset
    max30003_write_register(0x08, 0x000000);
    sl_sleeptimer_delay_millisecond(500);

    // ✅ Step 1: Configure CNFG_ECG first (RATE = 10 for 125 sps)
    printf("Configuring RATE=10 (125 sps with FMSTR=01)...\n");
    max30003_write_register(0x15, 0x800000);  // RATE=10, GAIN=00(default)
    sl_sleeptimer_delay_millisecond(100);

    // ✅ Step 2: Configure CNFG_GEN (FMSTR=01, EN_ECG=1)
    printf("Configuring FMSTR=01 (32000 Hz)...\n");
    max30003_write_register(0x10, 0x180000);  // FMSTR=01, EN_ECG=1
    sl_sleeptimer_delay_millisecond(200);

    // SYNCH
    max30003_write_register(0x09, 0x000000);
    sl_sleeptimer_delay_millisecond(200);

    // Configure EMUX
    max30003_write_register(0x14, 0x000000);
    sl_sleeptimer_delay_millisecond(100);

    // Final SYNCH
    max30003_write_register(0x09, 0x000000);
    sl_sleeptimer_delay_millisecond(500);

    // Wait for first sample
    uint32_t timeout = 0;
    while(timeout < 100) {
        uint32_t status = max30003_read_register(0x01);
        if (status & (1 << 23)) {
            // Verify configuration
            uint32_t gen = max30003_read_register(0x10);
            uint32_t ecg = max30003_read_register(0x15);

            uint8_t fmstr = (gen >> 20) & 0x03;
            uint8_t rate = (ecg >> 22) & 0x03;
            uint8_t gain = (ecg >> 16) & 0x03;

            printf("\n=== Configuration ===\n");
            printf("CNFG_GEN: 0x%06lX\n", (unsigned long)gen);
            printf("CNFG_ECG: 0x%06lX\n", (unsigned long)ecg);
            printf("FMSTR: %d ", fmstr);
            if (fmstr == 1) printf("(32000 Hz) [OK]\n");
            else printf("(not 32000 Hz) [WARN]\n");

            printf("RATE: %d ", rate);
            if (rate == 2) printf("(125 sps) [OK]\n");
            else printf("(not 125 sps) [WARN]\n");

            printf("GAIN: %d ", gain);
            if (gain == 0) printf("(20 V/V)\n\n");
            else printf("(%d)\n\n", gain);

            if (fmstr == 1 && rate == 2) {
                printf("[SUCCESS] 125 sps configured!\n\n");
            } else {
                printf("[WARNING] Chip may be using different config\n\n");
            }

            return true;
        }
        sl_sleeptimer_delay_millisecond(10);
        timeout++;
    }

    printf("[FAIL] Timeout\n");
    return false;
}

// Check if data is ready
bool max30003_data_ready(void) {
    uint32_t status = max30003_read_register(0x01);
    bool eint = (status & (1 << 23)) != 0;

    return eint;
}

