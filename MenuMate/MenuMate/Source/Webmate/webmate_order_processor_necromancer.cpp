#include "webmate_order_processor_necromancer.hh"
#include "webmate_order_processor_worker.hh"

webmate_order_processor_t::impl_t::necromancer_t::necromancer_t(
  std::auto_ptr<webmate_order_processor_t::impl_t::worker_t> &worker_ptr_ref)
  : TThread(false),
    worker_ptr_ref_(worker_ptr_ref)
{
    /* We'll release the thread via. the auto_ptr. */
    FreeOnTerminate = false;
}

void __fastcall
webmate_order_processor_t::impl_t::necromancer_t::Execute()
{
    while (!Terminated) {
        /* Tell the worker it's time to start... */
        worker_ptr_ref_->Start();
        /* Wait for the worker to finish. Did she finish because something
         * crushed her horribly or because the job's done? If it finished
         */
        worker_ptr_ref_->WaitFor();
        if (worker_ptr_ref_->FatalException != NULL) {
            /* She died a horrible death. Let's resurrect her so we can see it
             * happen again! YAY!
             */
            worker_ptr_ref_.reset(
              new webmate_order_processor_t::impl_t::worker_t(
                    worker_ptr_ref_->process_order_action()));
        }
    }
}

