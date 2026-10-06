#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define CHANNEL_FILE "serial_channel.txt"

int main()
{
    char message[256];
    FILE *file;

    printf("         DEVICE B - RECEIVER         \n");
    
    printf("\nWaiting for data...\n");

    
    Sleep(1000); // Give transmitter time to create the file

    
    file = fopen(CHANNEL_FILE, "r"); // Open virtual serial channel

    if (file == NULL)
    {
        printf("Error: No data available.\n");
        return 1;
    }

    
    fgets(message, sizeof(message), file); // Read message

    fclose(file);

    printf("\n[RECEIVER]\n");
    printf("Data received successfully.\n");

    printf("\nReceived bytes:\n");

    
    for (int i = 0; i < strlen(message); i++) // Display byte-by-byte reception
    {
        printf("Received: %c\n", message[i]);

        
        Sleep(200); // Simulate receiving delay
    }

    printf("\nComplete message: %s\n", message);

    return 0;
}