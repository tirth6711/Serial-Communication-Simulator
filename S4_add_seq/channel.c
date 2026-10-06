#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define INPUT_FILE  "original_packet.txt"
#define OUTPUT_FILE "serial_channel.txt"

#define MAX_PACKET_SIZE 1000

double random_percentage()
{
    return ((double)rand() / RAND_MAX) * 100.0;
}

void corrupt_packet(char packet[])
{
    int length = strlen(packet);

    if (length <= 0)
        return;

    int position = rand() % length;

    if (packet[position] >= '0' &&
        packet[position] <= '9')
    {
        packet[position] = 'F';
    }
    else
    {
        packet[position] = '0';
    }

    printf("    CORRUPTION: Position %d modified\n",
           position);
}

int main()
{
    FILE *input;
    FILE *output;

    char packet[MAX_PACKET_SIZE];

    double loss_probability;
    double corruption_probability;

    int max_delay;

    int packet_count = 0;
    int lost_count = 0;
    int corrupted_count = 0;
    int delivered_count = 0;

    srand((unsigned int)time(NULL));

    printf("========================================\n");
    printf("     MULTI-PACKET CHANNEL SIMULATOR\n");
    printf("========================================\n");

    printf("\nPacket loss probability (%%): ");
    scanf("%lf", &loss_probability);

    printf("Data corruption probability (%%): ");
    scanf("%lf", &corruption_probability);

    printf("Maximum delay (ms): ");
    scanf("%d", &max_delay);

    input = fopen(INPUT_FILE, "r");

    if (input == NULL)
    {
        printf("\nERROR: Cannot open %s\n",
               INPUT_FILE);

        printf("Run transmitter first.\n");

        return 1;
    }

    // Clear previous channel output
    output = fopen(OUTPUT_FILE, "w");

    if (output == NULL)
    {
        printf("ERROR: Cannot create channel file.\n");
        fclose(input);
        return 1;
    }

    fclose(output);

    // Process each packet line
    while (fgets(packet,
                 sizeof(packet),
                 input) != NULL)
    {
        packet_count++;

        // Remove newline
        packet[strcspn(packet, "\n")] = '\0';

        printf("\n----------------------------------------\n");

        printf("PACKET %d\n", packet_count);

        printf("----------------------------------------\n");

        printf("Original:\n%s\n", packet);

        // Check packet loss
        double loss_value = random_percentage();

        printf("Loss random value: %.2f%%\n",
               loss_value);

        if (loss_value < loss_probability)
        {
            printf("RESULT: PACKET LOST\n");

            lost_count++;

            continue;
        }

        // Check corruption
        double corruption_value =
            random_percentage();

        printf("Corruption random value: %.2f%%\n",
               corruption_value);

        if (corruption_value < corruption_probability)
        {
            corrupt_packet(packet);

            corrupted_count++;
        }
        else
        {
            printf("No corruption.\n");
        }

        // Delay
        int delay = 0;

        if (max_delay > 0)
        {
            delay = rand() % (max_delay + 1);
        }

        printf("Transmission delay: %d ms\n",
               delay);

        Sleep(delay);

        // Append packet to output channel
        output = fopen(OUTPUT_FILE, "a");

        if (output == NULL)
        {
            printf("ERROR: Cannot open output channel.\n");

            fclose(input);

            return 1;
        }

        fprintf(output, "%s\n", packet);

        fclose(output);

        delivered_count++;

        printf("RESULT: PACKET DELIVERED\n");
    }

    fclose(input);

    printf("\n========================================\n");
    printf("       CHANNEL STATISTICS\n");
    printf("========================================\n");

    printf("Total packets      : %d\n",
           packet_count);

    printf("Delivered packets  : %d\n",
           delivered_count);

    printf("Lost packets       : %d\n",
           lost_count);

    printf("Corrupted packets  : %d\n",
           corrupted_count);

    if (packet_count > 0)
    {
        double loss_rate =
            ((double)lost_count / packet_count) * 100;

        printf("Packet loss rate   : %.2f%%\n",
               loss_rate);
    }

    printf("\nChannel simulation completed.\n");

    return 0;
}