/*
    Component: DATA_PARSER
    Author: Rafael J. Scholtz
    Date: 23/08/2022
    Contents: Functions to serialize data from structs into a buffer to be
    transmitted by sockets. This is an app-level library.
*/

#ifndef HEADER_DP
#define HEADER_DP

// --- Includes --- //
#include "lwip/sockets.h"
#include "esp_system.h"
#include "esp_log.h"

// --- Defines --- //

// Each buffer has an ID to tell variable configurations
// e.g.: type of sample and bit size. Max ID size is 1 byte
#define DP_MAX_BUFFER_ID_SIZE 1

// Max transmit buffer size
// the actual size is computed separately, but cannot excede this
#define DP_MAX_BUFFER_SIZE 6000

// Min transmit buffer size
// required for good tx speed
#define DP_MIN_BUFFER_SIZE 2500

// Can be altered for tests
// 200 implies that, for 7 bytes per time/sample -> ~1400 bytes payload
#define DP_MAX_SAMPLE_BUFFER_SIZE 720

// Max number of logged variables (signals)
// May be tested to get max number
// for now 10 is reasonable
#define DP_MAX_SAMPLE_VARIABLE_NUMBER 10

// See SPI impact of defines below
#define NUM_OF_BUFFERS DP_MAX_SAMPLE_VARIABLE_NUMBER
// #define DATA_LIM 200

// --- DATA TYPES --- //

// Message type enum - holds up to 1 byte (not as right now though)
typedef enum
{
    // 8 bits samples - starts at 0
    NOT_USED = 0,
    SAMPLE_8_BITS = 1,
    SAMPLE_16_BITS = 2
}DP_BUFFER_ID_TYPE;

// 8 bit sample buffer
typedef struct
{
    /* data */
    unsigned char BUFFER_ID;
    uint8_t SAMPLE_BUFFER[DP_MAX_SAMPLE_BUFFER_SIZE];
}DP_SAMPLE_BUFFER_8_BITS_TYPE;

// 16 bit sample buffer
typedef struct
{
    unsigned char BUFFER_ID; // ID meant for tracking variable

    // 16 bit array as buffer
    // it's treatment is made based in BUFFER_ID_TABLE
    uint16_t SAMPLE_BUFFER[DP_MAX_SAMPLE_BUFFER_SIZE];

    unsigned int buffer_ix; // buffer_ix for SPI 
}DP_SAMPLE_BUFFER_TYPE;

// --- External variables --- //
extern unsigned char * DATA_PAYLOAD_BUFFER_ptr;
extern unsigned char DATA_PAYLOAD_BUFFER1[DP_MAX_BUFFER_SIZE];
extern unsigned char DATA_PAYLOAD_BUFFER2[DP_MAX_BUFFER_SIZE];
//extern DP_SAMPLE_BUFFER_TYPE * DP_buffer_ptrs[NUM_OF_BUFFERS];
//extern DP_SAMPLE_BUFFER_TYPE BUFFER1;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER2;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER3;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER4;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER5;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER6;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER7;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER8;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER9;
//extern DP_SAMPLE_BUFFER_TYPE BUFFER10;
extern unsigned char DP_BUFFER_ID_TABLE[NUM_OF_BUFFERS];
extern unsigned int DATA_PAYLOAD_BUFFER_SIZE;
extern unsigned int DP_current_buffer1_size;
extern unsigned int DP_current_buffer2_size;



// --- Public functions --- //

// Serialize functions
unsigned char * DP_serialize_8_bit_buffer(unsigned char *buffer_ptr, DP_SAMPLE_BUFFER_TYPE * buffer);
unsigned char * DP_serialize_16_bit_buffer(unsigned char *buffer_ptr, DP_SAMPLE_BUFFER_TYPE * buffer);
//void DP_serialize_all(void);
unsigned int DP_get_max_buffer_size(void);
unsigned int IRAM_ATTR DP_write_to_buffer(void *arg, unsigned char size);


// ============ TEST FUNCTION ============== //
void DP_initialize(void);


// --- Ignore record --- //

//void test();

/*
// 16 bit sample buffer with label
typedef struct
{
    char VARIABLE_LABEL[DP_MAX_VARIABLE_CHAR_NAME_SIZE];
    unsigned char SAMPLE_BYTE_NUMBER;
    uint16_t SAMPLE_BUFFER[DP_MAX_SAMPLE_BUFFER_SIZE];
}DP_SAMPLE_BUFFER_16_BITS_TYPE;
*/

#endif