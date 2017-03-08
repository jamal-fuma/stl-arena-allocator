#include "hexdump.h"

#if defined(__cplusplus)
extern "C" {
#endif

void hexdump(FILE * fp, uint8_t const * buf, size_t len, size_t width)
{
    size_t i=0;
    printf("\n");
    for(i=0;  i<len; i += width)
    {
        // len < width
        if(!((len - i) > len) && ((len - (i + width)) > (len-i)))
        {
            width = len;
        }
        fprintf(fp,"%p %6zu: ", &buf[i], i);
        fprint_hex_column(fp,width,width,buf+i);          // 5 * len-i + 3 * (width-len-i)
        fprintf(fp,"| ");                                 // 2
        fprint_ascii_column(fp,width,width,buf+i);        // 1 * width
        fprintf(fp,"\n");                                 // 1
    }
}

#if defined(__cplusplus)
}
#endif
