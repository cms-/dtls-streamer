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

#define TEST_FILE       "test"
#define TEST_EXT		".jpg"
#define NUM_FRAMES		9

#define HTTP_HEAD \
	"HTTP/1.0 200 OK\r\n" \
	"Access-Control-Allow-Origin: *\r\n" \
	STD_HEADER \
	"Content-Type: multipart/x-mixed-replace;boundary=" \
	BOUNDARY \
	"\r\n\r\n"

#define STD_HEADER "Connection: close\r\n" \
    "Server: DTLS-streamer/0.1\r\n" \
    "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
    "Pragma: no-cache\r\n" \
    "Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n"

#define HTTP_STITCH \
    "--" \
    HTTP_BOUNDARY \
	"Content-Type: image/jpeg\r\n" \
	"Content-Length: %u\r\n" \
	"X-Timestamp: %d.%06d\r\n" \
    "\r\n"

#define HTTP_BOUNDARY	"boundarydonotcross\r\n"

// ******* file_read *******
// Reads binary data from a fully qualified file path.
// Inputs: a file path string and an unallocated file buffer.
// Outputs: length of file loaded or negative error code.
int32_t file_read( const char *f_path, uint8_t **file_buf );

// ******* fifo_free *******
// Releases memory assigned to a file buffer.
// Inputs: a pointer to a file buffer pointer
// Outputs: none
void file_free( uint8_t **file_buf );

// ******* stream_create *******
// Reads a JPEG from file into a FIFO buffer in order to simulate a 
// video capture device.
// Inputs: a file buffer loaded with JPEG data, length of the JPEG data,
//         and an initialized fifo_p pointer.
// Outputs: non-zero on error.
uint32_t frame_create( void *file_buf, uint32_t file_len, fifo_p f );

// ******* packet_create *******
// Assembles a serialized packet using supplied fifo and packet length.
// Inputs: an empty initialized packet buffer, an initialized fifo_p 
// pointer, and the number of bytes to load from the fifo.
// Outputs: Length of loaded *packet_buf, < 0 on error.
int32_t packet_create( void *p_buf, fifo_p f, uint32_t payload_len );

#endif /* file.h */