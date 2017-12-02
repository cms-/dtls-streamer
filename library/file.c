#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

// ******* file_read *******
// Reads binary data from a fully qualified file path.
// Inputs: a file path string and an unallocated file buffer.
// Outputs: length of file loaded or negative error code.
int32_t file_read( const char *f_path, uint8_t **file_buf )
{
    int32_t f_size;
    FILE *f_handle;

    if( ( f_handle = fopen( f_path, "rb" ) ) == NULL )
        return( -1 );

    fseek( f_handle, 0, SEEK_END );

    if( ( f_size = ftell( f_handle ) ) == -1 )
    {
        fclose( f_handle );
        return( -2 );
    }
    fseek( f_handle, 0, SEEK_SET );

    if ( ( *file_buf = calloc ( f_size + 1, sizeof(uint8_t) ) ) == NULL )
    {
        fclose( f_handle );
        return( -3 );
    }
    
    if ( fread( *file_buf, sizeof(uint8_t), f_size, f_handle ) != (uint32_t) f_size )
    {
        fclose( f_handle );
        free( file_buf );
        return( -4 );
    }

    fclose( f_handle );
    return( f_size );
}

// ******* fifo_free *******
// Releases memory assigned to a file buffer.
// Inputs: a pointer to a file buffer pointer
// Outputs: none
void file_free( uint8_t **file_buf )
{
    free( *file_buf );
}

// ******* stream_create *******
// Reads a JPEG from file into a FIFO buffer in order to simulate a 
// video capture device.
// Inputs: a file buffer loaded with JPEG data, length of the JPEG data,
//         and an initialized fifo_p pointer.
// Outputs: non-zero on error.
uint32_t frame_create( void *file_buf, uint32_t file_len, fifo_p f )
{
    uint32_t ret = 0;
    // uint32_t len = 0;
    // uint8_t buf[BUFFERSIZE] = {0};
    // uint8_t eof[4] = "\r\n";
    struct timeval timestamp;
    gettimeofday ( &timestamp, NULL );

    // len = sprintf( (char *) buf, HTTP_STITCH, (int) file_len,
    //             (int) timestamp.tv_sec, (int) timestamp.tv_usec );

    // ret = fifo_put( buf, f, len );
    // if (ret != len)
    // {
    //     return ( 1 );
    // }
    // printf( "\nstream_create ret; put func_buf: %u\n", ret );

    ret = fifo_put( file_buf, f, file_len );
    if ( ret != file_len )
    {
        return ( 2 );
    }
    //printf( "\nstream_create ret; put file_buf: %u\n", ret );

    // ret = fifo_put( eof, f, 2 );
    // if ( ret != 2 )
    // {
    //     return ( 3 );
    // }
    // printf( "\nstream_create ret; put eof: %u\n", ret );

    return ( 0 );
}

// ******* packet_create *******
// Assembles & writes a serialized packet using supplied fifo and per-packet metadata.
// Inputs: an empty & allocated buffer, a protobuf-c packet, an initialized fifo_p 
// pointer, the desired packet payload length, a uuid for the current frame/file, the 
// packet sequence of the current frame/file, and the number of packets per frame/file.
// Outputs: Length of loaded *packet_buf, < 0 on error.
int32_t packet_create( void *p_buf, Packet *p, fifo_p f, uint32_t payload_len,
                        uint64_t uuid, uint32_t seq, uint32_t len )
{
    uint32_t crc = 0;
    uint32_t fifo_ret;
    uint32_t p_buf_ret;
    uint8_t *buf;
    buf = (uint8_t *) calloc( payload_len, sizeof(uint8_t) );
    
    fifo_ret = fifo_get( buf, f, payload_len );
    //crc = crc32( buf, fifo_ret );
    p->payload.len = fifo_ret;
    p->payload.data = &buf[0];

    p->uuid = uuid;
    p->len = len;
    p->seq = seq;
    p->crc = crc;

    p_buf_ret = protobuf_c_message_pack( (const struct ProtobufCMessage *) p, p_buf );

    free ( buf );
    return ( p_buf_ret );

}
