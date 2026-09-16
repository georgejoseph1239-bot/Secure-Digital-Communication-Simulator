#ifndef DECODING_H
#define DECODING_H

void bits_to_bytes(
    const int *bits,
    int bit_count,
    unsigned char *bytes,
    int byte_count
);

#endif