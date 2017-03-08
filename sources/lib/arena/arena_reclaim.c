#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "arena.h"

#if defined(__cplusplus)
extern "C" {
#endif

void arena_reclaim(fuma_arena_t * arena)
{
    arena->avail += arena->reclaim;
    arena->reclaim = 0;
}

#if defined(__cplusplus)
}
#endif
