#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <string.h>

#include "arena.h"

#if defined(__cplusplus)
extern "C" {
#endif

void arena_init(fuma_arena_t * arena, void * memory, size_t len)
{
    if(arena)
    {
        arena->memory  = memory;
        arena->total   = len;
        arena->avail   = (len - (len & eAlignmentOfAllocatedMemory-1));
        arena->reclaim = 0;
        arena->slop    = len - arena->avail;
        arena->nallocs = 0;
        arena->nfrees  = 0;
        if(arena->memory)
        {
 //           memset(arena->memory,0,len);
        }
    }
}

#if defined(__cplusplus)
}
#endif
