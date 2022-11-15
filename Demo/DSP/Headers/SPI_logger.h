/*
 * SPI.h
 *
 *  Created on: 25 de mai de 2022
 *      Author: rafae
 */

#include "F28x_Project.h"

#ifndef HEADERS_SPI_H_
#define HEADERS_SPI_H_

// --- Defines --- //
#define SPI_MAX_SAMPLE_NR 10
#define TRUE 1
#define FALSE 0
#define SPI_BATCH_NR 15
#define SPI_RX_INT_NR 15
#define SPI_CS_PIN 61
//#define SPI_EN_PIN 61
#define SPI_MAX_QUEUE_SIZE 150

// --- Types --- //

typedef enum
{
    SPI_MODULE_FAILURE = 0,
    SPI_MODULE_OK
} SPI_STATUS;

typedef enum
{
    SPI_DATA = 10,
    SPI_PROTOCOL = 200
} SPI_HEADER;

typedef enum
{
    SPI_ESP_READY = 100,
    SPI_ESP_OK = 200,
    SPI_ESP_FAILURE = 300
} SPI_ESP_RESPONSE;


// --- Globals --- //
extern bool SPI_transaction_occuring;
extern unsigned char SPI_FIFO_TX_nr;
extern unsigned char SPI_FIFO_RX_nr;
extern SPI_STATUS SPI_global_status;


/*
 * Basic SPI idea
 * SPI can either work in FIFO mode (first in first out) or independently
 * If set in FIFO, by writing 1 word to SPITXBUF, it is automatically pushed to the FIFO Tx buffer
 * The FIFO Tx buffer can hold up to 16 words (1 word = 16 bits = 2 bytes)
 *
 * If set in non FIFO, you need to complete a transmission to rewrite in SPITXBUF and begin another transmission
 */

// --- Public Functions --- //
/*  SPI functions */
void SPI_INIT(void);
SPI_STATUS SPI_TX(Uint16 data); // Sends 16 bits data
Uint16 SPI_RX(void); // Reads 16 bits data
void SPI_register_sample(unsigned char sample_bit_nr);
SPI_STATUS SPI_comm_setup(void);

/* Queue logic functions */
int SPI_TX_queue_isFull();
int SPI_TX_queue_isEmpty();
void SPI_TX_enQueue(Uint16 element);
Uint16 SPI_TX_deQueue();





#endif /* HEADERS_SPI_H_ */
