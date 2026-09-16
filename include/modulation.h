#ifndef MODULATION_H
#define MODULATION_H

void bpsk_modulate(
    const int *bits,
    int bit_count,
    double *signal,
    int samples_per_bit
);

#endif
