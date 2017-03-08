#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <string.h>

#include "arena.h"

#if defined(__cplusplus)
extern "C" {
#endif

void arena_finit(fuma_arena_t * arena)
{
    if(arena && arena->memory)
    {
 //       memset(arena->memory,0,arena->total);
//        memset(arena,0,sizeof(*arena));
    }
    if(arena)
    {
  //      memset(arena,0,sizeof(*arena));
    }
}

#if defined(__cplusplus)
}
#endif
