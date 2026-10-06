#include <stdio.h>
#include <string.h>
#include "protocol.h"


// Calculate checksum
unsigned char calculate_checksum(
    unsigned char device_id,
    unsigned char data_type,
    unsigned char sequence,
    unsigned char length,
    const unsigned char *data
)
{
    unsigned int sum = 0;

    sum += device_id;
    sum += data_type;
    sum += sequence;
    sum += length;

    for (int i = 0; i < length; i++)
    {
        sum += data[i];
    }

    return (unsigned char)(sum & 0xFF);
}


// Create packet
Packet create_packet(unsigned char sequence, const char *message)
{
    Packet packet;

    memset(&packet, 0, sizeof(Packet));

    unsigned char length = (unsigned char)strlen(message);

    packet.sequence = sequence;
    packet.data_length = length;

    strcpy(packet.data, message);

    int index = 0;

    // START
    packet.bytes[index++] = START_BYTE;

    // DEVICE ID
    packet.bytes[index++] = DEVICE_ID;

    // DATA TYPE
    packet.bytes[index++] = DATA_TYPE;

    // SEQUENCE NUMBER
    packet.bytes[index++] = sequence;

    // LENGTH
    packet.bytes[index++] = length;

    // DATA
    for (int i = 0; i < length; i++)
    {
        packet.bytes[index++] = (unsigned char)message[i];
    }

    // CHECKSUM
    unsigned char checksum =
        calculate_checksum(
            DEVICE_ID,
            DATA_TYPE,
            sequence,
            length,
            (unsigned char *)message
        );

    packet.bytes[index++] = checksum;

    // END
    packet.bytes[index++] = END_BYTE;

    packet.length = index;

    return packet;
}