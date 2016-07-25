#include "webmate_order_processor_impl.hh"
#include "webmate_order_processor_necromancer.hh"
#include "webmate_order_processor_worker.hh"

webmate_order_processor_t::impl_t::impl_t(
  i_process_order_action_t * const process_order_action)
  : worker_(new worker_t(process_order_action)),
    necromancer_(new necromancer_t(worker_))
{
}

webmate_order_processor_t::impl_t::~impl_t()
{
    worker_->Terminate();
    necromancer_->Terminate();

    /* The worker's potentially blocked waiting for a filename.
     * Give it a bogus one to pull it out of its sleep.
     */
    worker_->queue_a_file_to_be_processed(L"Wake up, moron!");

    /* We don't want the worker to be deallocated before the necromancer's
     * terminated so we'll wait for the necromancer here. The reason we have
     * to do this is because the worker is held on an auto_ptr and would be
     * released as soon as this destructor terminated.
     */
    necromancer_->WaitFor();
}

void
webmate_order_processor_t::impl_t::queue_a_file_to_be_processed(
  const std::wstring filename)
{
    worker_->queue_a_file_to_be_processed(filename);
}

