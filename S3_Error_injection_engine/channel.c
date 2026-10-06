#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define INPUT_FILE  "original_packet.txt"
#define OUTPUT_FILE "serial_channel.txt"

#define MAX_PACKET_SIZE 1000

// Generate random number between 0 and 100
double random_percentage()
{
    return ((double)rand() / RAND_MAX) * 100.0;
}

// Calculate packet size
int get_packet_size(char packet[])
{
    int size = 0;

    while (packet[size] != '\0')
    {
        size++;
    }

    return size;
}

// Corrupt one byte in the packet
void corrupt_packet(char packet[])
{
    int size = get_packet_size(packet);

    if (size <= 0)
        return;

    // Choose a random position
    int position = rand() % size;

    // Change the character
    if (packet[position] >= '0' &&
        packet[position] <= '9')
    {
        packet[position] = 'F';
    }
    else
    {
        packet[position] = '0';
    }

    printf("\n*** DATA CORRUPTION OCCURRED ***\n");
    printf("Corrupted character position: %d\n", position);
}

int main()
{
    FILE *input;
    FILE *output;

    char packet[MAX_PACKET_SIZE];

    double loss_probability;
    double corruption_probability;

    int max_delay;
    int delay;

    double random_value;

    // Initialize random number generator
    srand((unsigned int)time(NULL));

    printf("========================================\n");
    printf("       VIRTUAL SERIAL CHANNEL\n");
    printf("         ERROR INJECTION ENGINE\n");
    printf("========================================\n");

    // User configuration
    printf("\nEnter packet loss probability (%%): ");
    scanf("%lf", &loss_probability);

    printf("Enter data corruption probability (%%): ");
    scanf("%lf", &corruption_probability);

    printf("Enter maximum transmission delay (ms): ");
    scanf("%d", &max_delay);

    // Open original packet
    input = fopen(INPUT_FILE, "r");

    if (input == NULL)
    {
        printf("\nERROR: Cannot open %s\n", INPUT_FILE);
        printf("Run the transmitter first.\n");
        return 1;
    }

    // Read packet
    int index = 0;

    while (index < MAX_PACKET_SIZE - 1 &&
           fscanf(input, "%c", &packet[index]) == 1)
    {
        index++;
    }

    packet[index] = '\0';

    fclose(input);

    printf("\n----------------------------------------\n");
    printf("ORIGINAL PACKET\n");
    printf("----------------------------------------\n");

    printf("%s\n", packet);

    // Generate random value for packet loss
    random_value = random_percentage();

    printf("\nRandom Loss Value : %.2f%%\n",
           random_value);

    // Check packet loss
    if (random_value < loss_probability)
    {
        printf("\n========================================\n");
        printf("          PACKET LOST\n");
        printf("========================================\n");

        // Create empty channel file
        output = fopen(OUTPUT_FILE, "w");

        if (output != NULL)
        {
            fclose(output);
        }

        return 0;
    }

    printf("\nPacket is not lost.\n");

    // Generate random value for corruption
    random_value = random_percentage();

    printf("Random Corruption Value : %.2f%%\n",
           random_value);

    // Check corruption
    if (random_value < corruption_probability)
    {
        corrupt_packet(packet);
    }
    else
    {
        printf("No data corruption occurred.\n");
    }

    // Generate transmission delay
    if (max_delay > 0)
    {
        delay = rand() % (max_delay + 1);
    }
    else
    {
        delay = 0;
    }

    printf("\nTransmission Delay : %d ms\n",
           delay);

    Sleep(delay);

    // Write packet to channel
    output = fopen(OUTPUT_FILE, "w");

    if (output == NULL)
    {
        printf("\nERROR: Cannot create %s\n",
               OUTPUT_FILE);

        return 1;
    }

    fprintf(output, "%s", packet);

    fclose(output);

    printf("\n----------------------------------------\n");
    printf("CHANNEL RESULT\n");
    printf("----------------------------------------\n");

    printf("Packet delivered to receiver.\n");

    printf("\nFinal packet:\n");
    printf("%s\n", packet);

    printf("\nChannel simulation completed.\n");

    return 0;
}