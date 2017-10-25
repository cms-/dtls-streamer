/**
 * \file.h
 *
 * \basic file access and manipulation to support UDP streaming
 *  with a DTLS server.
 *  Released under MIT license by Chris Slothouber sol at hax space 
 *
 */
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "buffer.h"

#define TEST_FILE       "test33.jpg"

// ******* file_read *******
// Initializes a FIFO structure, preparing it for usage.
// Inputs: a path string and an unallocated file buffer
// Ouputs: length of file loaded
uint32_t file_read( const char *f_path, uint8_t **file_buf );

// ******* stream_create *******
// Stitches together a continues stream of JPEG frames connected with
// the appropriate intersitial boundary information.
// Inputs: a file buffer loaded with JPEG data; an initialized fifo_p pointer
// Ouputs: non-zero on error
uint32_t stream_create( void *file_buf, fifo_p f );

#endif /* file.h */