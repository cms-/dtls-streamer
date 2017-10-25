#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

// ******* file_read *******
// Initializes a FIFO structure, preparing it for usage.
// Inputs: a path string and an unallocated file buffer.
// Ouputs: length of file loaded or negative error code.
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
        free( *file_buf );
        return( -4 );
    }

    fclose( f_handle );
    return f_size;
}

// ******* stream_create *******
// Assembles a continuous stream of JPEG frames connected with HTTP
// interstitial boundary information.
// Inputs: a file buffer loaded with JPEG data; length of the JPEG data;
//         and an initialized fifo_p pointer.
// Ouputs: non-zero on error.
uint32_t stream_create( void *file_buf, uint32_t f_len, fifo_p f )
{
    uint32_t ret = 0;
    uint32_t len = 0;
    uint8_t buf[BUFFERSIZE] = {0};
    struct timeval timestamp;
    gettimeofday ( &timestamp, NULL );

    // if fifo_p is empty:
    //     len += sprintf(the header)


    // len += sprintf( print mimetype )
    // (int) timestamp.tv_sec, (int) timestamp.tv_usec);

    // fifo_put buf, len

    // fifo_put file_buf, len

    return ret;

}