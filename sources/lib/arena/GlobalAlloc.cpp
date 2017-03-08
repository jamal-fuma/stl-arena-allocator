#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "Arena.hpp"

#include <stdexcept>

//////////////////////////////////////////////////////////////////////
// global new
// 1) called as Arena a; T *t[count] = new[](a)T();
void* operator new[](size_t count, size_t nbytes, Arena & arena)
{
    std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << "\n";
    return global_arena_allocate(arena,count,nbytes);
}

// global new[]
// 2) called as Arena a; T *t = new(a)T();
void* operator new(size_t nbytes, Arena & arena)
{
    std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << "\n";
    return global_arena_allocate(arena,1,nbytes);
}

// global delete
// only called if Arena::Arena() throws in a placement new
// following a call of global void * operator new(size_t,Arena &)
void operator delete(void * ptr, Arena & arena)
{
    std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << "\n";
    global_arena_deallocate(arena,ptr,sizeof(Arena));
}

// global delete[]
// only called if Arena::Arena() throws in a placement new[]
// following a call of global void * operator new[](size_t,size_t,Arena &)
void operator delete[](void *ptr,size_t count, Arena & arena)
{
    std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << "\n";
    global_arena_deallocate(arena,ptr,sizeof(Arena)*count);
}

//////////////////////////////////////////////////////////////////////
// all allocs come from here
void * global_arena_allocate(Arena & arena, size_t count, size_t nbytes)
{
    std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << "\n";

    void *ptr = arena.allocate(nbytes*count);
    if(ptr)
        return ptr;

    throw std::bad_alloc();
}

// all frees go to here
void global_arena_deallocate(Arena & arena, void *ptr, size_t nbytes)
{
    std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << "\n";
    arena.deallocate(ptr,nbytes);
    arena.reclaim();
}

//////////////////////////////////////////////////////////////////////
