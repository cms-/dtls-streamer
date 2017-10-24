#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t file_read( const char *f_path, uint8_t **file_buf )
{
    uint32_t f_size;
    FILE *f_handle;

    if( ( f_handle = fopen( f_path, "rb" ) ) == NULL )
        return( 0 );

    fseek( f_handle, 0, SEEK_END );

    if( ( f_size = ftell( f_handle ) ) == -1 )
    {
        fclose( f_handle );
        return( 0 );
    }
    fseek( f_handle, 0, SEEK_SET );

    if ( ( *file_buf = calloc ( f_size + 1, sizeof(uint8_t) ) ) == NULL )
    {
        fclose( f_handle );
        return( 0 );
    }
    printf("\nFILE SIZE: %u\n\n", f_size);
    if ( fread( *file_buf, sizeof(uint8_t), f_size, f_handle ) != f_size )
    {
        fclose( f_handle );
        free( *file_buf );
        return( 0 );
    }

    fclose( f_handle );
    return f_size;
}

// uint32_t stream_create( void *file_buf, fifo_p f )
// {

// }