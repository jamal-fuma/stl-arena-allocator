#ifndef LIB_FUMA_MEMORY_MANAGER_HPP
#define LIB_FUMA_MEMORY_MANAGER_HPP

#include "ArenaAllocator.hpp"

// std::string with arena allocator
template <typename CharT  = char,
         typename TraitT = std::char_traits<CharT>> using ArenaString = 
         std::basic_string<CharT,TraitT,ArenaAllocator<CharT>>;


struct MemoryManager
{
    MemoryManager(char *base, size_t len);
    ~MemoryManager() = default;

    // free everything, well technically forget the memory has been allocated
    // so if there are any references to this memory still active, this is a very bad function call
    // you have been warned
    void discard();

    void statdump(FILE *fp);
    void hexdump(FILE *fp);



    std::function<void(FILE*)> m_hexdumper;
};

template <typename T>
struct ArenaDeleter
{
    ArenaDeleter(ArenaAllocator<T> & alloc)
        : m_allocator(alloc)
    {
    }

    void operator()(T *ptr) const
    {
        allocator.destroy(ptr);
        allocator.deallocate(ptr);
    }

    ArenaAllocator<T> & m_allocator;
};


template <typename T, class...Args>
std::unique_ptr<T,ArenaDeleter<T>> &&
allocate_unique(ArenaAllocator<T> & allocator, Args && ...args)
{
    T* ptr = nullptr;
    try
    {
        // this should be wrapped in a new operator
        ptr = allocator.allocate(1);
        allocator.construct(ptr,std::forward<Args>(args)...);
    }
    catch(...)
    {
        // did allocation fail ?
        if(ptr)
            // nope, constructer threw
            alloc.deallocate(ptr);

        throw;
    }
    ArenaDeleter<T> deleter(allocator);
    return std::move(std::unique_ptr<T,decltype(deleter)>(std::move(ptr),deleter));
}

template <typename T, class...Args>
std::shared_ptr<T> &&
allocate_shared(ArenaAllocator<T> & alloc,Args && ...args)
{
    // construct an object in memory allocated in an arena and return a shared ptr to it,
    // the control block and the contents are both allocated at once from the arena
    // this means we don't need a custom deleter, 
    return std::move(std::shared_ptr<T>::allocate_shared(alloc, std::forward<Args>(args)...));
}

#endif /* ndef LIB_FUMA_MEMORY_MANAGER_HPP */ 
