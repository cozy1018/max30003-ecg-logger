#ifndef MAX30003_H
#define MAX30003_H

#include <stdint.h>
#include <stdbool.h>


void max30003_write_register(uint8_t reg_addr, uint32_t data);
uint32_t max30003_read_register(uint8_t reg_addr);
int32_t max30003_read_ecg_sample(void);
float max30003_convert_to_mv(int32_t raw_value);
bool max30003_init(void);
bool max30003_data_ready(void);

void max30003_test_gain(void);

#endif // MAX30003_H
