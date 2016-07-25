#ifndef h_payment_system_hook
#define h_payment_system_hook

#include <functional>

class TPaymentTransaction;

namespace payment_system {
namespace hooks {

class i_payment_system_hook_t
    : public std::unary_function<void, TPaymentTransaction &> {
public:
        i_payment_system_hook_t();
        virtual ~i_payment_system_hook_t();

        virtual void operator()(TPaymentTransaction &) = 0;
}; /* i_payment_system_hook_t */

} /* hooks */
} /* payment_system */

#endif /* ! h_payment_system_hook */
