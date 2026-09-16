#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "security.h"
#include "modulation.h"
#include "channel.h"
#include "demodulation.h"
#include "decoding.h"
#include "ber.h"
#include "snr.h"
#include "snr_ber.h"
#include "fft.h"

#define MAX_MESSAGE 256
#define MAX_CIPHERTEXT (MAX_MESSAGE + SECURITY_TAG_SIZE)
#define MAX_BITS (MAX_CIPHERTEXT * 8)
#define SAMPLES_PER_BIT 20


void print_binary(unsigned char c)
{
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (c >> i) & 1);
    }
}


int main()
{
    char message[MAX_MESSAGE];
    char key[100];

    unsigned char security_key[SECURITY_KEY_SIZE];

    unsigned char nonce[SECURITY_NONCE_SIZE];

    unsigned char ciphertext[MAX_MESSAGE + SECURITY_TAG_SIZE];

    unsigned char received_ciphertext[
        MAX_MESSAGE + SECURITY_TAG_SIZE
    ];

    unsigned char decrypted_message[MAX_MESSAGE + 1];

    size_t ciphertext_length = 0;
    size_t decrypted_length = 0;

   int bits[MAX_BITS];
int received_bits[MAX_BITS];

double signal[
    MAX_BITS * SAMPLES_PER_BIT
];

double received_signal[
    MAX_BITS * SAMPLES_PER_BIT
];


    /* =========================================================
       INITIALIZE SECURITY LIBRARY
       ========================================================= */

    if (initialize_security() != 0)
    {
        printf("Security initialization failed.\n");
        return 1;
    }


    /* =========================================================
       HEADER
       ========================================================= */

    printf("============================================\n");
    printf("   SECURE DIGITAL COMMUNICATION SIMULATOR\n");
    printf("============================================\n\n");


    /* =========================================================
       INPUT MESSAGE
       ========================================================= */

    printf("Enter your secret message: ");

    fgets(
        message,
        sizeof(message),
        stdin
    );

    message[strcspn(message, "\n")] = '\0';

    int message_length =
        strlen(message);

    if (message_length == 0)
    {
        printf("Message cannot be empty.\n");
        return 1;
    }


    /* =========================================================
       SECURITY PASSWORD
       ========================================================= */

    printf("Enter security password: ");

    fgets(
        key,
        sizeof(key),
        stdin
    );

    key[strcspn(key, "\n")] = '\0';

    if (strlen(key) == 0)
    {
        printf("Security password cannot be empty.\n");
        return 1;
    }


    /* =========================================================
       ORIGINAL MESSAGE
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("ORIGINAL MESSAGE\n");
    printf("--------------------------------------------\n");

    printf("%s\n", message);


    /* =========================================================
       DERIVE SECURITY KEY
       ========================================================= */

    if (derive_security_key(
            key,
            security_key) != 0)
    {
        printf("Security key generation failed.\n");
        return 1;
    }


    /* =========================================================
       SECURE ENCRYPTION
       ========================================================= */

    if (secure_encrypt(
            (unsigned char *)message,
            message_length,
            security_key,
            nonce,
            ciphertext,
            &ciphertext_length) != 0)
    {
        printf("Encryption failed.\n");
        return 1;
    }


    printf("\n--------------------------------------------\n");
    printf("SECURE ENCRYPTED DATA\n");
    printf("--------------------------------------------\n");

    for (size_t i = 0;
         i < ciphertext_length;
         i++)
    {
        printf(
            "%02X ",
            ciphertext[i]
        );
    }

    printf("\n");


    /* =========================================================
       NONCE
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("TRANSMISSION NONCE\n");
    printf("--------------------------------------------\n");

    for (int i = 0;
         i < SECURITY_NONCE_SIZE;
         i++)
    {
        printf(
            "%02X ",
            nonce[i]
        );
    }

    printf("\n");


    /* =========================================================
       CONVERT CIPHERTEXT TO BITS
       ========================================================= */

    int bit_count = 0;

    for (size_t i = 0;
         i < ciphertext_length;
         i++)
    {
        unsigned char byte =
            ciphertext[i];

        for (int j = 7;
             j >= 0;
             j--)
        {
            bits[bit_count] =
                (byte >> j) & 1;

            bit_count++;
        }
    }


    printf("\n--------------------------------------------\n");
    printf("SECURE TRANSMISSION BITS\n");
    printf("--------------------------------------------\n");

    for (int i = 0;
         i < bit_count;
         i++)
    {
        printf(
            "%d",
            bits[i]
        );

        if ((i + 1) % 8 == 0)
        {
            printf(" ");
        }
    }

    printf("\n");


    /* =========================================================
       BPSK MODULATION
       ========================================================= */

    bpsk_modulate(
        bits,
        bit_count,
        signal,
        SAMPLES_PER_BIT
    );

    int total_samples =
        bit_count *
        SAMPLES_PER_BIT;


    /* =========================================================
       CHANNEL - AWGN
       ========================================================= */

    double noise_amplitude = 0.30;

    add_awgn_noise(
        signal,
        received_signal,
        total_samples,
        noise_amplitude
    );


    /* =========================================================
       SNR CALCULATION
       ========================================================= */

    double signal_power =
        calculate_signal_power(
            signal,
            total_samples
        );

    double noise_power =
        calculate_noise_power(
            signal,
            received_signal,
            total_samples
        );

    double snr_db =
        calculate_snr_db(
            signal_power,
            noise_power
        );


    /* =========================================================
       BPSK DEMODULATION
       ========================================================= */

    bpsk_demodulate(
        received_signal,
        received_bits,
        bit_count,
        SAMPLES_PER_BIT
    );


    /* =========================================================
       BER CALCULATION
       ========================================================= */

    int bit_errors =
        calculate_bit_errors(
            bits,
            received_bits,
            bit_count
        );

    double ber =
        calculate_ber(
            bit_errors,
            bit_count
        );


    /* =========================================================
       RECEIVED BITS
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("RECEIVER - BPSK DEMODULATION\n");
    printf("--------------------------------------------\n");

    printf("Recovered bits:\n");

    for (int i = 0;
         i < bit_count;
         i++)
    {
        printf(
            "%d",
            received_bits[i]
        );

        if ((i + 1) % 8 == 0)
        {
            printf(" ");
        }
    }

    printf("\n");


    /* =========================================================
       CONVERT RECEIVED BITS TO CIPHERTEXT
       ========================================================= */

    int received_byte_count =
        (bit_count + 7) / 8;

    for (int i = 0;
         i < received_byte_count;
         i++)
    {
        unsigned char value = 0;

        for (int j = 0;
             j < 8;
             j++)
        {
            int index =
                i * 8 + j;

            value <<= 1;

            if (index < bit_count)
            {
                value |=
                    received_bits[index];
            }
        }

        received_ciphertext[i] =
            value;
    }


    /* =========================================================
       TRANSMISSION ANALYSIS
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("TRANSMISSION ANALYSIS\n");
    printf("--------------------------------------------\n");

    printf(
        "Total bits       : %d\n",
        bit_count
    );

    printf(
        "Correct bits     : %d\n",
        bit_count - bit_errors
    );

    printf(
        "Bit errors       : %d\n",
        bit_errors
    );

    printf(
        "BER              : %.6f\n",
        ber
    );


    /* =========================================================
       CHANNEL ANALYSIS
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("CHANNEL ANALYSIS\n");
    printf("--------------------------------------------\n");

    printf(
        "Channel          : AWGN\n"
    );

    printf(
        "Noise level      : %.2f\n",
        noise_amplitude
    );

    printf(
        "Signal Power     : %.6f\n",
        signal_power
    );

    printf(
        "Noise Power      : %.6f\n",
        noise_power
    );

    printf(
        "SNR              : %.2f dB\n",
        snr_db
    );


    /* =========================================================
       RECEIVED CIPHERTEXT
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("RECEIVED CIPHERTEXT\n");
    printf("--------------------------------------------\n");

    for (int i = 0;
         i < received_byte_count;
         i++)
    {
        printf(
            "%02X ",
            received_ciphertext[i]
        );
    }

    printf("\n");


    /* =========================================================
       FFT SIGNAL ANALYSIS
       ========================================================= */

    printf("\nStarting FFT signal analysis...\n");

    calculate_fft(
        signal,
        total_samples
    );


    /* =========================================================
       SNR vs BER ANALYSIS
       ========================================================= */

    printf("\nStarting SNR vs BER analysis...\n");

    run_snr_ber_analysis(
        bits,
        bit_count,
        SAMPLES_PER_BIT
    );


    /* =========================================================
       AUTHENTICATION + DECRYPTION
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("SECURITY VERIFICATION\n");
    printf("--------------------------------------------\n");

    if (secure_decrypt(
            received_ciphertext,
            received_byte_count,
            nonce,
            security_key,
            decrypted_message,
            &decrypted_length) != 0)
    {
        printf(
            "AUTHENTICATION FAILED!\n"
        );

        printf(
            "The received ciphertext was\n"
            "modified or the security key is wrong.\n"
        );

        return 1;
    }


    decrypted_message[
        decrypted_length
    ] = '\0';


    printf(
        "Authentication     : PASSED\n"
    );

    printf(
        "Integrity check    : PASSED\n"
    );


    /* =========================================================
       FINAL RECEIVED MESSAGE
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("RECEIVED MESSAGE\n");
    printf("--------------------------------------------\n");

    printf(
        "%s\n",
        decrypted_message
    );


    /* =========================================================
       SECURITY SUMMARY
       ========================================================= */

    printf("\n--------------------------------------------\n");
    printf("SECURITY SUMMARY\n");
    printf("--------------------------------------------\n");

    printf(
        "Encryption         : Authenticated encryption\n"
    );

    printf(
        "Authentication     : Poly1305\n"
    );

    printf(
        "Encryption cipher  : XSalsa20\n"
    );

    printf(
        "Nonce               : Random\n"
    );

    printf(
        "Message integrity   : Verified\n"
    );


    /* =========================================================
       COMPLETION
       ========================================================= */

    printf("\n============================================\n");
    printf("Secure transmission completed.\n");
    printf("============================================\n");


    return 0;
}