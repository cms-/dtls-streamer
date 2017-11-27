
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct fifo_t {
    uint8_t buffer[BUFFERSIZE+1]; // number of elements in buffer
    uint32_t putPt;
    uint32_t getPt;
} fifo_t;

// ******* fifo_init *******
// Initializes a FIFO structure, preparing it for usage.
// Inputs: none
// Ouputs: an initialized fifo_p pointer
fifo_p fifo_init( void )
{
    fifo_p f;

    f = ( fifo_p ) calloc( 1, sizeof(fifo_t) + 4 );

    if ( f == NULL )
    {
        printf( "  ! buffer memory allocation failed\n" );
        exit( 1 );
    }

    f->putPt = 0;
    f->getPt = 0;
    return( f );
}

// ******* fifo_put *******
// Appends a length of data to a specified fifo.
// Inputs: pointer to input buffer, a fifo_p pointer, number of bytes to insert.
// Outputs: number of bytes inserted successfully from the in_buf.
uint32_t fifo_put( void *in_buf, fifo_p f, uint32_t in_bytes )
{
    uint32_t bytes;
    char *in;
    in = in_buf;

    for ( bytes=0; bytes < in_bytes; bytes++ )
    {
        // Check to see if there's space in the buffer
        // Full condition: counter reached BUFFERSIZE
        if ( f->putPt == BUFFERSIZE )
        {
            return( bytes ); // no room left, return number of bytes inserted
        }
        f->buffer[f->putPt] = (uint8_t)*in++;
        f->putPt++; // advance to next buffer byte
        
    }
    return( bytes ); // return number of bytes added to the buffer
}

// ******* fifo_get *******
// Retrieves and removes data from a specified buffer.
// Inputs: point to an output buffer, a fifo_p pointer, number of bytes to read.
// Outputs: number of bytes read into the out_buf.
uint32_t fifo_get( void *out_buf, fifo_p f, uint32_t out_bytes )
{
    uint32_t bytes;
    char *out;
    out = out_buf;

    for ( bytes=0; bytes < out_bytes; bytes++ ) {
        // Ensure we're not at the empty condition: f->putPt == f->getPt
        if ( f->putPt == f->getPt )
        {
            return( bytes );
        }
        else
        {
            
            *out++ = f->buffer[f->getPt];
            f->getPt++;
        }

    }
    return( bytes );
}

// ******* fifo_destroy *******
// Releases memory assigned to a FIFO structure, freeing it for further use.
// Inputs: an active fifo_p pointer
// Ouputs: none
void fifo_destroy( fifo_p f )
{
    free( f );
    printf( "\n  . Freeing fifo buffer...\n" );
}

// ******* fifo_stat *******
// Returns the number of elements in the buffer.
// Inputs: an active fifo_p pointer.
// Ouputs: zero on empty, otherwise number of elements in buffer
uint32_t fifo_stat( fifo_p f )
{
    return f->putPt;
}