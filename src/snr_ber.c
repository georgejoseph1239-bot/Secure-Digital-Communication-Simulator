#include "snr_ber.h"
#include "modulation.h"
#include "channel.h"
#include "demodulation.h"
#include "ber.h"
#include "snr.h"

#include <stdio.h>
#include <stdlib.h>

#define ANALYSIS_BITS 10000
#define ANALYSIS_SAMPLES_PER_BIT 20
#define ANALYSIS_SAMPLES \
    (ANALYSIS_BITS * ANALYSIS_SAMPLES_PER_BIT)


void run_snr_ber_analysis(
    const int *bits,
    int bit_count,
    int samples_per_bit
)
{
    /*
     * The original message is not used for the
     * statistical SNR-BER experiment.
     *
     * Instead, a large random bit sequence is
     * generated so that BER results are meaningful.
     */

    (void)bits;
    (void)bit_count;
    (void)samples_per_bit;


    /*
     * Allocate memory dynamically.
     *
     * This avoids stack overflow caused by
     * very large arrays.
     */

    int *analysis_bits =
        malloc(ANALYSIS_BITS * sizeof(int));

    int *received_bits =
        malloc(ANALYSIS_BITS * sizeof(int));

    double *signal =
        malloc(ANALYSIS_SAMPLES * sizeof(double));

    double *received_signal =
        malloc(ANALYSIS_SAMPLES * sizeof(double));


    /*
     * Check whether memory allocation was successful.
     */

    if (analysis_bits == NULL ||
        received_bits == NULL ||
        signal == NULL ||
        received_signal == NULL)
    {
        printf("\nError: Memory allocation failed.\n");

        free(analysis_bits);
        free(received_bits);
        free(signal);
        free(received_signal);

        return;
    }


    /* Generate random test bits */

    for (int i = 0; i < ANALYSIS_BITS; i++)
    {
        analysis_bits[i] = rand() % 2;
    }


    /*
     * Noise levels used for testing.
     *
     * Higher noise amplitude means
     * poorer channel quality.
     */

    double noise_levels[] =
    {
        0.05,
        0.10,
        0.20,
        0.30,
        0.50,
        0.70,
        1.00
    };


    int number_of_tests =
        sizeof(noise_levels) /
        sizeof(noise_levels[0]);


    /* Generate BPSK signal */

    bpsk_modulate(
        analysis_bits,
        ANALYSIS_BITS,
        signal,
        ANALYSIS_SAMPLES_PER_BIT
    );


    printf("\n============================================\n");
    printf("          SNR vs BER ANALYSIS\n");
    printf("============================================\n");

    printf("\nTest bits: %d\n", ANALYSIS_BITS);

    printf("\n%-15s %-15s %-15s\n",
           "Noise Level",
           "SNR (dB)",
           "BER");

    printf("--------------------------------------------\n");


    /*
     * Test the communication system at
     * different noise levels.
     */

    for (int test = 0;
         test < number_of_tests;
         test++)
    {
        double noise_amplitude =
            noise_levels[test];


        /* Add AWGN noise */

        add_awgn_noise(
            signal,
            received_signal,
            ANALYSIS_SAMPLES,
            noise_amplitude
        );


        /* Calculate signal power */

        double signal_power =
            calculate_signal_power(
                signal,
                ANALYSIS_SAMPLES
            );


        /* Calculate noise power */

        double noise_power =
            calculate_noise_power(
                signal,
                received_signal,
                ANALYSIS_SAMPLES
            );


        /* Calculate SNR */

        double snr_db =
            calculate_snr_db(
                signal_power,
                noise_power
            );


        /* Demodulate received signal */

        bpsk_demodulate(
            received_signal,
            received_bits,
            ANALYSIS_BITS,
            ANALYSIS_SAMPLES_PER_BIT
        );


        /* Calculate bit errors */

        int bit_errors =
            calculate_bit_errors(
                analysis_bits,
                received_bits,
                ANALYSIS_BITS
            );


        /* Calculate BER */

        double ber =
            calculate_ber(
                bit_errors,
                ANALYSIS_BITS
            );


        printf(
            "%-15.2f %-15.2f %-15.6f\n",
            noise_amplitude,
            snr_db,
            ber
        );
    }


    printf("--------------------------------------------\n");

    printf("Higher noise -> Lower SNR -> Higher BER\n");

    printf("============================================\n");


    /*
     * Release dynamically allocated memory.
     */

    free(analysis_bits);
    free(received_bits);
    free(signal);
    free(received_signal);
}