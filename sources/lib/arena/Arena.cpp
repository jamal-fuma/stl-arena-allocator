#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "Arena.hpp"
#include <boost/current_function.hpp>
#include <iostream>

// holds the secret space for arena self allocation
// this lets us bootstrap the dynamic allocation system
static char buf[1<<11] = {0};

// the single definition of our static member
Arena Arena::m_base{buf,sizeof(buf)};


Arena::Arena(char *buf, size_t len)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    ::arena_init(this,buf,len);
}

Arena::~Arena()
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    ::arena_finit(this);
}

void Arena::discard()
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    ::arena_discard(this);
}

void Arena::reclaim()
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    ::arena_reclaim(this);
}

char * Arena::strdup_no_throw(const char *str)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return ::arena_strdup(this,str);
}

char * Arena::strndup_no_throw(const char *str, size_t len)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return ::arena_strndup(this,str,len);
}

char * Arena::strndup_no_throw(const std::string & str, size_t len)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return strndup_no_throw(str.c_str(),len);
}

void * Arena::allocate(size_t bytes)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return ::arena_alloc_bytes(this,bytes);
}

bool Arena::valid_address(void *ptr)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return ::arena_valid_address(this,ptr);
}

void Arena::deallocate(void * ptr, size_t nbytes)
{
//    std::cout << "Arena: " << (void*)this << " " << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return ::arena_free_bytes(this,ptr,nbytes);
}

// these obtain memory from global_arena_allocate()
// 1) called as new Arena
void* Arena::operator new(size_t nbytes) throw()
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return global_arena_allocate(Arena::m_base,1,nbytes);
}

// 2) ? - returns memory unmodified as everyone seems too
void* Arena::operator new(size_t nbytes, void* locality_hint) throw()
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return locality_hint;
}

// 3) called as new Arena[count]
void* Arena::operator new[](size_t count, size_t nbytes) throw()
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    return global_arena_allocate(Arena::m_base,count,nbytes);
}
// these release memory with global_arena_deallocate()
// 1) called as delete arena
void Arena::operator delete(void * ptr)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    global_arena_deallocate(Arena::m_base,ptr,sizeof(Arena));
}

// 2) called as delete[] arena
void Arena::operator delete[](void *ptr, size_t count)
{
//    std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
    global_arena_deallocate(Arena::m_base,ptr,sizeof(Arena)*count);
}
