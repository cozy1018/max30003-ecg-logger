#include "app_iostream_eusart.h"
#include "max30003.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_eusart.h"
#include "sl_sleeptimer.h"
#include <stdio.h>

bool g_init_success = false;

void app_init(void)
{
    app_iostream_eusart_init();

    printf("\n=== SPI Setup ===\n\n");

    // SPI initialization
    CMU_ClockEnable(cmuClock_EUSART1, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortC, 3, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortC, 2, gpioModeInput, 0);
    GPIO_PinModeSet(gpioPortA, 7, gpioModePushPull, 1);

    EUSART1->EN_CLR = EUSART_EN_EN;
    while (EUSART1->EN & EUSART_EN_EN);

    EUSART1->CFG0 = EUSART_CFG0_SYNC | EUSART_CFG0_MSBF;
    EUSART1->CFG1 = 0;
    EUSART1->CFG2 = EUSART_CFG2_MASTER
                  | EUSART_CFG2_CLKPOL_IDLELOW
                  | EUSART_CFG2_CLKPHA_SAMPLELEADING;

    EUSART1->CLKDIV = 400 << _EUSART_CLKDIV_DIV_SHIFT;
    EUSART1->FRAMECFG = EUSART_FRAMECFG_DATABITS_EIGHT;

    EUSART1->EN_SET = EUSART_EN_EN;
    while (!(EUSART1->EN & EUSART_EN_EN));
    EUSART1->CMD = EUSART_CMD_TXEN | EUSART_CMD_RXEN;

    GPIO->EUSARTROUTE[1].TXROUTE = (gpioPortC << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
                                  | (3 << _GPIO_EUSART_TXROUTE_PIN_SHIFT);
    GPIO->EUSARTROUTE[1].RXROUTE = (gpioPortC << _GPIO_EUSART_RXROUTE_PORT_SHIFT)
                                  | (2 << _GPIO_EUSART_RXROUTE_PIN_SHIFT);
    GPIO->EUSARTROUTE[1].SCLKROUTE = (gpioPortC << _GPIO_EUSART_SCLKROUTE_PORT_SHIFT)
                                    | (1 << _GPIO_EUSART_SCLKROUTE_PIN_SHIFT);
    GPIO->EUSARTROUTE[1].ROUTEEN = GPIO_EUSART_ROUTEEN_TXPEN
                                  | GPIO_EUSART_ROUTEEN_RXPEN
                                  | GPIO_EUSART_ROUTEEN_SCLKPEN;

    printf("[OK] SPI configured\n\n");

    // Initialize MAX30003
    g_init_success = max30003_init();

    if (g_init_success) {
        printf("MAX30003 initialized successfully!\n\n");
    } else {
        printf("MAX30003 initialization failed!\n");
    }
}

void app_process_action(void)
{
    app_iostream_eusart_process_action();

    if (g_init_success) {
        if (max30003_data_ready()) {
            int32_t raw = max30003_read_ecg_sample();
            float mv = max30003_convert_to_mv(raw);
            int32_t mv_int = (int32_t)(mv * 10000.0f);

            printf("%ld,%ld\n", (long)raw, (long)mv_int);
        }
    } else {
        sl_sleeptimer_delay_millisecond(1000);
    }
}
