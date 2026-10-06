#include <stdio.h>
#include <string.h>

#include "protocol.h"


int validate_packet(
    Packet packet,
    unsigned char expected_sequence,
    char *received_message,
    unsigned char *received_sequence
)
{

    if (packet.length < 7)
    {
        printf("  [RECEIVER] Packet too short\n");
        return 0;
    }


    // -----------------------------------------
    // Check START
    // -----------------------------------------

    if (packet.bytes[0] != START_BYTE)
    {
        printf("  [RECEIVER] START byte error\n");
        return 0;
    }


    // -----------------------------------------
    // Check DEVICE ID
    // -----------------------------------------

    if (packet.bytes[1] != DEVICE_ID)
    {
        printf("  [RECEIVER] Invalid Device ID\n");
        return 0;
    }


    // -----------------------------------------
    // Check DATA TYPE
    // -----------------------------------------

    if (packet.bytes[2] != DATA_TYPE)
    {
        printf("  [RECEIVER] Invalid Data Type\n");
        return 0;
    }


    // -----------------------------------------
    // Get sequence
    // -----------------------------------------

    unsigned char sequence = packet.bytes[3];

    *received_sequence = sequence;


    printf("  [RECEIVER] Sequence received: %02X\n",
           sequence);


    // -----------------------------------------
    // Check sequence number
    // -----------------------------------------

    if (sequence != expected_sequence)
    {
        printf("  [RECEIVER] Expected sequence: %02X\n",
               expected_sequence);

        printf("  [RECEIVER] Sequence mismatch!\n");

        return 0;
    }


    // -----------------------------------------
    // Get length
    // -----------------------------------------

    unsigned char length = packet.bytes[4];


    if (length > MAX_DATA_SIZE)
    {
        printf("  [RECEIVER] Invalid data length\n");
        return 0;
    }


    // Expected total packet length:

    int expected_packet_length =
        1 +     // START
        1 +     // DEVICE ID
        1 +     // DATA TYPE
        1 +     // SEQUENCE
        1 +     // LENGTH
        length +
        1 +     // CHECKSUM
        1;      // END


    if (packet.length != expected_packet_length)
    {
        printf("  [RECEIVER] Packet length error\n");
        return 0;
    }


    // -----------------------------------------
    // Check END
    // -----------------------------------------

    if (packet.bytes[packet.length - 1] != END_BYTE)
    {
        printf("  [RECEIVER] END byte error\n");
        return 0;
    }


    // -----------------------------------------
    // Extract data
    // -----------------------------------------

    int data_start = 5;

    for (int i = 0; i < length; i++)
    {
        received_message[i] =
            (char)packet.bytes[data_start + i];
    }

    received_message[length] = '\0';


    // -----------------------------------------
    // Get received checksum
    // -----------------------------------------

    unsigned char received_checksum =
        packet.bytes[5 + length];


    // -----------------------------------------
    // Calculate checksum again
    // -----------------------------------------

    unsigned char calculated_checksum =
        calculate_checksum(
            packet.bytes[1],
            packet.bytes[2],
            packet.bytes[3],
            packet.bytes[4],
            &packet.bytes[5]
        );


    printf("  [RECEIVER] Received checksum : %02X\n",
           received_checksum);

    printf("  [RECEIVER] Calculated checksum: %02X\n",
           calculated_checksum);


    // -----------------------------------------
    // Compare checksum
    // -----------------------------------------

    if (received_checksum != calculated_checksum)
    {
        printf("  [RECEIVER] CHECKSUM ERROR!\n");

        return 0;
    }


    // -----------------------------------------
    // Packet is valid
    // -----------------------------------------

    printf("  [RECEIVER] DATA: %s\n",
           received_message);

    printf("  [RECEIVER] PACKET VALID\n");

    return 1;
}