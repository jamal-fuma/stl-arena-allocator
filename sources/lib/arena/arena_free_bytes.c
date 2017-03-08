#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <string.h>
#include <errno.h>

#include "arena.h"

#if defined(__cplusplus)
extern "C" {
#endif

void arena_free_bytes(fuma_arena_t *arena, void * ptr, size_t len)
{
    errno = 0;
    if(arena_valid_address(arena,ptr))
    {
        // we only deal with aligned sizes, but a user has no way of seeing this
        size_t kAlign       = eAlignmentOfAllocatedMemory-1;
        size_t kMask        = ~(kAlign);

        // align down if unaligned
        size_t min_align    = kMask & (len);
        size_t max_align    = kMask & (len + kAlign);

        if(min_align != len)
            len = max_align;

//        memset(ptr,0,len);
        if((arena->memory+arena->avail) == ptr)
        {
            // reclaim if it was the last allocation
            --arena->nallocs;
            arena->avail += len;
        }
        else
        {
            // reclaim it later
            arena->nfrees++;
            arena->reclaim += len;
        }
        return;
   }
   errno = EINVAL;
}

#if defined(__cplusplus)
}
#endif
