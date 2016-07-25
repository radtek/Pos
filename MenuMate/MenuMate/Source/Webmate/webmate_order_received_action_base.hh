#ifndef hh_order_received_action_base
#define hh_order_received_action_base

#include <SyncObjs.hpp>

#include <string>
#include <map>

#include "webmate_order_receiver.hh"

class webmate_order_received_action_base_t 
  : public webmate_order_receiver_t::i_order_received_action_t {
public:
    /*
     * Yes, this declaration is identical to i_order_received_action_t.
     * We're redeclaring it here with a new name for clarity. A received
     * hook is distinct from the received action. Hooks are used to add
     * behavior to the opening and closing of the reception action.
     *
     * They don't have the same meaning and they don't have the same
     * semantics.
     */
    class i_order_received_hook_t
      : public i_disallow_assignment_t,
        public i_disallow_copy_construction_t {
    public:
        virtual ~i_order_received_hook_t() { };

        virtual bool operator()(TMemoryStream &input_buffer,
                                TMemoryStream &output_buffer) = 0;

    protected:
        i_order_received_hook_t() { };
    }; /* i_order_received_hook_t */

    webmate_order_received_action_base_t();
    virtual ~webmate_order_received_action_base_t();

    void operator()(TMemoryStream &input_buffer,
                    TMemoryStream &output_buffer);

    bool add_before_hook(const std::wstring name,
                         i_order_received_hook_t * const hook);
    bool remove_before_hook(const std::wstring name);
    bool add_after_hook(const std::wstring name,
                        i_order_received_hook_t * const hook);
    bool remove_after_hook(const std::wstring name);

protected:
    virtual bool before_action(TMemoryStream &input_buffer,
                               TMemoryStream &output_buffer);
    virtual bool action(TMemoryStream &input_buffer,
                        TMemoryStream &output_buffer) = 0;
    virtual void after_action(TMemoryStream &input_buffer,
                              TMemoryStream &output_buffer);

private:
    std::map<std::wstring, i_order_received_hook_t *> after_hooks_;
    std::map<std::wstring, i_order_received_hook_t *> before_hooks_;
    std::auto_ptr<TMutex> after_hook_lock_;
    std::auto_ptr<TMutex> before_hook_lock_;
}; /* webmate_order_receiver_t::webmate_order_received_action_base_t */

#endif /* ! hh_order_received_action_base */

