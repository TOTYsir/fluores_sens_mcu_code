#include <Adafruit_ADS1X15.h>
#include "ads1115.h"
#include "led.h"

Adafruit_ADS1115 ads;

void adc_init() {
    if (!ads.begin()) {
        Serial.println("Failed to initialise ADC");
        // while (1); // Keep waiting
    }
    ads.setDataRate(RATE_ADS1115_250SPS);
}

void adc_read_store(float values0[], float values1[], int sample_cnt) {
    int16_t adc0 = ads.readADC_SingleEnded(0);
    int16_t adc1 = ads.readADC_SingleEnded(1);
    float volts0 = ads.computeVolts(adc0);
    float volts1 = ads.computeVolts(adc1);

    if (sample_cnt < SAMPLES_PER_ON_CYCLE + SAMPLES_PER_OFF_CYCLE) {
        values0[sample_cnt] = volts0;
        values1[sample_cnt] = volts1;
    }
}


