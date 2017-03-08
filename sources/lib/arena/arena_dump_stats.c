#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "arena.h"
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

void
arena_dump_stats(fuma_arena_t * arena,FILE *fp)
{
    fprintf(fp,"%p: ", arena);
    fprintf(fp,"%12s|%12s|%12s|%12s|", "Total", "Avail", "Used","Reclaim");
    fprintf(fp,"%12s|%12s|%12s\n","Allocs","Frees","Errors");
    fprintf(fp,"%p: ", arena);
    fprintf(fp,"%12zu|%12zu|%12zu|%12zu", arena->total,arena->avail,
            (arena->total - (arena->avail + arena->slop)),
            arena->reclaim);
    fprintf(fp,"%12zu|%12zu|%12zu\n",(size_t)arena->nallocs,(size_t)arena->nfrees,(size_t)arena->nerrors);
}

#if defined(__cplusplus)
}
#endif
