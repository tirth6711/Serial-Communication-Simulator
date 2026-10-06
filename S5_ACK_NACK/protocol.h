#ifndef PROTOCOL_H
#define PROTOCOL_H

// ============================================
// Protocol Configuration
// ============================================

#define START_BYTE 0xAA
#define END_BYTE   0x55

#define DEVICE_ID  0x01
#define DATA_TYPE  0x01

#define MAX_DATA_SIZE   100
#define MAX_PACKET_SIZE 256

#define MAX_RETRIES 3


// ============================================
// Packet Structure
// ============================================

typedef struct
{
    unsigned char bytes[MAX_PACKET_SIZE];

    int length;

    unsigned char sequence;

    unsigned char data_length;

    char data[MAX_DATA_SIZE + 1];

} Packet;


// ============================================
// Checksum Function
// ============================================

unsigned char calculate_checksum(
    unsigned char device_id,
    unsigned char data_type,
    unsigned char sequence,
    unsigned char length,
    const unsigned char *data
);


// ============================================
// Transmitter Function
// ============================================

Packet create_packet(
    unsigned char sequence,
    const char *message
);


// ============================================
// Channel Function
// ============================================

int transmit_through_channel(
    Packet input,
    Packet *output,
    int loss_probability,
    int corruption_probability,
    int delay_ms
);


// ============================================
// Receiver Function
// ============================================

int validate_packet(
    Packet packet,
    unsigned char expected_sequence,
    char *received_message,
    unsigned char *received_sequence
);

#endif