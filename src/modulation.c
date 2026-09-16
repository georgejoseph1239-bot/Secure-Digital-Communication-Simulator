#include "modulation.h"
#include <math.h>

#define PI 3.14159265358979323846

void bpsk_modulate(
    const int *bits,
    int bit_count,
    double *signal,
    int samples_per_bit
)
{
    double carrier_frequency = 5.0;

    for (int i = 0; i < bit_count; i++)
    {
        double phase;

        if (bits[i] == 1)
        {
            phase = 0.0;
        }
        else
        {
            phase = PI;
        }

        for (int j = 0; j < samples_per_bit; j++)
        {
            double t = (double)j / samples_per_bit;

            signal[i * samples_per_bit + j] =
                cos(2.0 * PI * carrier_frequency * t + phase);
        }
    }
}