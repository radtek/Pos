#ifndef hh_webmate_order_processor_impl
#define hh_webmate_order_processor_impl

#include "webmate_order_processor.hh"

class webmate_order_processor_t::impl_t 
  : public i_disallow_assignment_t,
    public i_disallow_copy_construction_t {
public:
	impl_t(i_process_order_action_t * const process_order_action);
    ~impl_t();

	void queue_a_file_to_be_processed(const std::wstring filename);

private:
	class necromancer_t;
	class worker_t;

	std::auto_ptr<webmate_order_processor_t::impl_t::worker_t>
	  worker_;
	std::auto_ptr<webmate_order_processor_t::impl_t::necromancer_t>
	  necromancer_;
}; /* webmate_order_processor_t::impl_t */

#endif /* ! hh_webmate_order_processor_impl */

