#ifndef LIB_FUMA_ARENA_ALLOCATOR_HPP
#define LIB_FUMA_ARENA_ALLOCATOR_HPP

#include <new>
#include <iostream>

#include <boost/current_function.hpp>

class Arena;

// globals
void *
global_arena_allocate(Arena & arena, size_t count, size_t nbytes);

void
global_arena_deallocate(Arena & arena, void *ptr, size_t nbytes);

// global placement operators
// 1) called as Arena a; T *t[count] = new[](a)T();
void* operator new[](size_t count, size_t nbytes, Arena & arena);

// 2) called as Arena a; T *t = new(a)T();
void* operator new(size_t nbytes, Arena & arena);

// only called if Arena::Arena() throws in a placement new
void operator delete(void * ptr, Arena & arena);

// only called if Arena::Arena() throws in a placement new[]
void operator delete[](void * ptr, size_t count, Arena & arena);

template <class T>
struct ArenaAllocationPolicy
{
    static T* allocate(size_t count, Arena & arena)
    {
//      std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
      return reinterpret_cast<T*>(global_arena_allocate(arena,count,sizeof(T)));
    }

    static void deallocate(T *ptr, size_t count, Arena & arena)
    {
//        std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n";
        global_arena_deallocate(arena,ptr, (sizeof(T)*count));
    }
};

// enables use of std::allocate_shared
// and normal STL containers
template <class T, class AllocationPolicy=ArenaAllocationPolicy<T>>
struct ArenaAllocator
{
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef const T*                    const_pointer;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef size_t                      size_type;
    typedef std::ptrdiff_t              difference_type;

    typedef std::true_type propagate_on_container_copy_assignment;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type propagate_on_container_swap;

    typedef Arena                       arena_type;
    typedef std::shared_ptr<arena_type> shared_arena_type;

    shared_arena_type m_parent;

    shared_arena_type parent() const {
        return m_parent;
    }

    ArenaAllocator(shared_arena_type parent)
        : m_parent{parent}
    {
//        std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
    }

     // CopyConstructable
    template<typename U>
    ArenaAllocator(const ArenaAllocator<U>& other) throw()
        : m_parent{other.parent()}
        {
//            std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        }

    // CopyAssignable
    ArenaAllocator<T> & operator=(const ArenaAllocator<T> & rhs)
    {
//        std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        if(&rhs != this)
        {
            m_parent = rhs.parent();
        }
        return *this;
    }

    ~ArenaAllocator() = default;

    template<typename FromT>
    struct rebind
    {
        typedef ArenaAllocator<FromT> other;
    };

   ArenaAllocator<T> select_on_container_copy_construction()
    {
//        std::cout << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        return *this;
    }

    // throwing version throws std::bad_alloc
    pointer allocate(size_type count)
    {
//        std::cout << "f() passed count: " << count << " " << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";

        pointer ptr = AllocationPolicy::allocate(count,*m_parent);
        std::cout << "allocated pointer: " << (void*)ptr << " to " << count << " element(s)" << "\n" ;
        return ptr;
    }

    // construction
    template <class U, class... Args>
    void construct(U* placement, Args && ...args)
    {
//        std::cout << "f() passed pointer: " << (void*)placement << " " << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        // explicitly use default placement operator to construct into the memory,
        // we support all overloaded constructers
        ::new((void *)placement) U(args...);
    };

    template <class U>
    void destroy(U* ptr)
    {
//        std::cout << "f() passed pointer: " << (void*)ptr << " " << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        // call destructor
        ptr->~U();
    };

    void deallocate(pointer ptr, size_type count)
    {
//        std::cout << "f() passed pointer: " << (void*)ptr << " " << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        std::cout << "released pointer: " << (void*)ptr << " to " << count << " element(s)" << "\n" ;
        AllocationPolicy::deallocate(ptr,count,*m_parent);
        ptr = nullptr;
    }

    uintptr_t parent_id() const {
        return  static_cast<uintptr_t>(&m_parent);
    }

    template <class U >
    bool may_release_memory_on_behalf_of(const ArenaAllocator<U> & rhs)
    {
//        std::cout << "f() passed pointer: " << (void*)(&rhs) << " " << BOOST_CURRENT_FUNCTION << " " << __FILE__ << ":" << __LINE__ << "\n\n";
        (parent_id() == rhs.parent_id());
    }
};
// Equivence for Allocators, means "can free each other's memory"
template <class T, class U>
bool operator==(const ArenaAllocator<T>& lhs, const ArenaAllocator<U>& rhs)
{
    // this is actually a stricter requirement than required
    return lhs.may_release_memory_on_behalf_of(rhs);
}

// Equivence for Allocators, means "can free each other's memory"
template <class T, class U>
bool operator!=(const ArenaAllocator<T>& lhs, const ArenaAllocator<U>& rhs)
{
    return !(lhs == rhs);
}

#include "hexdump.h"
struct MemoryManager
{
    template <typename CharT  = char,
              typename TraitT = std::char_traits<CharT>>
    using ArenaString = std::basic_string<CharT,TraitT,ArenaAllocator<CharT>>;

    template <typename T, template <typename ...Args> class SequenceT>
    using ArenaSequence = SequenceT<T,ArenaAllocator<T>>;

    // construct an object in memory allocated in an arena and return a shared ptr to it,
    // the control block and the contents are both allocated at once from the arena
    MemoryManager(char *base, size_t len)
        : m_arena{std::make_shared<Arena>(base,len)}
        , m_hexdumper{[&base,&len](FILE*fp){
            ::hexdump(fp,reinterpret_cast<const uint8_t *>(base),len,16);
        }}
    {
    }

    void discard(){
        m_arena->discard();
    }
    void hexdump(FILE *fp)
    {
        m_hexdumper(fp);
    }

    ~MemoryManager()
    {
    }


    template <typename T, class...Args>
    T &&
    construct(Args && ...args)
    {
        ArenaAllocator<T> alloc = {
            get_allocator<T>()
        };
        T tmp{args...,alloc};
        return std::move(tmp);
    }

    template <typename T, class...Args>
    std::shared_ptr<T>
    allocate_shared(Args && ...args)
    {
        ArenaAllocator<T> alloc = {
            get_allocator<T>()
        };
        return std::shared_ptr<T>::allocate_shared(alloc, std::forward<Args>(args)...);
    }


    template <typename T>
    ArenaAllocator<T>
    get_allocator()
    {
        return std::move(static_cast< ArenaAllocator<T> >(m_arena));
    }

    std::shared_ptr<Arena> m_arena;
    std::function<void(FILE*)> m_hexdumper;
};


#endif /* ndef LIB_FUMA_ARENA_ALLOCATOR_HPP */
