#ifndef BER_H
#define BER_H

int calculate_bit_errors(
    const int *transmitted_bits,
    const int *received_bits,
    int bit_count
);

double calculate_ber(
    int bit_errors,
    int bit_count
);

#endif