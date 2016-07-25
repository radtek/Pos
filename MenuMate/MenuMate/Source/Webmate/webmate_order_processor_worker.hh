#ifndef hh_webmate_order_processor_worker
#define hh_webmate_order_processor_worker

#include <Classes.hpp>
#include <SyncObjs.hpp>

#include <list>

#include "webmate_order_processor_impl.hh"

class webmate_order_processor_t::impl_t::worker_t : public TThread {
public:
    worker_t(i_process_order_action_t * const process_order_action);

    i_process_order_action_t *process_order_action();
    void queue_a_file_to_be_processed(const std::wstring filename);

private:
    void __fastcall Execute();

    std::wstring grab_a_file_to_process();

    std::list<std::wstring> files_to_be_processed_;
    i_process_order_action_t * const process_order_action_;

    std::auto_ptr<TMutex> list_lock_;
    std::auto_ptr<TSemaphore> n_files_waiting_to_be_processed_;
}; /* webmate_order_processor_t::worker_t */

#endif /* ! hh_webmate_order_processor_worker */

