#include <i_spinlock.hh>

namespace utilities {
namespace locking {

spinlock_reference::spinlock_reference(i_spinlock &referenced_spinlock)
    : spinlock_(referenced_spinlock)
{
        spinlock_.enter();
}

spinlock_reference::~spinlock_reference()
{
        spinlock_.exit();
}

} /* locking */
} /* utilities */

