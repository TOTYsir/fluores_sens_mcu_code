#ifndef __ADS1115_H
#define __ADS1115_H

void adc_init(void);
void adc_read_store(float photodiode_readings[3][50], int &data_index);

#endif
