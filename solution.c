#include <stdio.h>
#include <stdint.h>

#define MAX_PACKET_DATA_LENGTH (58)

typedef struct data_packet_t {
    uint8_t id;
    uint8_t data_length;
    uint8_t data[MAX_PACKET_DATA_LENGTH];
    uint16_t crc;
} data_packet_t;


uint16_t calculateCRC16(uint8_t *data, uint8_t length) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return crc;
}


int isPacketCorrupted(data_packet_t *packet) {
    uint16_t calculatedCRC = calculateCRC16(packet->data, packet->data_length);
    return calculatedCRC != packet->crc;
}

int main() {
    data_packet_t packet;
    packet.id = 1;
    packet.data_length = 10;
    for (int i = 0; i < packet.data_length; i++) {
        packet.data[i] = i + 1;
    }
    packet.crc = calculateCRC16(packet.data, packet.data_length);
    
    
    packet.data[3] = 0xFF;
    
    
    if (isPacketCorrupted(&packet)) {
        printf("The packet is corrupted.\n");
    } else {
        printf("The packet is not corrupted.\n");
    }
    
    return 0;
}