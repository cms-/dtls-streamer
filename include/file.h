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

#define HTTP_HEAD \
	"HTTP/1.0 200 OK\r\n" \
	"Access-Control-Allow-Origin: *\r\n" \
	STD_HEADER \
	"Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n" \
	"\r\n" \
	"--" BOUNDARY "\r\n"

#define STD_HEADER "Connection: close\r\n" \
    "Server: MJPG-Streamer/0.2\r\n" \
    "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
    "Pragma: no-cache\r\n" \
    "Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n"

#define HTTP_STITCH \
	"Content-Type: image/jpeg\r\n" \
	"Content-Length: %u\r\n" \
	"X-Timestamp: %d.%06d\r\n" \
    "\r\n"

#define HTTP_BOUNDARY	"boundarydonotcross\r\n"

// ******* file_read *******
// Initializes a FIFO structure, preparing it for usage.
// Inputs: a path string and an unallocated file buffer.
// Ouputs: length of file loaded or negative error code.
int32_t file_read( const char *f_path, uint8_t **file_buf );

// ******* stream_create *******
// Assembles a continuous stream of JPEG frames connected with HTTP
// interstitial boundary information.
// Inputs: a file buffer loaded with JPEG data; length of the JPEG data;
//         and an initialized fifo_p pointer.
// Ouputs: non-zero on error.
uint32_t stream_create( void *file_buf, uint32_t f_len, fifo_p f );

#endif /* file.h */