#include <Arduino.h>

#include <ads1115.h>
#include <ssd1306.h>
#include <led.h>
#include <ble.h>
#include <uart.h>

// Sample period
#define SAMPLING_TIME_MS 200

// Serial configuration
#define BAUD_RATE 115200
// ADC readings configuration
#define MAX_SIZE 50
float photodiode_readings[3][MAX_SIZE];
int data_index = 0;
// PWM settings
float pwm_duty_cycle = 0.70;

// Counter initialisation
int sampleCounter = 1;
int totalSampleCounter = 1;
int cycleCounter = 1;



void setup() {
    Serial.begin(BAUD_RATE);
    lcd_init();
    adc_init();
    led_pwm_init(pwm_duty_cycle);
    ble_init();
}

void loop() {
   unsigned long startMillis = millis();

    int cycle_finish_flag = led_control(pwm_duty_cycle, sampleCounter);

    adc_read_store(photodiode_readings, data_index);
    float volts0 = photodiode_readings[0][data_index - 1];
    float volts1 = photodiode_readings[1][data_index - 1];
    lcd_flush(volts0, volts1);
    serial_update(volts0, volts1, sampleCounter, totalSampleCounter, cycleCounter);
    ble_send(volts0, volts1);

    sampleCounter++;
    totalSampleCounter++;

    if (cycle_finish_flag) {
        sampleCounter = 1; // ++
        cycleCounter++;
    }
    
    unsigned long endMillis = millis(); 
    unsigned long elapsedTime = endMillis - startMillis;
    // Period control
    if (elapsedTime < SAMPLING_TIME_MS) {
        vTaskDelay((SAMPLING_TIME_MS - elapsedTime) / portTICK_PERIOD_MS);
    }
}
