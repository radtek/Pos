#ifndef __hh_spinlock__
#define __hh_spinlock__

#include <i_spinlock.hh>
#include <windows.h>

namespace utilities {
namespace locking {

class spinlock : public i_spinlock {
public:
        spinlock();
        ~spinlock();

        void enter();
        void exit();

        bool try_enter();

private:
        spinlock(const spinlock &initializer);
        spinlock &operator=(const spinlock &rhs);

        volatile LONG lock_;
};

} /* locking */
} /* utilities */

#endif /* ! __hh_spinlock__ */

