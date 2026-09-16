#include "ber.h"

int calculate_bit_errors(
    const int *transmitted_bits,
    const int *received_bits,
    int bit_count
)
{
    int errors = 0;

    for (int i = 0; i < bit_count; i++)
    {
        if (transmitted_bits[i] != received_bits[i])
        {
            errors++;
        }
    }

    return errors;
}

double calculate_ber(
    int bit_errors,
    int bit_count
)
{
    if (bit_count == 0)
        return 0.0;

    return (double)bit_errors / bit_count;
}
