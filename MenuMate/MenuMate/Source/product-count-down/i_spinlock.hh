#ifndef __hh_i_spinlock__
#define __hh_i_spinlock__

namespace utilities {
namespace locking {

class i_spinlock {
public:
        virtual ~i_spinlock() { };

        virtual void enter() = 0;
        virtual void exit() = 0;

        virtual bool try_enter() = 0;

protected:
        i_spinlock() { };

private:
        i_spinlock(const i_spinlock &initializer);
        i_spinlock &operator=(const i_spinlock &rhs);
};

class spinlock_reference {
public:
        spinlock_reference(i_spinlock &referenced_spinlock);
        ~spinlock_reference();

private:
        spinlock_reference(const spinlock_reference &initializer);
        spinlock_reference &operator=(const spinlock_reference &rhs);

        i_spinlock &spinlock_;
};

} /* locking */
} /* utilities */

#endif /* ! __hh_i_spinlock__ */

