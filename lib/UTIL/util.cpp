#include "util.h"


float get_norm(float values0[], float values1[]) {
    // CHANNEL 0
    float on_avg_ch0 = calculate_average(values0, 1, SAMPLES_PER_ON_CYCLE + 1); // start index = 1
    float off_avg_ch0 = calculate_average(values0, SAMPLES_PER_ON_CYCLE + 1, SAMPLES_PER_OFF_CYCLE);

    // CHANNEL 1
    float on_avg_ch1 = calculate_average(values1, 1, SAMPLES_PER_ON_CYCLE + 1);
    float off_avg_ch1 = calculate_average(values1, SAMPLES_PER_ON_CYCLE + 1, SAMPLES_PER_OFF_CYCLE);

    return (on_avg_ch0 - off_avg_ch0) / (on_avg_ch1 - off_avg_ch1);
}


float calculate_average(float *array, int left, int right) {
    float sum = 0;
    for (int i = left; i < right; i++) {
        sum += array[i];
    }
    return sum / (right - left);
}