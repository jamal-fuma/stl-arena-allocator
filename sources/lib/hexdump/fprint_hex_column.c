#include <ctype.h>
#include "hexdump.h"

#if defined(__cplusplus)
extern "C" {
#endif

void
fprint_hex_column(FILE * fp,int width, int wrap_at, const uint8_t * buf)
{
    /* print the hex version */
    int i;
    for(i=0 ; i<width; i++)
    {
        fprintf(fp,"%2.2x ", (unsigned char) buf[i]);
    }
    /* pad the remaining bytes so the lines match neatly*/
    for(i = width ; i < wrap_at ; i++)
    {
        fprintf(fp,"%c%c ", '.', '.');
    }
}

#if defined(__cplusplus)
}
#endif
