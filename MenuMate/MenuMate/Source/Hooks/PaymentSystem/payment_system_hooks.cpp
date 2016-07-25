#include <algorithm>

#include "PaymentTransaction.h"
#include "payment_system_hook.h"
#include "payment_system_hooks.h"

namespace payment_system {
namespace hooks {

payment_system_hooks_t::payment_system_hooks_t()
    : hooks_()
{
}

payment_system_hooks_t::payment_system_hooks_t(
    const payment_system_hooks_t &initializer)
    : hooks_(initializer.hooks_)
{
}

payment_system_hooks_t::~payment_system_hooks_t()
{
}

payment_system_hooks_t &
payment_system_hooks_t::operator=(const payment_system_hooks_t &rhs)
{
        if (this == &rhs)
                return *this;

        hooks_ = rhs.hooks_;
        return *this;
}

void
payment_system_hooks_t::operator()(
    const std::wstring &hook_name,
    TPaymentTransaction &payment_transaction)
{
        std::map<std::wstring,
                 std::vector<i_payment_system_hook_t *> >::iterator hook =
            hooks_.find(hook_name);

        if (hook != hooks_.end()) {
                std::vector<i_payment_system_hook_t *>::iterator i =
                    hook->second.begin();
                std::vector<i_payment_system_hook_t *>::iterator j =
                    hook->second.end();
                for ( ; i != j; ++i)
                        (**i)(payment_transaction);
        }
}

payment_system_hooks_t &
payment_system_hooks_t::add_hook(
    const std::wstring &hook_name,
    i_payment_system_hook_t *hook_function)
{
                hooks_[hook_name].push_back(hook_function);
                return *this;
}

payment_system_hooks_t &
payment_system_hooks_t::remove_hook(
    const std::wstring &hook_name,
    i_payment_system_hook_t *hook_function)
{
        std::map<std::wstring,
                 std::vector<i_payment_system_hook_t *> >::iterator i =
            hooks_.find(hook_name);

        if (i == hooks_.end())
                return *this;
        else if (hook_function != 0x0) {
                std::vector<i_payment_system_hook_t *> &function_vector =
                    i->second;
                std::vector<i_payment_system_hook_t *>::iterator j =
                    std::find(function_vector.begin(),
                              function_vector.end(),
                              hook_function);
                if (j != function_vector.end())
                        function_vector.erase(j);
                if (function_vector.size() != 0)
                    return *this;
        }

        hooks_.erase(i);
        return *this;
}

} /* hooks */
} /* payment_system */
