/*
    Component: DATA_PARSER
    Author: Rafael J. Scholtz
    Date: 15/08/2022
    Contents: Functions to serialize data from structs into a buffer to be
    transmitted by sockets. This is an app-level library.
*/

#include <stdio.h>
#include "DATA_PARSER.h"
#include "esp_attr.h"

// --- Global variables --- //
// Payload of fixed size aranged in 1-byte chars
unsigned char DATA_PAYLOAD_BUFFER1[DP_MAX_BUFFER_SIZE]; // Buffer to be updated by SPI
unsigned char DATA_PAYLOAD_BUFFER2[DP_MAX_BUFFER_SIZE]; // Buffer to be copied when buffer 1 is full
unsigned char * DATA_PAYLOAD_BUFFER_ptr = &DATA_PAYLOAD_BUFFER1[0];
//unsigned int DATA_PAYLOAD_BUFFER_SIZE = DP_MIN_BUFFER_SIZE;
unsigned int DP_current_buffer1_size = 0;
unsigned int DP_current_buffer2_size = 0;


// Buffer ID table
unsigned char DP_BUFFER_ID_TABLE[NUM_OF_BUFFERS];

/*
// Buffer pointers
DP_SAMPLE_BUFFER_TYPE * DP_buffer_ptrs[NUM_OF_BUFFERS] = {
    &BUFFER1,
    &BUFFER2,
    &BUFFER3,
    &BUFFER4,
    &BUFFER5,
    &BUFFER6,
    &BUFFER7,
    &BUFFER8,
    &BUFFER9,
    &BUFFER10
};
*/



// ======= START OF TEST CODE ======= //

//static const char *payload = "Message from ESP32 ";
// Dummy payloads
//int8_t payload_8_bits[DP_MAX_SAMPLE_BUFFER_SIZE];
//int16_t payload_16_bits[DP_MAX_SAMPLE_BUFFER_SIZE];




// Test function to initialize arrays
void DP_initialize(void)
{
    /*
    // Dummy arrays
    //  -> 1 word as [0, 1, 2, ..., DP_MAX_SAMPLE_BUFFER_SIZE-1]
    for(int i = 0; i<DP_MAX_SAMPLE_BUFFER_SIZE; i++)
        payload_8_bits[i] = i;

    //  -> 2 word as [DP_MAX_SAMPLE_BUFFER_SIZE, ..., 3, 2, 1]
    for(int i = 0; i<DP_MAX_SAMPLE_BUFFER_SIZE; i++)
        payload_16_bits[i] = DP_MAX_SAMPLE_BUFFER_SIZE - i;
    */
    
    // Initializes id table
    //DP_BUFFER_ID_TABLE[0] = SAMPLE_16_BITS;
    //DP_BUFFER_ID_TABLE[1] = SAMPLE_16_BITS;
    //DP_BUFFER_ID_TABLE[2] = SAMPLE_16_BITS;
    //DP_BUFFER_ID_TABLE[3] = SAMPLE_16_BITS;

    // Writes correct value to DATA_PAYLOAD_BUFFER_SIZE
    //DATA_PAYLOAD_BUFFER_SIZE = DP_get_max_buffer_size();
}


// ======= END OF TEST CODE ======= //






// --- Public functions --- //

// Function: DP_SERIALIZE_BUFFER
// Input:
// Output: 
// Description: Serializes struct of type DP_SAMPLE_BUFFER_8_BITS_TYPE in buffer pointed by buffer_ptr
unsigned char * DP_serialize_8_bit_buffer(unsigned char *buffer_ptr, DP_SAMPLE_BUFFER_TYPE * buffer)
{
    // Sends buffer ID
    buffer_ptr[0] = buffer->BUFFER_ID;

    // Shifts pointer
    buffer_ptr += 1;

    // Simply sends each array member
    for (int i = 0; i < DP_MAX_SAMPLE_BUFFER_SIZE; i++)
    {
        buffer_ptr[0] = buffer->SAMPLE_BUFFER[i];
        buffer_ptr += 1;
    }

    
    // Returns pointer
    return buffer_ptr;
}


