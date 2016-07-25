#include "webmate_order_processor_worker.hh"

/*
 * The slave sleeps until we wake it in the morning ...
 */
webmate_order_processor_t::impl_t::worker_t::worker_t(
  i_process_order_action_t * const process_order_action)
  : TThread(true),
    files_to_be_processed_(),
    process_order_action_(process_order_action),
    list_lock_(new TMutex(true)),
    n_files_waiting_to_be_processed_(new TSemaphore(true))
{
    /* We'll release the thread via. auto_ptr, thanks. */
    FreeOnTerminate = false;

    /*
     * The ``easy constructor'' for TSemaphore initializes it's count to
     * to one. We don't want this: we want it to be zero so that the thread
     * immediately upon it's first attempt to decrement it.
     */
    n_files_waiting_to_be_processed_->Acquire();
}

webmate_order_processor_t::i_process_order_action_t *
webmate_order_processor_t::impl_t::worker_t::process_order_action()
{
    return process_order_action_;
}

void
webmate_order_processor_t::impl_t::worker_t::queue_a_file_to_be_processed(
  const std::wstring filename)
{
    /* We want exclusive access to the list. */
    list_lock_->Acquire();
    files_to_be_processed_.push_back(filename);
    /* Increment the semaphore's value. If the worker is waiting for a new
     * filename to process, it'll wake.
     *
     * Be sure to have the semaphore increment /after/ the list insert. This
     * order ensures that the worker will not try to pop an item from the list
     * while this function is adding something.
     */
    n_files_waiting_to_be_processed_->Release();
    /* Be friendly and let others add stuff too! */
    list_lock_->Release();
}

void __fastcall
webmate_order_processor_t::impl_t::worker_t::Execute()
{
    /* Make sure to loop on the ``Terminated'' property as TThread's destructor
     * sets it during destruction. The gotcha is, is that the destructor waits
     * for the Execute(...) method to finish before it continues with it's work.
     * If we didn't loop on ``Terminated'', Execute(...)'d never stop running
     * and so the thread could never be destroyed.
     */
    while (!Terminated) {
        const std::wstring file_to_process = grab_a_file_to_process();

        /* It's no big deal if the action functor kills us as the necromancer
         * will resurrect us if needed.
         */
        (*process_order_action_)(file_to_process);

        /* We might've been told to terminate while we were sleeping.
         *
         * If this was the case, the ``filename'' given to us will be bogus
         * and would've been used only to wake us from our slumber.
         */
        if (Terminated)
            return;
    }
}

std::wstring
webmate_order_processor_t::impl_t::worker_t::grab_a_file_to_process()
{
    std::wstring file_to_be_processed;

    /* Decrement the semaphore value. We're conceptually removing an element
     * from the list with this operation.
     *
     * This will block the thread should there be no elements in the list.
     *
     * Make sure the semaphore decrement happens before locking the list. If
     * we lock the list first and ``fail'' to lock the semaphore, the thread
     * will sleep holding the list lock and cause a dead-lock as the
     * queue_file_to_be_processed(...) function requires a lock on the list
     * before it can increment the semaphore.
     *
     * Quite a mouthful, I know.
     */
    n_files_waiting_to_be_processed_->Acquire();
    /* Make sure it's safe to operate on the list. */
    list_lock_->Acquire();
    /* Get the oldest filename. */
    file_to_be_processed = files_to_be_processed_.front();
    files_to_be_processed_.pop_front();
    /* Let the others access the list again. */
    list_lock_->Release();

    return file_to_be_processed;
}


