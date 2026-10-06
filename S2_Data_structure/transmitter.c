#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define CHANNEL_FILE "serial_channel.txt"

#define START_BYTE  0xAA
#define END_BYTE    0x55

#define DEVICE_ID   0x01
#define DATA_TYPE   0x01

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
    char message[MAX_DATA_SIZE];
    unsigned char data[MAX_DATA_SIZE];

    unsigned char length;
    unsigned char checksum;

    FILE *file;
    printf("DEVICE A - TRANSMITTER\n");

    printf("\nEnter message to send: ");
    fgets(message, sizeof(message), stdin);

    // Remove newline
    message[strcspn(message, "\n")] = '\0';
    length = strlen(message);

    if (length == 0)
    {
        printf("Error: Empty message.\n");
        return 1;
    }

    // Convert message to bytes
    for (int i = 0; i < length; i++)
    {
        data[i] = (unsigned char)message[i];
    }

    // Calculate checksum
    checksum = calculate_checksum(
        DEVICE_ID,
        DATA_TYPE,
        length,
        data
    );

    // Open virtual serial channel
    file = fopen(CHANNEL_FILE, "w");

    if (file == NULL)
    {
        printf("Error: Cannot open serial channel.\n");
        return 1;
    }

    // Write packet to channel in hexadecimal format
    fprintf(file, "%02X ", START_BYTE);
    fprintf(file, "%02X ", DEVICE_ID);
    fprintf(file, "%02X ", DATA_TYPE);
    fprintf(file, "%02X ", length);

    for (int i = 0; i < length; i++)
    {
        fprintf(file, "%02X ", data[i]);
    }

    fprintf(file, "%02X ", checksum);
    fprintf(file, "%02X", END_BYTE);
    fclose(file);

    // Display packet
    printf("PACKET CREATED:\n");
    printf("START      : %02X\n", START_BYTE);
    printf("DEVICE ID  : %02X\n", DEVICE_ID);
    printf("DATA TYPE  : %02X\n", DATA_TYPE);
    printf("LENGTH     : %02X\n", length);
    printf("DATA       : ");

    for (int i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }

    printf("\nCHECKSUM   : %02X\n", checksum);
    printf("END        : %02X\n", END_BYTE);

    // Display complete packet
    printf("\nComplete Packet:\n");

    printf("%02X ", START_BYTE);
    printf("%02X ", DEVICE_ID);
    printf("%02X ", DATA_TYPE);
    printf("%02X ", length);

    for (int i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }

    printf("%02X ", checksum);
    printf("%02X\n", END_BYTE);

    // Simulate byte-by-byte transmission
    printf("BYTE BY BYTE TRANSMISSION\n");

    printf("Sending START      : %02X\n", START_BYTE);
    Sleep(200);

    printf("Sending DEVICE ID  : %02X\n", DEVICE_ID);
    Sleep(200);

    printf("Sending DATA TYPE  : %02X\n", DATA_TYPE);
    Sleep(200);

    printf("Sending LENGTH     : %02X\n", length);
    Sleep(200);

    for (int i = 0; i < length; i++)
    {
        printf("Sending DATA       : %02X (%c)\n",
               data[i],
               data[i]);

        Sleep(200);
    }

    printf("Sending CHECKSUM   : %02X\n", checksum);
    Sleep(200);

    printf("Sending END        : %02X\n", END_BYTE);

    printf("\nTransmission completed successfully.\n");

    return 0;
}