#ifndef __ADS1115_H
#define __ADS1115_H

void adc_init(void);
void adc_read_store(float values0[], float values1[], int sample_cnt);

#endif
