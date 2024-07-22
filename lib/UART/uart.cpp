#include <Arduino.h>

// Using serial port plotter software
void serial_update_plotter(float volts0, float volts1, float norm, int sample_num, int total_sample, int cycle_num) {
    float current_time_ms = total_sample * 200 / 1000;
    
    //Serial.printf("Cycle: %d, Total Sample: %d, Cycle Sample: %d, Time: %lu ms -- PDy: %.3f V, PDo: %.3f V, Norm: %.3f\n", 
    //    cycle_num, total_sample, sample_num, current_time, volts0, volts1, norm);
    
    // Serial.printf("$%d %d %d %lu %.3f %.3f %.3f;", 
    //     cycle_num, total_sample, sample_num, current_time, volts0, volts1, norm);

    Serial.printf("$%.2f %.3f %.3f %.5f;", current_time_ms, volts0, volts1, norm); // follow the serial port plotter
    vTaskDelay(20);
}

// Record the data into the CSV file using Python script
void serial_update_record(float volts0, float volts1, float norm, int sample_num, int total_sample, int cycle_num) {
    float current_time_ms = total_sample * 200 / 1000;
    
    //Serial.printf("Cycle: %d, Total Sample: %d, Cycle Sample: %d, Time: %lu ms -- PDy: %.3f V, PDo: %.3f V, Norm: %.3f\n", 
    //    cycle_num, total_sample, sample_num, current_time, volts0, volts1, norm);
    
    // Serial.printf("$%d %d %d %lu %.3f %.3f %.3f;", 
    //     cycle_num, total_sample, sample_num, current_time, volts0, volts1, norm);

    Serial.printf("%.2f %.3f %.3f %.5f\n", current_time_ms, volts0, volts1, norm); // follow the serial port plotter
    vTaskDelay(20);
}