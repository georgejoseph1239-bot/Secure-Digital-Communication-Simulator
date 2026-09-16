#ifndef CHANNEL_H
#define CHANNEL_H

void add_awgn_noise(
    const double *signal,
    double *received_signal,
    int sample_count,
    double noise_amplitude
);

#endif