// Function: DP_SERIALIZE_16_BIT_BUFFER
// Input:
// Output: 
// Description: Serializes struct of type DP_SAMPLE_BUFFER_16_BITS_TYPE in buffer pointed by buffer_ptr
unsigned char * IRAM_ATTR DP_serialize_16_bit_buffer(unsigned char *buffer_ptr, DP_SAMPLE_BUFFER_TYPE * buffer)
{

    // Sends buffer ID
    buffer_ptr[0] = buffer->BUFFER_ID;

    // Shifts pointer
    buffer_ptr += 1;

    for (int i = 0; i < DP_MAX_SAMPLE_BUFFER_SIZE; i++)
    {
        // Sends 8 most significative bits first
        buffer_ptr[0] = buffer->SAMPLE_BUFFER[i] >> 8;
        buffer_ptr +=1; // Shifts pointer
        // Sends 8 least significative bits last
        buffer_ptr[0] = buffer->SAMPLE_BUFFER[i];
        buffer_ptr +=1; // Shifts pointer again
    }
    

    // Returns pointer
    return buffer_ptr;
}

// Function: DP_write_to_buffer
// Input: addres to chunk of data to be written and size of data in bytes
// Output: number of used bytes in wifi tx buffer
// Description: Serializes data and writes it to the 8 bit global buffer
unsigned int IRAM_ATTR DP_write_to_buffer(void *arg, unsigned char size)
{
    // If current buffer size is 0, reset the pointer to start
    if(DP_current_buffer1_size == 0) DATA_PAYLOAD_BUFFER_ptr = &DATA_PAYLOAD_BUFFER1[0];

    // Writes serialized data to buffer
    //unsigned char byte_array[size];
    memcpy(DATA_PAYLOAD_BUFFER_ptr, arg, size);
    DATA_PAYLOAD_BUFFER_ptr+=size;
    DP_current_buffer1_size += size;
    /*
    for(unsigned char i = 0; i<size; i++){
        DATA_PAYLOAD_BUFFER_ptr[i] = byte_array[i];
    }
    */
    

    return DP_current_buffer1_size;
}

// Function: DP_serialize_all
// Input: void
// Output: void
// Description: Serializes data of all used buffers in this module
/*
void IRAM_ATTR DP_serialize_all(void)
{
    // Returns buffer to the beginning
    DATA_PAYLOAD_BUFFER_ptr = &DATA_PAYLOAD_BUFFER1[0];
    
    // Checks each used buffer and serializes it accordingly
    unsigned char i = 0;
    for(i=0; i<DP_MAX_SAMPLE_VARIABLE_NUMBER; i++)
    {
        switch (DP_BUFFER_ID_TABLE[i])
        {
        // Just breaks
        case NOT_USED:
            break;
        // 8 bit buffer
        case SAMPLE_8_BITS:
            DATA_PAYLOAD_BUFFER_ptr = DP_serialize_8_bit_buffer(DATA_PAYLOAD_BUFFER_ptr, DP_buffer_ptrs[i]);
            break;
        // 16 bit buffer
        case SAMPLE_16_BITS:
            DATA_PAYLOAD_BUFFER_ptr = DP_serialize_16_bit_buffer(DATA_PAYLOAD_BUFFER_ptr, DP_buffer_ptrs[i]);
            break;
        // Shouldn't reach code below
        default:
            break;
        }
    }
}
*/

// Function: DP_get_buffer_size
// Input: void
// Output: max number of bytes in the wifi transmit buffer 
// Description: Computes the number of bytes in the transmit buffer based on
// the variable size and number of buffers
unsigned int DP_get_max_buffer_size(void)
{
    unsigned char i = 0;
    unsigned int buffer_size = 0;
    for(i = 0; i<DP_MAX_SAMPLE_VARIABLE_NUMBER; i++){
        buffer_size += DP_BUFFER_ID_TABLE[i] * (DP_MAX_SAMPLE_BUFFER_SIZE);
        //if(DP_BUFFER_ID_TABLE[i] != NOT_USED)   buffer_size++;
    }
    return buffer_size;
}


bool DP_buffer_is_full(unsigned int buffer_byte_nr)
{
    return buffer_byte_nr >= DP_MIN_BUFFER_SIZE;
}

// --- Ignore record --- //

/*
void test(void)
{
    printf("component test\n");
}
*/

/*



*/