#include "hexdump.h"
#include "MemoryManager.hpp"

Arena a(buf,len);
ArenaAllocator<T>(a);
std::shared_ptr<Arena,ArenaDeleter,ArenaAllocator<Arena>>
MemoryManager::MemoryManager(char *base, size_t len)
    : m_arena{std::make_shared<Arena>(base,len)}
    , m_hexdumper{[&base,&len](FILE*fp){
        ::hexdump(fp,reinterpret_cast<const uint8_t *>(base),len,16);
    }}
{
}

void MemoryManager::statdump(FILE *fp)
{
    m_arena->dump(fp);
}

void MemoryManager::hexdump(FILE *fp)
{
    m_hexdumper(fp);
}

// free everything, well technically forget the memory has been allocated
// so if there are any references to this memory still active, this is a very bad function call
// you have been warned
void MemoryManager::discard()
{
    m_arena->discard();
}

