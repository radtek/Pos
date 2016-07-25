#ifndef hh_webmate_order_processor
#define hh_webmate_order_processor

#include <memory>
#include <string>

#include "i_disallow_assignment.hh"
#include "i_disallow_copy_construction.hh"

class webmate_order_processor_t
  : public i_disallow_assignment_t,
    public i_disallow_copy_construction_t {
public:
    /*
     * webmate_order_processor_t doesn't dictate /what/ we do to process
     * the orders. It just ensures that orders are handed off for
     * processing a sequential manner.
     *
     * The actual processing of an order is handled by a subclass of
     * this interface.
     */
    class i_process_order_action_t
      : public i_disallow_assignment_t,
        public i_disallow_copy_construction_t {
    public:
        virtual ~i_process_order_action_t() { };

        virtual bool operator()(const std::wstring filename) = 0;

    protected:
        i_process_order_action_t() { };
    }; /* i_process_order_action_t */

    webmate_order_processor_t(
      i_process_order_action_t * const process_order_action);

    void queue_a_file_to_be_processed(const std::wstring filename);

private:
    class impl_t;

    std::auto_ptr<webmate_order_processor_t::impl_t> impl_;
}; /* webmate_order_processor_t */

#endif /* ! hh_webmate_order_processor */

