#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "arena.h"
#include <errno.h>

#if defined(__cplusplus)
extern "C" {
#endif

void *
arena_alloc_bytes(fuma_arena_t * arena, size_t bytes)
{
    size_t kAlign       = eAlignmentOfAllocatedMemory-1;
    size_t kMask        = ~(kAlign);

    // align down if unaligned
    size_t min_align    = kMask & (bytes);
    size_t max_align    = kMask & (bytes + kAlign);

    if(min_align != bytes)
        bytes = max_align;

    // crap out if we run out of memory
    void *ptr = NULL;
    if(0 == ((arena->avail - bytes) > arena->avail))
    {
        // counter
        ++arena->nallocs;

        // allocation
        arena->avail -= bytes;

        // user visible memory
        ptr = (void *)(arena->memory+arena->avail);
    }
    else
    {
        arena->nerrors++;
    }
    return ptr;
}

#if defined(__cplusplus)
}
#endif
