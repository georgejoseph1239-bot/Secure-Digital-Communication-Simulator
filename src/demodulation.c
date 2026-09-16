#include "demodulation.h"
#include <math.h>

#define PI 3.14159265358979323846

void bpsk_demodulate(
    const double *received_signal,
    int *bits,
    int bit_count,
    int samples_per_bit
)
{
    double carrier_frequency = 5.0;

    for (int i = 0; i < bit_count; i++)
    {
        double correlation = 0.0;

        for (int j = 0; j < samples_per_bit; j++)
        {
            double t = (double)j / samples_per_bit;

            /*
             * Generate the same carrier used
             * by the transmitter.
             */
            double carrier =
                cos(2.0 * PI * carrier_frequency * t);

            /*
             * Correlate received signal
             * with the carrier.
             */
            correlation +=
                received_signal[i * samples_per_bit + j]
                * carrier;
        }

        /*
         * BPSK decision:
         *
         * Positive correlation → 1
         * Negative correlation → 0
         */
        if (correlation >= 0)
            bits[i] = 1;
        else
            bits[i] = 0;
    }
}