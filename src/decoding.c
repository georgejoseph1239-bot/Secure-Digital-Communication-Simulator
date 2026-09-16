#include "decoding.h"

void bits_to_bytes(
    const int *bits,
    int bit_count,
    unsigned char *bytes,
    int byte_count
)
{
    for (int i = 0; i < byte_count; i++)
    {
        unsigned char value = 0;

        for (int j = 0; j < 8; j++)
        {
            int bit_index = i * 8 + j;

            if (bit_index < bit_count)
            {
                value = (value << 1) | bits[bit_index];
            }
        }

        bytes[i] = value;
    }
}