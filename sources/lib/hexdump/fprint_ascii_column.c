#include <ctype.h>

#include "hexdump.h"

#if defined(__cplusplus)
extern "C" {
#endif

void
fprint_ascii_column(FILE * fp,int width, int wrap_at, const uint8_t * buf)
{
    /* print the ascii version escaping non-printables*/
    int i;
    for(i=0 ; i<width ; i++)
    {
        fprintf(fp,"%c", (isgraph(buf[i]) ? buf[i] : '.'));
    }
    /* pad the remaining bytes so the lines match neatly*/
    for(i=width ; i<wrap_at; i++)
    {
        fprintf(fp,"%c", '.');
    }
}

#if defined(__cplusplus)
}
#endif
