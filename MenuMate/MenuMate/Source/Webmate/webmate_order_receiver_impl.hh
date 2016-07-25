#ifndef hh_webmate_order_receiver_impl
#define hh_webmate_order_receiver_impl

#include <IdTCPServer.hpp>

#include "webmate_order_receiver.hh"

class webmate_order_receiver_t::impl_t
  : public i_disallow_assignment_t,
    public i_disallow_copy_construction_t {
public:
    impl_t(i_order_received_action_t *received_action,
           configuration_t configuration);
    ~impl_t();

    void disable_receiver();
    void enable_receiver();
    bool is_receiver_enabled() const;

private:
    void initialize_tcp_server();
    void __fastcall talk_with_client(TIdContext *connection_context);

    configuration_t configuration_;
    std::auto_ptr<TIdTCPServer> tcp_server_;
    i_order_received_action_t *received_action_;

    bool receiver_is_enabled_;
}; /* webmate_order_receiver_t::impl_t */

#endif /* ! hh_webmate_order_receiver_impl */

