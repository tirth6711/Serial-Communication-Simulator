#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHANNEL_FILE "serial_channel.txt"

#define START_BYTE 0xAA
#define END_BYTE   0x55

#define MAX_DATA_SIZE 100
#define MAX_PACKET_SIZE 1000

unsigned char calculate_checksum(
    unsigned char device_id,
    unsigned char data_type,
    unsigned char sequence,
    unsigned char length,
    unsigned char data[]
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

int main()
{
    FILE *file;

    char packet[MAX_PACKET_SIZE];

    unsigned char expected_sequence = 1;

    int packet_count = 0;

    printf("========================================\n");
    printf("        DEVICE B - RECEIVER\n");
    printf("========================================\n");

    file = fopen(CHANNEL_FILE, "r");

    if (file == NULL)
    {
        printf("ERROR: No packets available.\n");
        return 1;
    }

    while (fgets(packet,
                 sizeof(packet),
                 file) != NULL)
    {
        packet_count++;

        packet[strcspn(packet, "\n")] = '\0';

        printf("\n========================================\n");

        printf("PACKET RECEIVED #%d\n",
               packet_count);

        printf("========================================\n");

        printf("Raw packet:\n%s\n",
               packet);

        unsigned int value;

        unsigned char start;
        unsigned char device_id;
        unsigned char data_type;
        unsigned char sequence;
        unsigned char length;

        unsigned char data[MAX_DATA_SIZE];

        unsigned char received_checksum;
        unsigned char end;

        int index = 0;

        // START
        if (sscanf(packet + index,
                   "%x",
                   &value) != 1)
        {
            printf("ERROR: Cannot read START.\n");
            continue;
        }

        start = value;

        // Move to next byte
        while (packet[index] != ' ' &&
               packet[index] != '\0')
            index++;

        while (packet[index] == ' ')
            index++;

        if (start != START_BYTE)
        {
            printf("ERROR: Invalid START.\n");
            continue;
        }

        // DEVICE ID
        sscanf(packet + index,
               "%x",
               &value);

        device_id = value;

        while (packet[index] != ' ' &&
               packet[index] != '\0')
            index++;

        while (packet[index] == ' ')
            index++;

        // DATA TYPE
        sscanf(packet + index,
               "%x",
               &value);

        data_type = value;

        while (packet[index] != ' ' &&
               packet[index] != '\0')
            index++;

        while (packet[index] == ' ')
            index++;

        // SEQUENCE
        sscanf(packet + index,
               "%x",
               &value);

        sequence = value;

        while (packet[index] != ' ' &&
               packet[index] != '\0')
            index++;

        while (packet[index] == ' ')
            index++;

        // LENGTH
        sscanf(packet + index,
               "%x",
               &value);

        length = value;

        while (packet[index] != ' ' &&
               packet[index] != '\0')
            index++;

        while (packet[index] == ' ')
            index++;

        // DATA
        for (int i = 0; i < length; i++)
        {
            sscanf(packet + index,
                   "%x",
                   &value);

            data[i] = value;

            while (packet[index] != ' ' &&
                   packet[index] != '\0')
                index++;

            while (packet[index] == ' ')
                index++;
        }

        // CHECKSUM
        sscanf(packet + index,
               "%x",
               &value);

        received_checksum = value;

        while (packet[index] != ' ' &&
               packet[index] != '\0')
            index++;

        while (packet[index] == ' ')
            index++;

        // END
        sscanf(packet + index,
               "%x",
               &value);

        end = value;

        // Display packet information
        printf("\nSTART      : %02X\n", start);
        printf("DEVICE ID  : %02X\n", device_id);
        printf("DATA TYPE  : %02X\n", data_type);
        printf("SEQUENCE   : %02X\n", sequence);
        printf("LENGTH     : %02X\n", length);

        printf("DATA       : ");

        for (int i = 0; i < length; i++)
        {
            printf("%02X ", data[i]);
        }

        printf("\n");

        printf("CHECKSUM   : %02X\n",
               received_checksum);

        printf("END        : %02X\n",
               end);

        // Check END
        if (end != END_BYTE)
        {
            printf("\nERROR: Invalid END byte.\n");
            continue;
        }

        // Calculate checksum
        unsigned char calculated_checksum;

        calculated_checksum =
            calculate_checksum(
                device_id,
                data_type,
                sequence,
                length,
                data
            );

        printf("\nReceived checksum   : %02X",
               received_checksum);

        printf("\nCalculated checksum : %02X\n",
               calculated_checksum);

        if (received_checksum !=
            calculated_checksum)
        {
            printf("\nSTATUS: CHECKSUM ERROR\n");
            printf("Packet is corrupted.\n");

            continue;
        }

        // Sequence check
        printf("\nExpected sequence : %02X\n",
               expected_sequence);

        printf("Received sequence : %02X\n",
               sequence);

        if (sequence == expected_sequence)
        {
            printf("SEQUENCE STATUS: CORRECT\n");

            expected_sequence++;
        }
        else if (sequence > expected_sequence)
        {
            printf("SEQUENCE STATUS: PACKET LOSS DETECTED\n");

            printf("Missing packet(s): ");

            for (int i = expected_sequence;
                 i < sequence;
                 i++)
            {
                printf("%02X ", i);
            }

            printf("\n");

            expected_sequence = sequence + 1;
        }
        else
        {
            printf("SEQUENCE STATUS: DUPLICATE/OLD PACKET\n");
        }

        // Display message
        printf("\nReceived Message: ");

        for (int i = 0; i < length; i++)
        {
            printf("%c", data[i]);
        }

        printf("\n");
    }

    fclose(file);

    printf("\n========================================\n");
    printf("Receiver processing completed.\n");
    printf("========================================\n");

    return 0;
}