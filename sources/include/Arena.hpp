#ifndef LIB_FUMA_ARENA_HPP
#define LIB_FUMA_ARENA_HPP

#include "arena.h"

#include <string>
#include <memory>

//  http://en.cppreference.com/w/cpp/concept/Allocator
class Arena
: public fuma_arena_t
{
    public:
        Arena(char *buf=nullptr, size_t len=0);
        ~Arena();

        void discard(void);
        void reclaim(void);

        void reset(char *buf,len);

        // this is the global heap
        // so we can make_shared<arena> and control which arena is in use
        // clearly changing this willy nilly will spell disaster
        static Arena & instance()
        {
            return Arena::m_base;
        }

        char * strdup_no_throw(const char *str);
        char * strndup_no_throw(const char *str,size_t len);
        char * strndup_no_throw(const std::string & str, size_t len);

        // obtain memory from the pool, return NULL on failure
        void *allocate(size_t bytes);

        // deallocate the memory, modifies errno on failure
        // deallocate from which ever pool this came from
        // if you pass a pointer which did not come from arena_alloc_bytes()
        // bad things will happen.
        void deallocate(void *ptr, size_t len);

        // does a given arena own this pointer
        bool valid_address(void *ptr);

        // these obtain memory from global_arena_allocate()
        // 1) called as new Arena
        static void* operator new(size_t nbytes) throw();
        // 2) ? - returns memory unmodified as everyone seems too
        static void* operator new(size_t nbytes, void* locality_hint) throw();
        // 3) called as new Arena[count]
        static void* operator new[](size_t count, size_t nbytes) throw();

        // these release memory with global_arena_deallocate()
        // 1) called as delete arena
        static void operator delete(void * ptr);
        // 2) called as delete[] arena
        static void operator delete[](void *ptr, size_t count);

    private:
        // like highlander says, there can be only one
        static Arena m_base;
        Arena(const Arena &rhs) = delete;
        Arena operator=(const Arena & rhs) = delete;
};
#endif /* ndef LIB_FUMA_ARENA_HPP */
