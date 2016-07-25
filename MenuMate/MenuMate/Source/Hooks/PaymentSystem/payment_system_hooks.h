#ifndef h_payment_system_hooks
#define h_payment_system_hooks

#include <map>
#include <string>
#include <vector>

class TPaymentTransaction;

namespace payment_system {
namespace hooks {

class i_payment_system_hook_t;

class payment_system_hooks_t {
public:
        payment_system_hooks_t();
        payment_system_hooks_t(
            const payment_system_hooks_t &initializer);
        ~payment_system_hooks_t();

        payment_system_hooks_t &operator=(
            const payment_system_hooks_t &rhs);
        void operator()(const std::wstring &hook_name,
                        TPaymentTransaction &);

        payment_system_hooks_t &add_hook(const std::wstring &hook_name,
                                         i_payment_system_hook_t *);
        payment_system_hooks_t &remove_hook(
            const std::wstring &hook_name,
            i_payment_system_hook_t * = 0x0);
        
private:
        std::map<std::wstring,
                 std::vector<i_payment_system_hook_t *> > hooks_;
}; /* payment_system_hooks_t */

} /* hooks */
} /* payment_system */

#endif /* ! h_payment_system_hooks */

