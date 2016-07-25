#include "webmate_order_received_action_base.hh"

#define RECEIVED_ACTION_BASE_T webmate_order_received_action_base_t
#define I_ORDER_RECEIVED_HOOK_T RECEIVED_ACTION_BASE_T::i_order_received_hook_t

static
bool
execute_hook_chain(
  std::map<std::wstring, I_ORDER_RECEIVED_HOOK_T *> &hook_chain,
  TMutex &hook_chain_lock,
  TMemoryStream &input_buffer,
  TMemoryStream &output_buffer,
  const bool ignore_result)
{

    try {
        std::map<std::wstring,
                 I_ORDER_RECEIVED_HOOK_T *>::iterator i =
          hook_chain.begin();
        bool chain_result = true;

        hook_chain_lock.Acquire();
        for ( ; i != hook_chain.end() && chain_result != false; ++i)
            chain_result &=
              (*i->second)(input_buffer, output_buffer) | ignore_result;
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
  I_ORDER_RECEIVED_HOOK_T * const hook,
  std::map<std::wstring, I_ORDER_RECEIVED_HOOK_T *> &hook_chain,
  TMutex &hook_chain_lock)
{
    std::pair<std::map<std::wstring,
                       I_ORDER_RECEIVED_HOOK_T *>::iterator,
              bool> insertion_result;

    hook_chain_lock.Acquire();
    insertion_result =
      hook_chain.insert(
        std::pair<std::wstring,
                  I_ORDER_RECEIVED_HOOK_T *>(name, hook));
    hook_chain_lock.Release();

    return insertion_result.second;
}

static
bool
locked_hook_remove(
  const std::wstring &name,
  std::map<std::wstring, I_ORDER_RECEIVED_HOOK_T *> &hook_chain,
  TMutex &hook_chain_lock)
{
    std::map<std::wstring,
             I_ORDER_RECEIVED_HOOK_T *>::size_type n_removed;

    hook_chain_lock.Acquire();
    n_removed = hook_chain.erase(name);
    hook_chain_lock.Release();

    return n_removed != 0;
}

#undef I_ORDER_RECEIVED_HOOK_T

RECEIVED_ACTION_BASE_T::RECEIVED_ACTION_BASE_T()
  : after_hooks_(),
    after_hook_lock_(new TMutex(false)),
    before_hooks_(),
    before_hook_lock_(new TMutex(false))
{
}

RECEIVED_ACTION_BASE_T::~RECEIVED_ACTION_BASE_T()
{
}

void
RECEIVED_ACTION_BASE_T::operator()(
  TMemoryStream &input_buffer,
  TMemoryStream &output_buffer)
{
    if (before_action(input_buffer, output_buffer) == false)
        return;

    action(input_buffer, output_buffer);
    after_action(input_buffer, output_buffer);
}

bool
RECEIVED_ACTION_BASE_T::add_before_hook(
  const std::wstring name,
  i_order_received_hook_t * const hook)
{
    return locked_hook_insert(
             name, hook, before_hooks_, *before_hook_lock_);
}

bool
RECEIVED_ACTION_BASE_T::remove_before_hook(
  const std::wstring name)
{
    return locked_hook_remove(
             name, before_hooks_, *before_hook_lock_);
}

bool
RECEIVED_ACTION_BASE_T::add_after_hook(
  const std::wstring name,
  i_order_received_hook_t * const hook)
{
    return locked_hook_insert(
             name, hook, after_hooks_, *after_hook_lock_);
}

bool
RECEIVED_ACTION_BASE_T::remove_after_hook(
  const std::wstring name)
{
    return locked_hook_remove(
             name, after_hooks_, *after_hook_lock_);
}

bool
RECEIVED_ACTION_BASE_T::before_action(
  TMemoryStream &input_buffer,
  TMemoryStream &output_buffer)
{
    return execute_hook_chain(before_hooks_,
                              *before_hook_lock_,
                              input_buffer,
                              output_buffer,
                              false);
}

void
RECEIVED_ACTION_BASE_T::after_action(
  TMemoryStream &input_buffer,
  TMemoryStream &output_buffer)
{
    execute_hook_chain(after_hooks_,
                       *after_hook_lock_,
                       input_buffer,
                       output_buffer,
                       true);
}

#undef RECEIVED_ACTION_BASE_T

