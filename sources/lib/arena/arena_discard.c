#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <string.h>

#include "arena.h"
#if defined(__cplusplus)
extern "C" {
#endif

void arena_discard(fuma_arena_t * arena)
{
    arena->avail = arena->total;
    arena->avail -= arena->slop;
    arena->reclaim = 0;
 //   memset(arena->memory,0,arena->total);
}

#if defined(__cplusplus)
}
#endif
