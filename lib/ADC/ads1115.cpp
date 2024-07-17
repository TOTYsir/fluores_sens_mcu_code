#include <Adafruit_ADS1X15.h>
#include "ads1115.h"

Adafruit_ADS1115 ads;

void adc_init() {
    if (!ads.begin()) {
        Serial.println("Failed to initialise ADC");
        // while (1); // Keep waiting
    }
    ads.setDataRate(RATE_ADS1115_250SPS);
}


void adc_read_store(float photodiode_readings[2][50], int &data_index) {
    int16_t adc0 = ads.readADC_SingleEnded(0);
    int16_t adc1 = ads.readADC_SingleEnded(1);
    float volts0 = ads.computeVolts(adc0);
    float volts1 = ads.computeVolts(adc1);

    if (data_index < 50) {
        photodiode_readings[0][data_index] = volts0;
        photodiode_readings[1][data_index] = volts1;
        data_index++;
    } else {
        for (int i = 0; i < 49; i++) {
            photodiode_readings[0][i] = photodiode_readings[0][i + 1];
            photodiode_readings[1][i] = photodiode_readings[1][i + 1];
        }
        photodiode_readings[0][49] = volts0;
        photodiode_readings[1][49] = volts1;
    }
}

