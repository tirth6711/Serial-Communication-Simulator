#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "protocol.h"


int transmit_through_channel(
    Packet input,
    Packet *output,
    int loss_probability,
    int corruption_probability,
    int delay_ms
)
{
    printf("\n  [CHANNEL] ");

    // Simulate transmission delay
    if (delay_ms > 0)
    {
        Sleep(delay_ms);
    }

    // Random packet loss
    int loss_random = rand() % 100;

    if (loss_random < loss_probability)
    {
        printf("PACKET LOST\n");

        return 0;
    }

    // Copy original packet
    *output = input;

    // Random corruption
    int corruption_random = rand() % 100;

    if (corruption_random < corruption_probability)
    {
        /*
         * Change one actual byte in the packet.
         *
         * We avoid changing START and END.
         *
         * Packet structure:
         * 0 = START
         * 1 = DEVICE ID
         * 2 = DATA TYPE
         * 3 = SEQUENCE
         * 4 = LENGTH
         * 5... = DATA
         */

        if (output->length > 6)
        {
            int position = 5 + (rand() % (output->length - 6));

            unsigned char old_value = output->bytes[position];

            // Flip one bit
            output->bytes[position] ^= 0x01;

            printf("CORRUPTED\n");
            printf("  [CHANNEL] Byte %d changed: %02X -> %02X\n",
                   position,
                   old_value,
                   output->bytes[position]);

            return 2;
        }
    }

    printf("PACKET DELIVERED\n");

    return 1;
}