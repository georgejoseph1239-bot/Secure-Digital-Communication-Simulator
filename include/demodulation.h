#ifndef DEMODULATION_H
#define DEMODULATION_H

void bpsk_demodulate(
    const double *received_signal,
    int *bits,
    int bit_count,
    int samples_per_bit
);

#endif