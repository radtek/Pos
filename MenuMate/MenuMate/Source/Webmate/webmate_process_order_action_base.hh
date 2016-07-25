#ifndef hh_process_order_action_base
#define hh_process_order_action_base

#include <SyncObjs.hpp>

#include <string>
#include <map>

#include "webmate_order_processor.hh"

class webmate_process_order_action_base_t 
  : public webmate_order_processor_t::i_process_order_action_t {
public:
    class i_process_order_hook_t
      : public i_disallow_assignment_t,
        public i_disallow_copy_construction_t {
    public:
        virtual ~i_process_order_hook_t() { };

        virtual bool operator()(const std::wstring filename) = 0;

    protected:
        i_process_order_hook_t() { };
    }; /* i_process_order_hook_t */

    webmate_process_order_action_base_t();
    virtual ~webmate_process_order_action_base_t();

    bool operator()(const std::wstring filename);

    bool add_before_hook(const std::wstring name,
                         i_process_order_hook_t * const hook);
    bool remove_before_hook(const std::wstring name);
    bool add_after_hook(const std::wstring name,
                        i_process_order_hook_t * const hook);
    bool remove_after_hook(const std::wstring name);

protected:
    virtual bool before_action(const std::wstring filename);
    virtual bool action(const std::wstring filename) = 0;
    virtual void after_action(const std::wstring filename);

private:
    std::map<std::wstring, i_process_order_hook_t *> after_hooks_;
    std::map<std::wstring, i_process_order_hook_t *> before_hooks_;
    std::auto_ptr<TMutex> after_hook_lock_;
    std::auto_ptr<TMutex> before_hook_lock_;
}; /* webmate_order_processor_t::webmate_process_order_action_base_t */

#endif /* ! hh_process_order_action_base */

