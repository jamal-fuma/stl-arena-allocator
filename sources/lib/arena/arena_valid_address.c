#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "arena.h"

#if defined(__cplusplus)
extern "C" {
#endif

int
arena_valid_address(fuma_arena_t *arena, void *ptr)
{
    char *where = ptr;
    if(!arena->memory || !where)
        return 0;

    // this memory must start before our region ends
    return (where < (arena->memory + arena->total)) ?
        // this memory may alias the entire region
        (where >= arena->memory) : 0;
}

#if defined(__cplusplus)
}
#endif
