#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message_encode.h"

// Function prototypes for encoding and decoding
void encode_message();
void decode_message();

void run_cli()
{
    int choice;

    do
    {
        printf("\nSelect an option:\n");
        printf("1. Encode a message\n");
        printf("2. Decode a message\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character after number input

        switch (choice)
        {
        case 1:
            encode_message();
            break;
        case 2:
            decode_message();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid option, please try again.\n");
        }
    } while (choice != 0);
}

void encode_message()
{
    char input[1024];
    int outlen = 0;

    printf("Enter message to encode: ");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0; // Remove newline character if present

        uint8_t *encoded = sos_encode(input, &outlen);
        if (encoded)
        {
            printf("Encoded message: ");
            for (int i = 0; i < outlen; i++)
            {
                printf("%02X ", encoded[i]); // Print each byte in hexadecimal
            }
            printf("\n");
            free(encoded); // Remember to free the encoded message
        }
        else
        {
            printf("Encoding failed or no output.\n");
        }
    }
    else
    {
        printf("Failed to read input.\n");
    }
}

void decode_message()
{
    char input[1024]; // Assuming input is a sequence of hexadecimal bytes
    int outlen = 0;

    printf("Enter encoded message in hexadecimal: ");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0; // Remove newline character if present

        // Assuming sos_decode needs a packet_list* and we somehow construct it from input
        packet_list *packets = build_packet_list_from_input(input); // You need to implement this function
        char *decoded = sos_decode(packets);
        if (decoded)
        {
            printf("Decoded message: %s\n", decoded);
            free(decoded);
        }
        else
        {
            printf("Decoding failed or no output.\n");
        }

        // Assuming there's a function to free the packet list
        pl_free(packets);
    }
    else
    {
        printf("Failed to read input.\n");
    }
}
