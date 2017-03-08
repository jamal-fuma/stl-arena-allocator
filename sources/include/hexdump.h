#ifndef LIB_FUMA_HEXDUMP_H
#define LIB_FUMA_HEXDUMP_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

void hexdump(FILE * fp, const uint8_t * buf, size_t len, size_t width);
void fprint_hex_column(FILE * fp,int width, int wrap_at, const uint8_t * buf);
void fprint_ascii_column(FILE * fp,int width, int wrap_at, const uint8_t * buf);

#if defined(__cplusplus)
}
#endif

#endif /* LIB_FUMA_HEXDUMP_H */
