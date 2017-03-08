#ifndef LIB_FUMA_ARENA_H
#define LIB_FUMA_ARENA_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

enum { eAlignmentOfAllocatedMemory = 8 };

// maintains counters relating to memory usage
struct fuma_arena_tag
{
    char * memory;
    size_t avail;
    size_t reclaim;
    size_t total;
    size_t slop;
    uint32_t nallocs;
    uint32_t nfrees;
    uint32_t nerrors;
};

// a simple mapping back to the original to allow freeing chunks
struct fuma_arena_allocation_tag
{
    void * parent;
    size_t size;
};

typedef struct fuma_arena_tag            fuma_arena_t;
typedef struct fuma_arena_allocation_tag fuma_arena_allocation_t;

void arena_init(fuma_arena_t * arena, void * memory, size_t len);
void arena_finit(fuma_arena_t * arena);
void *arena_alloc_bytes(fuma_arena_t * arena, size_t bytes);

// relies on an heap embedded pointer
// void arena_free_bytes(void * ptr);

// relies on user doing bookeeping at higher level
void arena_free_bytes(fuma_arena_t *arena, void * ptr, size_t len);


char *arena_strndup(fuma_arena_t * arena, const char * buf, size_t len);
char *arena_strdup(fuma_arena_t * arena, const char * buf);

void arena_discard(fuma_arena_t * arena);
void arena_reclaim(fuma_arena_t * arena);

int arena_valid_address(fuma_arena_t * arena, void *buf);

void arena_dump_stats(fuma_arena_t * arena,FILE *fp);

#if defined(__cplusplus)
}

#endif


#endif /* LIB_FUMA_ARENA_H */
