# MAX30003 ECG Logger using Silicon Labs EUSART Bare Metal (SPI)

This project demonstrates how to read real-time ECG signals from the MAX30003 biopotential analog front-end sensor using a Silicon Labs MCU. The communication between the MCU and MAX30003 is established via SPI, and the data is sent over the virtual COM port (VCOM) using EUSART in a bare-metal (no RTOS) environment.

The received ECG data is streamed to the host computer via USB and logged as both raw and converted millivolt (mV) values.

## Project Overview

- **Platform:** Silicon Labs MCU (tested with EFR32MG24B310F1536IM48)
- **Sensor:** MAX30003 
- **Communication:** SPI (Sensor to MCU), UART (MCU to PC)
- **Gain:** ~20 V/V  
- **Sampling Rate:** ~150 Hz
- **Output Format:** `raw` and `mV` values

## Key Files

| File            | Description                                                                 |
|-----------------|-----------------------------------------------------------------------------|
| `app.c`         | Main application logic for SPI initialization, sensor data read, and UART output |
| `max30003.c`    | SPI communication driver and data handling for MAX30003                    |
| `max30003.h`    | Header file for MAX30003 register maps and function declarations           |

## Example Output

Data will be logged in the format:
```
raw, mv
11520, 0.328
11510, 0.326
…
```
