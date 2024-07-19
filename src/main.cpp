#include <Arduino.h>

#include <ads1115.h>
#include <ssd1306.h>
#include <led.h>
#include <ble.h>
#include <uart.h>
#include <util.h>

// Sample period
#define SAMPLING_TIME_MS 200

// Serial configuration
#define BAUD_RATE 115200

// ADC readings configuration
float pd_0[SAMPLES_PER_ON_CYCLE+SAMPLES_PER_OFF_CYCLE];
float pd_1[SAMPLES_PER_ON_CYCLE+SAMPLES_PER_OFF_CYCLE];

// PWM settings
float pwm_duty_cycle = 0.70;

// Counter initialisation
int sampleCounter = 1;
int totalSampleCounter = 1;
int cycleCounter = 1;

float norm_val = 0.0;



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

    int data_index = sampleCounter - 1;
    adc_read_store(pd_0, pd_1, data_index);
    float volts0 = pd_0[data_index];
    float volts1 = pd_1[data_index];

    if (sampleCounter == SAMPLES_PER_ON_CYCLE + SAMPLES_PER_OFF_CYCLE) {
        norm_val = get_norm(pd_0, pd_1);
    }

    serial_update(volts0, volts1, norm_val, sampleCounter, totalSampleCounter, cycleCounter);
    lcd_flush(volts0, volts1, norm_val);
    ble_send(volts0, volts1, norm_val);

    sampleCounter++;
    totalSampleCounter++;

    if (cycle_finish_flag) {
        sampleCounter = 1; // ++
        cycleCounter++;
        
        memset(pd_0, 0, sizeof(pd_0));
        memset(pd_1, 0, sizeof(pd_1));
    }
    
    unsigned long endMillis = millis(); 
    unsigned long elapsedTime = endMillis - startMillis;
    // Period control
    if (elapsedTime < SAMPLING_TIME_MS) {
        vTaskDelay((SAMPLING_TIME_MS - elapsedTime) / portTICK_PERIOD_MS);
    }
}
