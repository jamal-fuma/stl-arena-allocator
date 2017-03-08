#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "arena.h"

#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

char *
arena_strdup(fuma_arena_t * arena, const char * buf)
{
    return (!buf || !arena)
        ?  NULL
        : arena_strndup(arena,buf,strlen(buf));
}

char *
arena_strndup(fuma_arena_t * arena, const char * buf, size_t len)
{
    char * ptr = (!buf || !arena)
        ?  NULL
        : arena_alloc_bytes(arena,len+1);

    if(ptr)
    {
        memcpy(ptr,buf,len);
        ptr[len] = 0;
    }
    return ptr;
}

#if defined(__cplusplus)
}
#endif
