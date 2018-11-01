#include <iostream>
#include "linearallocator.h"

/**
 * @brief The Foo class, used to test our linear allocator
 */
class Foo {
public:
    /// Constructor - check this gets called
    Foo() {
        std::cerr << "Foo::Foo()\n";
    }

    /// A destructor - does this get called?
    ~Foo() {
        std::cerr << "Foo::~Foo()\n";
    }

    /// Overload the new operator to get memory from our linear allocator
    void * operator new(std::size_t sz) {
        // Retrieve a location from the pool allocator
        Foo* loc = (Foo*) LinearAllocatorSingleton::getInstance().alloc(sz);
        return loc;
        // Note that the constructor Foo::Foo() will get called - all this operator
        // does is get the memory from somewhere (malloc would also have worked here).
    }

    /// Always overload the delete if you overload the new (even if it doesn't do anything)
    void operator delete(void*) {
        // Here is where we're going to do some clever deallocation of the memory
        // from our linearallocator (which I've not yet implemented)

        // However, you'll notice that actually calling the destructor after our
        // special placement new is actually impossible to do from here! It must be
        // done elsewhere, or use smart pointers for everything. There are some suggestions
        // in Scott Meyers "Effective C++", but none of them are particularly attractive.
    }
private:
    /// Some goodies to shove in our class (this hopefully will data align so I don't have to stress)
    int m_data[8];

};

/**
 * @mainpage Pool Allocators made...difficult
 * In this workshop we're going to explore a bunch of interesting design patterns and
 * practices, and specifically we're going to look at new/delete overloading, pool allocators,
 * singletons and exceptions! I would like the following features, in increasing order of difficulty:
 * -# Implement a simple linear pool allocator class. Essentially this will allocate a block of memory
 *    once. Memory of a particular size can be requested from the allocator, and if there is any
 *    left a pointer will be returned. Note that simply allocating is easy enough - don't bother
 *    with deleting memory, as this will be much much more painful. Also, don't bother implementing
 *    memory reallocation unless you particularly want too - it's not needed for this exercise.
 *    Take a look at http://en.wikipedia.org/wiki/Memory_pool and at
 *    https://molecularmusings.wordpress.com/2012/08/14/memory-allocation-strategies-a-linear-allocator/
 *    (although I didn't use these, so my solution looks quite different).
 * -# Create a class with overloaded new and delete operators through which memory can be allocated
 *    from the pool. This should have some dummy data members to make it occupy some memory.
 * -# Put some exception handling in your pool allocator, so that you handle the situation when
 *    memory is requested but none is available.
 * -# Refactor your linear pool allocator as a Singleton class - this will mean that only one instance
 *    can exist. Consider using smart pointers to manage the lifetime of the memory block.
 */
int main()
{
    // Create a bunch of foo's until the allocation fails
    Foo *foo;
    try {
        for (int i=0; i<1000000;++i) {
            foo = new Foo;
        }
    }
    catch (const std::bad_alloc&) {
        std::cerr << "Allocation failed!\n";
        exit(0);
    }

    // If we're still in business, output how much memory we have left
    std::cout << "Remaining memory = "<< LinearAllocatorSingleton::getInstance().remaining() << std::endl;
    return 0;
}

