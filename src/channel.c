#include "channel.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void add_awgn_noise(
    const double *signal,
    double *received_signal,
    int sample_count,
    double noise_amplitude
)
{
    for (int i = 0; i < sample_count; i++)
    {
        /*
         * Generate two random values between 0 and 1.
         */
        double u1 = (double)rand() / RAND_MAX;
        double u2 = (double)rand() / RAND_MAX;

        /*
         * Box-Muller transform.
         * Converts uniform random numbers
         * into approximately Gaussian noise.
         */
        if (u1 < 0.000001)
            u1 = 0.000001;

        double gaussian_noise =
            sqrt(-2.0 * log(u1)) *
            cos(2.0 * M_PI * u2);

        received_signal[i] =
            signal[i] +
            noise_amplitude * gaussian_noise;
    }
}