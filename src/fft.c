#include "fft.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

void calculate_fft(
    const double *signal,
    int sample_count
)
{
    printf("\n============================================\n");
    printf("          FFT SIGNAL ANALYSIS\n");
    printf("============================================\n");

    printf("\nFrequency-domain analysis of BPSK signal\n\n");

    /*
     * Calculate the magnitude of selected
     * frequency components.
     */

    int max_frequency = sample_count / 2;

    if (max_frequency > 20)
        max_frequency = 20;

    printf("%-15s %-15s\n",
           "Frequency",
           "Magnitude");

    printf("--------------------------------------------\n");

    for (int k = 0; k <= max_frequency; k++)
    {
        double real = 0.0;
        double imag = 0.0;

        for (int n = 0; n < sample_count; n++)
        {
            double angle =
                (2.0 * PI * k * n) /
                sample_count;

            real +=
                signal[n] * cos(angle);

            imag -=
                signal[n] * sin(angle);
        }

        double magnitude =
            sqrt(
                real * real +
                imag * imag
            );

        magnitude =
            magnitude / sample_count;

        printf("%-15d %-15.6f\n",
               k,
               magnitude);
    }

    printf("--------------------------------------------\n");

    printf("FFT analysis completed.\n");

    printf("============================================\n");
}