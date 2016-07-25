#include "webmate_order_processor_impl.hh"

webmate_order_processor_t::webmate_order_processor_t(
  i_process_order_action_t * const process_order_action)
  : impl_(new impl_t(process_order_action))
{
}

void
webmate_order_processor_t::queue_a_file_to_be_processed(
  const std::wstring filename)
{
    /* This is safe, locking is performed in the implementation. */
    impl_->queue_a_file_to_be_processed(filename);
}

