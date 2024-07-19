#include <Arduino.h>

void serial_update(float volts0, float volts1, float norm, int sample_num, int total_sample, int cycle_num) {
    unsigned long current_time = total_sample * 200;
    
    //Serial.printf("Cycle: %d, Total Sample: %d, Cycle Sample: %d, Time: %lu ms -- PDy: %.3f V, PDo: %.3f V, Norm: %.3f\n", 
    //    cycle_num, total_sample, sample_num, current_time, volts0, volts1, norm);
    
    // Serial.printf("$%d %d %d %lu %.3f %.3f %.3f;", 
    //     cycle_num, total_sample, sample_num, current_time, volts0, volts1, norm);

    Serial.printf("$%.3f %.3f %.5f;", volts0, volts1, norm); // follow the serial port plotter
    vTaskDelay(20);
}