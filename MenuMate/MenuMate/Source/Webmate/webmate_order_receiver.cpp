#include "webmate_order_receiver_impl.hh"

webmate_order_receiver_t::webmate_order_receiver_t(
  i_order_received_action_t *received_action,
  configuration_t configuration)
  : impl_(new impl_t(received_action, configuration))
{
}

webmate_order_receiver_t::~webmate_order_receiver_t()
{
    /*
     * We need to disable the IdTCPServer here, manually, as for some reason
     * the destructor of webmate_order_receiver_t::impl_t isn't being triggered.
     * That destructor would've disabled and deallocated the server.
     *
     * This is something we're going to have to investigate as the auto_ptr
     * should trigger impl_t's destructor when it releases the instance.
     */
    disable_receiver();
}

void
webmate_order_receiver_t::disable_receiver()
{
    impl_->disable_receiver();
}

void
webmate_order_receiver_t::enable_receiver()
{
    impl_->enable_receiver();
}

bool
webmate_order_receiver_t::is_receiver_enabled()
const
{
    return impl_->is_receiver_enabled();
}

#undef CONFIGURATION

