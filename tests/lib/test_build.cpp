#include "TestHelper.hpp"

#include "hexdump.h"
#include "Arena.hpp"
#include "DarwinTimer.hpp"
#include <vector>
#include <iomanip>
//
struct FixtureData
: public Fuma::Test::Fixture
{
    char buf[1<<11];
};

// make an arena string feel like std::string
// typedef char arena_string ;// std::basic_string<char,std::char_traits<char>,ArenaAllocator<char>> arena_string;

struct FooThatThrows
{
    FooThatThrows()
    {
        throw std::runtime_error("Oops");
    }
};


BOOST_FIXTURE_TEST_SUITE(BasicSuite, FixtureData)

BOOST_AUTO_TEST_CASE(should_handle_strings)
{
	uint64_t rounds = 1UL << 15;
	uint64_t elapsed_ns[10] ={0};
    int tests=0;
	HiResTimer timer;

	// convert to presentation
    std::cout << "Timing default construction\n";
    elapsed_ns[tests++] = profile_op<void(void)>(
            timer,
            rounds,
            [this]() {
            MemoryManager mm(buf,sizeof(buf));
    });

	// convert to presentation
    {
        MemoryManager mm(buf,sizeof(buf));
        std::cout << "Timing persistant construction\n";
        elapsed_ns[tests++] = profile_op<void(void)>(
                timer,
                rounds,
                [this,&mm]() {

                });
    }

    {
        MemoryManager mm(buf,sizeof(buf));
        std::cout << "Timing persistent, constructing an empty string\n";
        elapsed_ns[tests++] = profile_op<void(void)>(
                timer,
                rounds,
                [this,&mm]() {
                auto str = std::move(mm.construct<MemoryManager::ArenaString<char>>());
        });
    }

    std::cout << "Timing default construction + constructing an empty string\n";
    elapsed_ns[tests++] = profile_op<void(void)>(
            timer,
            rounds,
            [this]() {
            MemoryManager mm(buf,sizeof(buf));
            auto str = std::move(mm.construct<MemoryManager::ArenaString<char>>());
    });


    {
        MemoryManager mm(buf,sizeof(buf));
        std::cout << "Timing persistent, constructing an empty string adding until failure\n";
        elapsed_ns[tests++] = profile_op<void(void)>(
                timer,
                rounds,
                [this,&mm]() {
            try{
                auto str = std::move(mm.construct<MemoryManager::ArenaString<char>>("hello world"));
                int i = 102;
                while(--i)
                {
                   str += ".";
                }
                mm.discard();
             }
            catch(const std::bad_alloc & e)
            {
                mm.discard();
            }
        });
    }
    {
        std::cout << "Timing default, constructing an empty string adding until failure\n";
        elapsed_ns[tests++] = profile_op<void(void)>(
                timer,
                rounds,
                [this]() {
            try{
                MemoryManager mm(buf,sizeof(buf));
                auto str = std::move(mm.construct<MemoryManager::ArenaString<char>>());
                while(1)
                {
                   str += ".";
                }
             }
            catch(const std::bad_alloc & e)
            {
            }
        });
    }


        // make a shared pointer to a string in the arena
      //  std::shared_ptr<arena_string> shared_arena_str = {
        //    mm.allocate_shared<arena_string>(
          //          "The first thing I would like to say is this, I am very proud of making this work and I deserve pat on the back"
            //        )
     //   };

//        mm.hexdump(stdout);
}

BOOST_AUTO_TEST_CASE(should_handle_vectors)
{
    MemoryManager mm(buf,sizeof(buf));
    std::cout << "started Again" << "\n";
    mm.hexdump(stdout);

    std::vector<int,ArenaAllocator<int>> vec{mm.get_allocator<int>()};
    for(int i=0; i<100; ++i)
    {
        try {
            std::cout << "Made " << i << " push backs\n";
            vec.push_back(0x44444444);
            mm.hexdump(stdout);
        }
        catch(const std::bad_alloc & e)
        {
            std::cout << "Threw on " << i << "th push backs\n";
            break;
        }
    }
    std::cout << "Finished Vector" << "\n";
    mm.hexdump(stdout);
}

BOOST_AUTO_TEST_CASE(show_cleanup_memory_if_allocation_succeeds_but_contruction_fails)
{

    MemoryManager mm(buf,sizeof(buf));
    std::cout << "started FooThatThrows" << "\n";
    mm.hexdump(stdout);

    try {
        std::shared_ptr<FooThatThrows> shared_arena_foo = {
            mm.allocate_shared<FooThatThrows>()
        };
    }
    catch(const std::bad_alloc & e)
    {
        std::cout << "allocate_shared<FooThatThrows> failed " << "\n";
    }

    std::cout << "Finished FooThatThrows" << "\n";
    mm.hexdump(stdout);
}

BOOST_AUTO_TEST_SUITE_END()
