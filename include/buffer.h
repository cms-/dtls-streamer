/**
 * \buffer.h
 *
 * \buffer suite to test UDP streaming via a DTLS server
 *
 *  Released under MIT license by Chris Slothouber sol at hax space 
 *
 *  This file is part of the output_dtls mjpg-streamer plugin 
 *  (http://github.com/cms-/)
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFFERSIZE 250000

typedef struct fifo_t *fifo_p;

// ******* fifo_init *******
// Initializes a FIFO structure, preparing it for usage.
// Inputs: none.
// Ouputs: an initialized fifo_p pointer.
fifo_p fifo_init( void );

// ******* fifo_put *******
// Appends a length of data to a specified fifo.
// Inputs: pointer to input buffer, a fifo_p pointer, number of bytes to insert.
// Outputs: number of bytes inserted successfully from the in_buf.
uint32_t fifo_put( void *in_buf, fifo_p f, uint32_t in_bytes );

// ******* fifo_get *******
// Retrieves and removes data from a specified buffer.
// Inputs: point to an output buffer, a fifo_p pointer, number of bytes to read.
// Outputs: number of bytes read into the out_buf.
uint32_t fifo_get( void *out_buf, fifo_p f, uint32_t out_bytes );

// ******* fifo_destroy *******
// Releases memory assigned to a FIFO structure, freeing it for further use.
// Inputs: an active fifo_p pointer.
// Ouputs: none.
void fifo_destroy( fifo_p f );

// ******* fifo_stat *******
// Returns the number of elements in the buffer.
// Inputs: an active fifo_p pointer.
// Ouputs: zero on empty, otherwise number of elements in buffer
uint32_t fifo_stat( fifo_p f );

#endif /* buffer.h */