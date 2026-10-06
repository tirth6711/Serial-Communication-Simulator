#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define CHANNEL_FILE "serial_channel.txt"

#define START_BYTE  0xAA
#define END_BYTE    0x55

#define MAX_DATA_SIZE 100

// Calculate checksum
unsigned char calculate_checksum(
    unsigned char device_id,
    unsigned char data_type,
    unsigned char length,
    unsigned char data[]
)
{
    unsigned int sum = 0;

    sum = sum + device_id;
    sum = sum + data_type;
    sum = sum + length;

    for (int i = 0; i < length; i++)
    {
        sum += data[i];
    }

    return (unsigned char)(sum & 0xFF);
}

int main()
{
    FILE *file;

    unsigned int temp;

    unsigned char start;
    unsigned char device_id;
    unsigned char data_type;
    unsigned char length;

    unsigned char data[MAX_DATA_SIZE];

    unsigned char received_checksum;
    unsigned char end;

    unsigned char calculated_checksum;

    printf("DEVICE B - RECEIVER\n");
    printf("\nWaiting for packet...\n");

    Sleep(1000);

    // Open channel
    file = fopen(CHANNEL_FILE, "r");

    if (file == NULL)
    {
        printf("Error: No packet available.\n");
        return 1;
    }

    // Read START byte
    if (fscanf(file, "%x", &temp) != 1)
    {
        printf("Error: Cannot read START byte.\n");
        fclose(file);
        return 1;
    }

    start = (unsigned char)temp;

    // Check START
    if (start != START_BYTE)
    {
        printf("Error: Invalid START byte.\n");
        fclose(file);
        return 1;
    }

    // Read DEVICE ID
    fscanf(file, "%x", &temp);
    device_id = (unsigned char)temp;

    // Read DATA TYPE
    fscanf(file, "%x", &temp);
    data_type = (unsigned char)temp;

    // Read LENGTH
    fscanf(file, "%x", &temp);
    length = (unsigned char)temp;

    if (length > MAX_DATA_SIZE)
    {
        printf("Error: Invalid data length.\n");
        fclose(file);
        return 1;
    }

    // Read DATA
    for (int i = 0; i < length; i++)
    {
        fscanf(file, "%x", &temp);
        data[i] = (unsigned char)temp;
    }

    // Read CHECKSUM
    fscanf(file, "%x", &temp);
    received_checksum = (unsigned char)temp;

    // Read END
    fscanf(file, "%x", &temp);
    end = (unsigned char)temp;

    fclose(file);

    // Display received packet
    printf("PACKET RECEIVED\n");
    printf("START      : %02X\n", start);
    printf("DEVICE ID  : %02X\n", device_id);
    printf("DATA TYPE  : %02X\n", data_type);
    printf("LENGTH     : %02X\n", length);
    printf("DATA       : ");

    for (int i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }

    printf("\nCHECKSUM   : %02X\n", received_checksum);
    printf("END        : %02X\n", end);

    // Display complete packet
    printf("\nComplete Packet:\n");
    printf("%02X ", start);
    printf("%02X ", device_id);
    printf("%02X ", data_type);
    printf("%02X ", length);

    for (int i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }

    printf("%02X ", received_checksum);
    printf("%02X\n", end);

    // Validate END byte
    if (end != END_BYTE)
    {
        printf("\nERROR: Invalid END byte.\n");
        return 1;
    }

    // Calculate checksum again
    calculated_checksum = calculate_checksum(
        device_id,
        data_type,
        length,
        data
    );

    printf("CHECKSUM VERIFICATION\n");
    printf("Received Checksum  : %02X\n", received_checksum);
    printf("Calculated Checksum: %02X\n",
           calculated_checksum);

    if (received_checksum == calculated_checksum)
    {
        printf("\nCHECKSUM STATUS: PASS\n");
        printf("Packet is VALID.\n");
    }
    else
    {
        printf("\nCHECKSUM STATUS: FAIL\n");
        printf("Packet is INVALID.\n");
    }

    // Convert data back to message
    printf("\nReceived Message: ");

    for (int i = 0; i < length; i++)
    {
        printf("%c", data[i]);
    }

    printf("\n");
    return 0;
}