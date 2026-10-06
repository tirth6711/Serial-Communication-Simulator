#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define CHANNEL_FILE "serial_channel.txt"

int main()
{
    char message[256];
    FILE *file;

    printf("       DEVICE A - TRANSMITTER        \n");
   
    printf("Enter message to send: ");
    fgets(message, sizeof(message), stdin);

    
    message[strcspn(message, "\n")] = '\0'; // Remove newline from input

    
    file = fopen(CHANNEL_FILE, "w"); // Open virtual serial channel

    if (file == NULL)
    {
        printf("Error: Cannot open serial channel.\n");
        return 1;
    }

    
    fprintf(file, "%s", message); // Write message to virtual channel

    fclose(file);

    printf("\n[TRANSMITTER]\n");
    printf("Message: %s\n", message);

    printf("\nSending bytes:\n");

    
    for (int i = 0; i < strlen(message); i++) // Display byte-by-byte transmission
    {
        printf("Sending: %c\n", message[i]);

        // Simulate transmission delay
        Sleep(200);
    }

    printf("\nTransmission completed.\n");

    return 0;
}