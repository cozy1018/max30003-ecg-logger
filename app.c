#include "sl_bt_api.h"
#include "gatt_db.h"
#include "app.h"
#include "app_assert.h"
#include "app_iostream_eusart.h"
#include "max30003.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_eusart.h"
#include "sl_sleeptimer.h"
#include <stdio.h>
#include <string.h>

// Bluetooth variables
static uint8_t advertising_set_handle = 0xff;
static uint8_t connection_handle = 0xFF;

bool g_init_success = false;

/*
 * Purpose: Configure all hardware at startup
 */

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
        printf("MAX30003 initialized successfully!\n");
        printf("Sampling rate: 125 Hz\n\n");
    } else {
        printf("MAX30003 initialization failed!\n");
    }
}


/*
 * Purpose: Continuously acquire and transmit ECG data at 125 Hz
 */

void app_process_action(void)
{
    static uint32_t last_send_time = 0;
    uint32_t current_time = sl_sleeptimer_get_tick_count();

    app_iostream_eusart_process_action();

    if (g_init_success) {
        // Send data every 8ms = 125 Hz
        if (max30003_data_ready() &&
            (current_time - last_send_time) > sl_sleeptimer_ms_to_tick(8)) {
            last_send_time = current_time;

            // Read sensor
            int32_t raw = max30003_read_ecg_sample();
            float mv = max30003_convert_to_mv(raw);

            // Serial output
            int32_t mv_int = (int32_t)(mv * 10000.0f);
            printf("%ld,%ld\n", (long)raw, (long)mv_int);

            // Bluetooth output
            if (connection_handle != 0xFF) {
                // Convert to 0-255 range
                float normalized = (mv + 2.0f) / 4.0f * 255.0f;
                if (normalized < 0.0f) normalized = 0.0f;
                if (normalized > 255.0f) normalized = 255.0f;
                uint8_t ecg_value = (uint8_t)normalized;

                // Format and send
                char buffer[20];
                snprintf(buffer, sizeof(buffer), "%d\n", ecg_value);

                sl_bt_gatt_server_send_notification(
                  connection_handle,
                  gattdb_spp_tx,
                  strlen(buffer),
                  (uint8_t*)buffer
                );
            }
        }
    } else {
        sl_sleeptimer_delay_millisecond(1000);
    }
}


/*
 * Purpose: Handle Bluetooth stack events
 */

// Bluetooth event handler
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
      printf("[BLE] System boot\n");

      // Create advertising set
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Set advertising timing
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, 160, 0, 0);
      app_assert_status(sc);

      // Generate advertising data
      sc = sl_bt_legacy_advertiser_generate_data(
        advertising_set_handle,
        sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Start advertising
      sc = sl_bt_legacy_advertiser_start(
        advertising_set_handle,
        sl_bt_legacy_advertiser_connectable);
      app_assert_status(sc);

      printf("[BLE] Advertising started\n");
      break;

    case sl_bt_evt_connection_opened_id:
      connection_handle = evt->data.evt_connection_opened.connection;
      printf("[BLE] Connection opened (handle: %d)\n", connection_handle);
      break;

    case sl_bt_evt_connection_closed_id:
      connection_handle = 0xFF;
      printf("[BLE] Connection closed\n");

      // Restart advertising
      sc = sl_bt_legacy_advertiser_generate_data(
        advertising_set_handle,
        sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      sc = sl_bt_legacy_advertiser_start(
        advertising_set_handle,
        sl_bt_legacy_advertiser_connectable);
      app_assert_status(sc);

      printf("[BLE] Advertising restarted\n");
      break;

    default:
      break;
  }
}
