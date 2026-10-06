#include <stdio.h>
#include <string.h>
#include <windows.h>

#define START_BYTE 0xAA
#define END_BYTE   0x55
#define DEVICE_ID  0x01
#define DATA_TYPE  0x01
#define MAX_DATA_SIZE 100

unsigned char calculate_checksum(
    unsigned char device_id,
    unsigned char data_type,
    unsigned char sequence,
    unsigned char length,
    char data[]
)
{
    unsigned int sum = 0;

    sum += device_id;
    sum += data_type;
    sum += sequence;
    sum += length;

    for (int i = 0; i < length; i++)
    {
        sum += (unsigned char)data[i];
    }

    return sum & 0xFF;
}

int main()
{
    int number_of_packets;
    char message[MAX_DATA_SIZE];

    printf("========================================\n");
    printf("       SERIAL TRANSMITTER - STEP 4\n");
    printf("========================================\n\n");

    printf("Enter number of packets: ");
    scanf("%d", &number_of_packets);

    // Remove the previous contents only ONCE
    FILE *file = fopen("original_packet.txt", "w");

    if (file == NULL)
    {
        printf("Error opening original_packet.txt\n");
        return 1;
    }

    fclose(file);

    // Now create packets one by one
    for (int packet = 1; packet <= number_of_packets; packet++)
    {
        printf("\nEnter message for Packet %d: ", packet);

        // Remove newline left by previous scanf
        getchar();

        fgets(message, sizeof(message), stdin);

        // Remove newline from message
        message[strcspn(message, "\n")] = '\0';

        unsigned char sequence = packet;
        unsigned char length = strlen(message);

        unsigned char checksum =
            calculate_checksum(
                DEVICE_ID,
                DATA_TYPE,
                sequence,
                length,
                message
            );

        // IMPORTANT: use "a" so previous packets are NOT erased
        file = fopen("original_packet.txt", "a");

        if (file == NULL)
        {
            printf("Error opening original_packet.txt\n");
            return 1;
        }

        // Write START
        fprintf(file, "%02X ", START_BYTE);

        // Device ID
        fprintf(file, "%02X ", DEVICE_ID);

        // Data Type
        fprintf(file, "%02X ", DATA_TYPE);

        // Sequence number
        fprintf(file, "%02X ", sequence);

        // Length
        fprintf(file, "%02X ", length);

        // Data
        for (int i = 0; i < length; i++)
        {
            fprintf(file, "%02X ", (unsigned char)message[i]);
        }

        // Checksum
        fprintf(file, "%02X ", checksum);

        // END
        fprintf(file, "%02X\n", END_BYTE);

        fclose(file);

        printf("Packet %d created successfully.\n", packet);
        printf("Sequence : %02X\n", sequence);
        printf("Message  : %s\n", message);
        printf("Length   : %02X\n", length);
        printf("Checksum : %02X\n", checksum);
    }

    printf("\n========================================\n");
    printf("All packets stored in original_packet.txt\n");
    printf("========================================\n");

    return 0;
}