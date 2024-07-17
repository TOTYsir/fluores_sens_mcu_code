#include <Arduino.h>
#include <ads1115.h>
#include <ssd1306.h>
#include <led.h>
#include <ble.h>
#include <uart.h>

// Serial configuration
#define BAUD_RATE 115200
// ADC readings configuration
#define MAX_SIZE 50
float photodiode_readings[3][MAX_SIZE];
int data_index = 0;
// PWM settings
float pwm_duty_cycle = 0.70;

extern int sampleCounter;
extern int totalSampleCounter;
extern int cycleCounter;


void setup() {
    Serial.begin(BAUD_RATE);
    lcd_init();
    adc_init();
    led_pwm_init();
    ble_init();
}

void loop() {
    led_control(pwm_duty_cycle);
    adc_read_store(photodiode_readings, data_index);
    float volts0 = photodiode_readings[0][data_index - 1];
    float volts1 = photodiode_readings[1][data_index - 1];
    lcd_flush(volts0, volts1);
    serial_update(volts0, volts1, sampleCounter, totalSampleCounter, cycleCounter);
    ble_send(volts0, volts1);
}
