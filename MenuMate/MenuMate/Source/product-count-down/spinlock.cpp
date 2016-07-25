#include <spinlock.hh>

namespace utilities {
namespace locking {

spinlock::spinlock()
    : lock_(false)
{
}

spinlock::~spinlock()
{
}

void
spinlock::enter()
{
        while (InterlockedCompareExchange(&lock_, true, false))
                ;
}

void
spinlock::exit()
{
        InterlockedExchange(&lock_, false);
}

bool
spinlock::try_enter()
{
        return !InterlockedCompareExchange(&lock_, true, false);
}

} /* locking */
} /* utilities */

