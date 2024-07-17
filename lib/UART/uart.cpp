#include <Arduino.h>

void serial_update(float volts0, float volts1, int sample_num, int total_sample, int cycle_num) {
    unsigned long elapsedTime = millis();
    Serial.printf("Cycle: %d, Total Sample: %d, Cycle Sample: %d, Time: %lu ms, PDy Voltage: %.3f V, PDo Voltage: %.3f V\n", 
        cycle_num, total_sample, sample_num, elapsedTime, volts0, volts1);
}