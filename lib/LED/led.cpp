#include <Arduino.h>
#include "led.h"

int sampleCounter = 0;
int totalSampleCounter = 0;
int cycleCounter = 0;
bool LEDState = HIGH;

void led_pwm_init() {
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, 0);
}

void led_control(float dutyCycle) {
    sampleCounter++;
    totalSampleCounter++;
    int samplesPerCycle = (LEDState == HIGH) ? SAMPLES_PER_ON_CYCLE : SAMPLES_PER_OFF_CYCLE;

    if (sampleCounter >= samplesPerCycle) {
        LEDState = !LEDState;
        if (LEDState) {
            ledcWrite(PWM_CHANNEL, (int)(MAX_DUTY_CYCLE * dutyCycle));
        } else {
            ledcWrite(PWM_CHANNEL, 0);
        }

        Serial.printf("Cycle: %d, Total Sample: %d, Cycle Sample: %d, ", 
            cycleCounter, totalSampleCounter, sampleCounter);
        
        sampleCounter = 0;
        if (LEDState == HIGH) {
            cycleCounter++;
        }
    }
}
