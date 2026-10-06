#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "protocol.h"


// Function to display packet in hexadecimal
void print_packet(Packet packet)
{
    printf("  Packet: ");

    for (int i = 0; i < packet.length; i++)
    {
        printf("%02X ", packet.bytes[i]);
    }

    printf("\n");
}


// Save original packet
void save_original_packet(Packet packet)
{
    FILE *file = fopen("original_packet.txt", "a");

    if (file == NULL)
    {
        printf("Error opening original_packet.txt\n");
        return;
    }

    for (int i = 0; i < packet.length; i++)
    {
        fprintf(file, "%02X ", packet.bytes[i]);
    }

    fprintf(file, "\n");

    fclose(file);
}


// Save delivered packet
void save_channel_packet(Packet packet)
{
    FILE *file = fopen("serial_channel.txt", "a");

    if (file == NULL)
    {
        printf("Error opening serial_channel.txt\n");
        return;
    }

    for (int i = 0; i < packet.length; i++)
    {
        fprintf(file, "%02X ", packet.bytes[i]);
    }

    fprintf(file, "\n");

    fclose(file);
}


int main()
{
    srand((unsigned int)time(NULL));


    printf("\n");
    printf("====================================================\n");
    printf("   INTELLIGENT SERIAL COMMUNICATION SIMULATOR\n");
    printf("                  \n");
    printf("        ACK / NACK + RETRANSMISSION\n");
    printf("====================================================\n");


    // ---------------------------------------------
    // Channel configuration
    // ---------------------------------------------

    int loss_probability;
    int corruption_probability;
    int delay_ms;

    printf("\nEnter packet loss probability (0-100): ");
    scanf("%d", &loss_probability);

    printf("Enter corruption probability (0-100): ");
    scanf("%d", &corruption_probability);

    printf("Enter transmission delay in milliseconds: ");
    scanf("%d", &delay_ms);


    // ---------------------------------------------
    // Number of packets
    // ---------------------------------------------

    int number_of_packets;

    printf("\nEnter number of packets: ");
    scanf("%d", &number_of_packets);


    if (number_of_packets <= 0)
    {
        printf("Invalid number of packets.\n");
        return 1;
    }


    if (number_of_packets > 255)
    {
        printf("Maximum 255 packets allowed.\n");
        return 1;
    }


    // ---------------------------------------------
    // Clear old files
    // ---------------------------------------------

    FILE *file;

    file = fopen("original_packet.txt", "w");

    if (file != NULL)
    {
        fclose(file);
    }


    file = fopen("serial_channel.txt", "w");

    if (file != NULL)
    {
        fclose(file);
    }


    // ---------------------------------------------
    // Statistics
    // ---------------------------------------------

    int total_packets = number_of_packets;

    int successful_packets = 0;
    int lost_packets = 0;
    int corrupted_packets = 0;
    int retransmissions = 0;
    int failed_packets = 0;


    unsigned char expected_sequence = 1;


    // ---------------------------------------------
    // Process each packet
    // ---------------------------------------------

    for (int i = 0; i < number_of_packets; i++)
    {
        char message[MAX_DATA_SIZE + 1];


        printf("\n");
        printf("====================================================\n");

        printf("Enter message for Packet %02X: ",
               expected_sequence);

        getchar();

        fgets(message, sizeof(message), stdin);

        message[strcspn(message, "\n")] = '\0';


        // Create packet
        Packet original_packet =
            create_packet(
                expected_sequence,
                message
            );


        printf("\n[TRANSMITTER]\n");

        printf("Sequence : %02X\n",
               expected_sequence);

        printf("Data     : %s\n",
               message);

        print_packet(original_packet);


        // Save original packet
        save_original_packet(original_packet);


        // -----------------------------------------
        // Retry mechanism
        // -----------------------------------------

        int packet_success = 0;

        for (int attempt = 0;
             attempt <= MAX_RETRIES;
             attempt++)
        {
            if (attempt == 0)
            {
                printf("\n  Attempt 1: Sending packet...\n");
            }
            else
            {
                printf("\n  Attempt %d: RETRANSMITTING packet...\n",
                       attempt + 1);

                retransmissions++;
            }


            Packet received_packet;


            // Send through channel
            int channel_result =
                transmit_through_channel(
                    original_packet,
                    &received_packet,
                    loss_probability,
                    corruption_probability,
                    delay_ms
                );


            // -------------------------------------
            // Packet lost
            // -------------------------------------

            if (channel_result == 0)
            {
                lost_packets++;

                printf("  [TRANSMITTER] No packet received.\n");
                printf("  [TRANSMITTER] Waiting for ACK...\n");

                printf("  [TRANSMITTER] TIMEOUT\n");

                continue;
            }


            // -------------------------------------
            // Packet delivered
            // -------------------------------------

            save_channel_packet(received_packet);


            char received_message[MAX_DATA_SIZE + 1];

            unsigned char received_sequence;


            // Receiver validation
            int valid =
                validate_packet(
                    received_packet,
                    expected_sequence,
                    received_message,
                    &received_sequence
                );


            // -------------------------------------
            // VALID → ACK
            // -------------------------------------

            if (valid)
            {
                printf("\n  [RECEIVER] ACK %02X\n",
                       expected_sequence);

                printf("  [TRANSMITTER] ACK received.\n");

                printf("  STATUS: SUCCESS ✓\n");

                successful_packets++;

                packet_success = 1;

                break;
            }


            // -------------------------------------
            // INVALID → NACK
            // -------------------------------------

            else
            {
                corrupted_packets++;

                printf("\n  [RECEIVER] NACK %02X\n",
                       expected_sequence);

                printf("  [TRANSMITTER] NACK received.\n");

                printf("  STATUS: RETRANSMISSION REQUIRED\n");
            }
        }


        // -----------------------------------------
        // Maximum retry reached
        // -----------------------------------------

        if (!packet_success)
        {
            printf("\n  !!! PACKET %02X FAILED !!!\n",
                   expected_sequence);

            failed_packets++;
        }


        // Move to next sequence number
        expected_sequence++;
    }


    // ---------------------------------------------
    // Final statistics
    // ---------------------------------------------

    printf("\n\n");
    printf("====================================================\n");
    printf("                 FINAL RESULTS\n");
    printf("====================================================\n");

    printf("Total Packets       : %d\n",
           total_packets);

    printf("Successful Packets  : %d\n",
           successful_packets);

    printf("Failed Packets      : %d\n",
           failed_packets);

    printf("Packet Loss Events  : %d\n",
           lost_packets);

    printf("Corruption Events   : %d\n",
           corrupted_packets);

    printf("Retransmissions     : %d\n",
           retransmissions);


    if (total_packets > 0)
    {
        float reliability =
            ((float)successful_packets /
             total_packets) * 100.0f;

        printf("Reliability         : %.2f%%\n",
               reliability);
    }


    printf("====================================================\n");

    printf("\nFiles generated:\n");
    printf("1. original_packet.txt\n");
    printf("2. serial_channel.txt\n");

    printf("\nSimulation completed.\n");

    return 0;
}