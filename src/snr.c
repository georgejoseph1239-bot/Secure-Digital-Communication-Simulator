#include "snr.h"
#include <math.h>

double calculate_signal_power(
    const double *signal,
    int sample_count
)
{
    if (sample_count == 0)
        return 0.0;

    double power = 0.0;

    for (int i = 0; i < sample_count; i++)
    {
        power += signal[i] * signal[i];
    }

    return power / sample_count;
}

double calculate_noise_power(
    const double *signal,
    const double *received_signal,
    int sample_count
)
{
    if (sample_count == 0)
        return 0.0;

    double noise_power = 0.0;

    for (int i = 0; i < sample_count; i++)
    {
        double noise = received_signal[i] - signal[i];
        noise_power += noise * noise;
    }

    return noise_power / sample_count;
}

double calculate_snr_db(
    double signal_power,
    double noise_power
)
{
    if (noise_power <= 0.0)
        return 999.0;

    return 10.0 * log10(signal_power / noise_power);
}
