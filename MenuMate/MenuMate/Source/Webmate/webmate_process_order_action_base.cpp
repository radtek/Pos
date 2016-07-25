#include "webmate_process_order_action_base.hh"

#define PROCESS_ORDER_ACTION_BASE_T webmate_process_order_action_base_t
#define I_PROCESS_ORDER_HOOK_T \
  PROCESS_ORDER_ACTION_BASE_T::i_process_order_hook_t

static
bool
execute_hook_chain(
  std::map<std::wstring, I_PROCESS_ORDER_HOOK_T *> &hook_chain,
  TMutex &hook_chain_lock,
  const std::wstring filename,
  const bool ignore_result)
{

    try {
        std::map<std::wstring,
                 I_PROCESS_ORDER_HOOK_T *>::iterator i =
          hook_chain.begin();
        bool chain_result = true;

        hook_chain_lock.Acquire();
        for ( ; i != hook_chain.end() && chain_result != false; ++i)
            chain_result &= (*i->second)(filename) | ignore_result;
        hook_chain_lock.Release();

        return chain_result;
    } catch (...) {
        hook_chain_lock.Release();
        return false;
    }
}

static
bool
locked_hook_insert(
  const std::wstring &name,
  I_PROCESS_ORDER_HOOK_T * const hook,
  std::map<std::wstring, I_PROCESS_ORDER_HOOK_T *> &hook_chain,
  TMutex &hook_chain_lock)
{
    std::pair<std::map<std::wstring,
                       I_PROCESS_ORDER_HOOK_T *>::iterator,
              bool> insertion_result;

    hook_chain_lock.Acquire();
    insertion_result =
      hook_chain.insert(
        std::pair<std::wstring,
                  I_PROCESS_ORDER_HOOK_T *>(name, hook));
    hook_chain_lock.Release();

    return insertion_result.second;
}

static
bool
locked_hook_remove(
  const std::wstring &name,
  std::map<std::wstring, I_PROCESS_ORDER_HOOK_T *> &hook_chain,
  TMutex &hook_chain_lock)
{
    std::map<std::wstring,
             I_PROCESS_ORDER_HOOK_T *>::size_type n_removed;

    hook_chain_lock.Acquire();
    n_removed = hook_chain.erase(name);
    hook_chain_lock.Release();

    return n_removed != 0;
}

#undef I_ORDER_RECEIVED_HOOK_T

PROCESS_ORDER_ACTION_BASE_T::PROCESS_ORDER_ACTION_BASE_T()
  : after_hooks_(),
    after_hook_lock_(new TMutex(false)),
    before_hooks_(),
    before_hook_lock_(new TMutex(false))
{
}

PROCESS_ORDER_ACTION_BASE_T::~PROCESS_ORDER_ACTION_BASE_T()
{
}

bool
PROCESS_ORDER_ACTION_BASE_T::operator()(
  const std::wstring filename)
{
      if (execute_hook_chain(before_hooks_,
                             *before_hook_lock_,
                             filename,
                             false) == false 
          || action(filename) == false)
        return false;

    execute_hook_chain(after_hooks_,
                       *after_hook_lock_,
                       filename,
                       true);

    return true;
}

bool
PROCESS_ORDER_ACTION_BASE_T::add_before_hook(
  const std::wstring name,
  i_process_order_hook_t * const hook)
{
    return locked_hook_insert(
             name, hook, before_hooks_, *before_hook_lock_);
}

bool
PROCESS_ORDER_ACTION_BASE_T::remove_before_hook(
  const std::wstring name)
{
    return locked_hook_remove(
             name, before_hooks_, *before_hook_lock_);
}

bool
PROCESS_ORDER_ACTION_BASE_T::add_after_hook(
  const std::wstring name,
  i_process_order_hook_t * const hook)
{
    return locked_hook_insert(
             name, hook, after_hooks_, *after_hook_lock_);
}

bool
PROCESS_ORDER_ACTION_BASE_T::remove_after_hook(
  const std::wstring name)
{
    return locked_hook_remove(
             name, after_hooks_, *after_hook_lock_);
}

bool
PROCESS_ORDER_ACTION_BASE_T::before_action(
  const std::wstring filename)
{
    return execute_hook_chain(before_hooks_,
                              *before_hook_lock_,
                              filename,
                              false);
}

void
PROCESS_ORDER_ACTION_BASE_T::after_action(
  const std::wstring filename)
{
    execute_hook_chain(after_hooks_,
                       *after_hook_lock_,
                       filename,
                       true);
}

#undef RECEIVED_ACTION_BASE_T

