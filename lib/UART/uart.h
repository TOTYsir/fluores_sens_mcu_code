#ifndef __UART_H
#define __UART_H

void serial_update_plotter(float volts0, float volts1, float norm, int sample_num, int total_sample, int cycle_num);
void serial_update_record(float volts0, float volts1, float norm, int sample_num, int total_sample, int cycle_num);

#endif