#ifndef SNR_H
#define SNR_H

double calculate_signal_power(
    const double *signal,
    int sample_count
);

double calculate_noise_power(
    const double *signal,
    const double *received_signal,
    int sample_count
);

double calculate_snr_db(
    double signal_power,
    double noise_power
);

#endif